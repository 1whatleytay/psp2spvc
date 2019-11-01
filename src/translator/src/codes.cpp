#include <translator/translator.h>

#include <gxp/block.h>

#include <util/util.h>

#include <fmt/format.h>

spv::Id CompilerGXP::resolveAlias(spv::Id id) {
    while (idAliases.find(id) != idAliases.end())
        id = idAliases[id];

    return id;
}

void CompilerGXP::useRegister(spv::Id id) {
    if (!config.optimizeRegisterSpace)
        return;

    id = resolveAlias(id);

    if (idUsesLeft[id] == 0)
        throw std::runtime_error(fmt::format("Id {} has no more uses.", id));

    idUsesLeft[id]--;
}

usse::RegisterReference CompilerGXP::getRegister(spv::Id id) {
    auto varying = idVaryings.find(id);
    if (varying != idVaryings.end())
        return getOrThrow(varyingReferences, getOrThrow(idVaryings, id));

    auto reg = idRegisters.find(resolveAlias(id));
    if (reg != idRegisters.end()) {
        useRegister(id);
        return reg->second.reference;
    }

    auto *constant = maybe_get<SPIRConstant>(id);
    if (constant) {
        // FP Constant only
        auto type = get_type(constant->constant_type);
        assert(type.vecsize <= 1
            && type.columns <= 1
            && type.basetype == SPIRType::Float);
        int32_t regIndex = usse::getFPConstantIndex(constant->m.c[0].r[0].f32);
        if (regIndex != -1) {
            return usse::RegisterReference({ usse::Type::Float32, 1, 1 }, usse::RegisterBank::FloatConstant, regIndex);
        }
    }

    throw std::runtime_error(fmt::format("Cannot find register, varying or constant with id {}.", id));
}

void CompilerGXP::writeRegister(spv::Id id, TranslatorReference reg) {
    if (idRegisters.find(id) == idRegisters.end()) {
        idRegisters[id] = std::move(reg);
        useRegister(id);
    } else {
        throw std::runtime_error(fmt::format("SSA Violation, id {} was assigned twice.", id));
    }
}

void CompilerGXP::aliasRegister(spv::Id empty, spv::Id value) {
    // Resolve aliases (so you can make aliases of aliases).
    empty = resolveAlias(empty);
    value = resolveAlias(value);

    // Link source -> destination.
    idAliases[empty] = value;
    idRegisters.erase(empty);

    if (config.optimizeRegisterSpace) {
        // Keep destination alive until source is fully used.
        idUsesLeft[value] += idUsesLeft[empty];
    }
}

void CompilerGXP::cleanupRegisters() {
    for (const auto &uses : idUsesLeft) {
        if (uses.second == 0 && !contains(idsCleaned, uses.first) && idAliases.find(uses.first) == idAliases.end()) {
            usse::RegisterReference reg = getOrThrow(idRegisters, uses.first).reference;

            if (reg.bank == usse::RegisterBank::Temporary) {
                builder.freeRegister(reg);
                idsCleaned.push_back(uses.first);
            }
        }
    }
}

void CompilerGXP::unimplemented(const TranslatorArguments &arguments) {
    throw std::runtime_error(fmt::format("{} is not implemented.", arguments.code.name));
}

void CompilerGXP::undefined(const TranslatorArguments &arguments) {
    throw std::runtime_error(fmt::format("Undefined instruction for code {}.", static_cast<uint32_t>(arguments.code.op)));
}

void CompilerGXP::opLoad(const TranslatorArguments &arguments) {
    spv::Id type = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id pointer = arguments.instruction[2];

    usse::RegisterReference reg = getRegister(pointer);

    if (config.optimizeRegisterSpace && idUsesLeft[pointer] == 0) {
        // If result allocation is going to be freed right after, just alias.
        aliasRegister(result, pointer);
    } else {
        // If not, allocate more space.
        usse::RegisterReference destination = builder.allocateRegister(usse::RegisterBank::Temporary, reg.type);
        arguments.block.createPack(reg, destination);
        writeRegister(result, { destination });
    }
}

void CompilerGXP::opStore(const TranslatorArguments &arguments) {
    spv::Id destination = arguments.instruction[0];
    spv::Id source = arguments.instruction[1];

    usse::RegisterReference sourceRegister = getRegister(source);
    usse::RegisterReference destinationRegister = getRegister(destination);

    if (sourceRegister.type.components == 1)
        arguments.block.createMov(sourceRegister, destinationRegister);
    else
        arguments.block.createPack(sourceRegister, destinationRegister);
}

void CompilerGXP::opMatrixTimesVector(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id matrix = arguments.instruction[2];
    spv::Id vector = arguments.instruction[3];

    usse::RegisterReference matrixRegister = getRegister(matrix);
    usse::RegisterReference vectorRegister = getRegister(vector);

    assert(matrixRegister.type.type == vectorRegister.type.type);
    assert(matrixRegister.type.arraySize == vectorRegister.type.components);

    usse::RegisterReference internal = builder.allocateRegister(
        usse::RegisterBank::Internal, vectorRegister.type);

    usse::RegisterReference temp = builder.allocateRegister(
        usse::RegisterBank::Temporary, vectorRegister.type);

    arguments.block.createPack(vectorRegister, internal);

    for (uint32_t a = 0; a < vectorRegister.type.components; a++) {
        arguments.block.createDot(matrixRegister.getElement(a), internal, temp.getComponents(a, 1));
    }

    builder.freeRegister(internal);
    writeRegister(result, { temp });
}

void CompilerGXP::opVectorTimesScalar(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id vectorId = arguments.instruction[2];
    spv::Id scalarId = arguments.instruction[3];

    usse::RegisterReference vector = getRegister(vectorId);
    usse::RegisterReference scalar = getRegister(scalarId);
    usse::RegisterReference destination = builder.allocateRegister(usse::RegisterBank::Temporary, vector.type);

    scalar.swizzle = usse::getSwizzleVec4All(usse::SwizzleChannel::X);
    scalar.lockSwizzle = true;
    scalar.type.components = vector.type.components;

    arguments.block.createMul(vector, scalar, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::opCompositeExtract(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id sourceId = arguments.instruction[2];
    uint32_t index = arguments.instruction[3];

    usse::RegisterReference source = getRegister(sourceId);

    writeRegister(result, { source.getComponents(index, 1) });
}

void CompilerGXP::opCompositeConstruct(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];

    SPIRType type = get_type(typeId);

    usse::RegisterReference output = builder.allocateRegister(usse::RegisterBank::Temporary,
        { translateType(type.basetype), type.vecsize, 1 });

    for (size_t a = 0; a < type.vecsize;) {
        spv::Id sourceId = arguments.instruction[2 + a];
        usse::RegisterReference source = getRegister(sourceId);

        uint32_t size = 1;
        while (a + size < type.vecsize) {
            usse::RegisterReference next = getRegister(arguments.instruction[2 + a + size]);
            bool matchingBanks = source.bank == next.bank;
            bool matchingIndices = (source.index + static_cast<uint32_t>(source.swizzle[0]) + size) ==
                (next.index + static_cast<uint32_t>(next.swizzle[0]));
            if (matchingBanks && matchingIndices) {
                size++;
            } else {
                break;
            }
        }

        if (size == 1)
            arguments.block.createMov(usse::RegisterReference(
                { source.type.type, size, 1 }, source.bank, source.index), output.getComponents(a, size));
        else
            arguments.block.createPack(usse::RegisterReference(
                { source.type.type, size, 1 }, source.bank, source.index), output.getComponents(a, size));

        a += size;
    }

    writeRegister(result, { output });
}

void CompilerGXP::opAccessChain(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id base = arguments.instruction[2];

    TranslatorReference ref;

    auto builtIn = static_cast<spv::BuiltIn>(0);
    uint32_t builtInValue = get<SPIRConstant>(arguments.instruction[3]).m.c[0].r[0].u32;

    SPIRType type = get_type_from_variable(base);

    if (type.basetype == SPIRType::Struct && is_member_builtin(type, builtInValue, &builtIn)) {
        writeRegister(result, { getOrThrow(varyingReferences, translateVarying(builtIn)) });
        return;
    } else {
        ref = getOrThrow(idRegisters, base);
    }

    for (uint32_t a = 3; a < arguments.wordCount - 1; a++) {
        spv::Id index = arguments.instruction[a];

        uint32_t value = get<SPIRConstant>(index).m.c[0].r[0].u32;

        if (ref.isStruct()) {
            TranslatorReference temp = ref.subreferences[value];
            ref = temp;
        } else if (ref.reference.type.arraySize > 1) {
            ref = { ref.reference.getElement(value) };
        } else if (ref.reference.type.components > 1) {
            ref = { ref.reference.getComponents(value, 1) };
        } else {
            throw std::runtime_error("Access Chain to a non composite type.");
        }
    }

    writeRegister(result, ref);
}


void CompilerGXP::opVectorShuffle(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[2];
    spv::Id secondId = arguments.instruction[3];

    SPIRType type = get_type(typeId);

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference temp = builder.allocateRegister(
        usse::RegisterBank::Temporary, translateType(type));

    for (uint32_t a = 0; a < type.vecsize; a++) {
        uint32_t index = arguments.instruction[4 + a];

        usse::RegisterReference source;

        if (index < first.type.components)
            source = first.getComponents(index, 1);
        else
            source = second.getComponents(index - first.type.components, 1);

        arguments.block.createMov(source, temp.getComponents(a, 1));
    }

    writeRegister(result, { temp });
}

void CompilerGXP::opFNegate(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id sourceId = arguments.instruction[2];

    usse::RegisterReference source = getRegister(sourceId);
    usse::RegisterReference destination = builder.allocateRegister(usse::RegisterBank::Temporary, source.type);

    int32_t zeroFP = usse::getFPConstantIndex(0);
    usse::RegisterReference zero(source.type, usse::RegisterBank::FloatConstant, zeroFP);

    arguments.block.createSub(zero, source, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::opFAdd(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[2];
    spv::Id secondId = arguments.instruction[3];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, first.type);

    arguments.block.createAdd(first, second, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::opFSub(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[2];
    spv::Id secondId = arguments.instruction[3];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, first.type);

    arguments.block.createSub(first, second, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::opFMul(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[2];
    spv::Id secondId = arguments.instruction[3];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, first.type);

    arguments.block.createMul(first, second, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::opDot(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[2];
    spv::Id secondId = arguments.instruction[3];

    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, { usse::Type::Float32, 1, 1 });

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference internal = builder.allocateRegister(
        usse::RegisterBank::Internal, second.type);

    arguments.block.createPack(second, internal);
    arguments.block.createDot(first, internal, destination);

    builder.freeRegister(internal);

    writeRegister(result, { destination });
}

void CompilerGXP::opFunctionCall(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id functionId = arguments.instruction[2];

    SPIRFunction function = get<SPIRFunction>(functionId);

    for (size_t a = 0; a < function.arguments.size(); a++) {
        // Should be alias here, not writeRegister.
        spv::Id moveToId = function.arguments[a].id;
        spv::Id moveFromId = arguments.instruction[3 + a];

        // Function use counts have not been created yet (until createFunction). Allow this assignment.
        if (config.optimizeRegisterSpace)
            idUsesLeft[moveToId]++;

        aliasRegister(moveToId, moveFromId);
    }

    spv::Id returnValue = createFunction(function);
    if (returnValue != 0)
        aliasRegister(result, returnValue);
}

void CompilerGXP::opExtInst(const TranslatorArguments &arguments) {
    auto set = static_cast<SPIRExtension::Extension>(arguments.instruction[2]);
    auto literal = static_cast<GLSLstd450>(arguments.instruction[3]);

    TranslatorImplementation implementation = getOrThrow(getOrThrow(extensions, set), literal);

    (this->*implementation)(arguments);
}

void CompilerGXP::extGLSLNormalize(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id sourceId = arguments.instruction[4];

    usse::RegisterReference source = getRegister(sourceId);
    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, source.type);

    usse::RegisterReference temporary = builder.allocateRegister(
        usse::RegisterBank::Internal, source.type);
    usse::RegisterReference magnitude = builder.allocateRegister(
        usse::RegisterBank::Internal, { source.type.type, 1, 1 });

    arguments.block.createPack(source, temporary);
    arguments.block.createDot(temporary, temporary, magnitude);
    arguments.block.createReverseSquareRoot(magnitude, magnitude);

    arguments.block.createMul(temporary, magnitude.getExpanded(source.type.components), destination);

    builder.freeRegister(magnitude);
    builder.freeRegister(temporary);

    writeRegister(result, { destination });
}

void CompilerGXP::extGLSLFMin(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[4];
    spv::Id secondId = arguments.instruction[5];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);
    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, first.type);

    arguments.block.createMin(first, second, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::extGLSLFMax(const TranslatorArguments &arguments) {
    spv::Id typeId = arguments.instruction[0];
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[4];
    spv::Id secondId = arguments.instruction[5];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);
    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, first.type);

    arguments.block.createMax(first, second, destination);

    writeRegister(result, { destination });
}

void CompilerGXP::extGLSLReflect(const TranslatorArguments &arguments) {
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[4];
    spv::Id secondId = arguments.instruction[5];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference internal = builder.allocateRegister(
        usse::RegisterBank::Internal, second.type);
    usse::RegisterReference magnitude = builder.allocateRegister(
        usse::RegisterBank::Internal, { usse::Type::Float32, 1, 1 })
            .getExpanded(second.type.components);
    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, second.type);

    usse::RegisterReference two = usse::RegisterReference({ usse::Type::Float32, 1, 1 },
        usse::RegisterBank::FloatConstant, usse::getFPConstantIndex(2)).getExpanded(1);

    arguments.block.createPack(second, internal);
    arguments.block.createDot(first, internal, magnitude);
    arguments.block.createMul(two, magnitude, magnitude);
    arguments.block.createMul(magnitude, first, destination);
    arguments.block.createSub(internal, destination, destination);

    builder.freeRegister(magnitude);
    builder.freeRegister(internal);

    writeRegister(result, { destination });
}

void CompilerGXP::extGLSLPow(const TranslatorArguments &arguments) {
    spv::Id result = arguments.instruction[1];
    spv::Id firstId = arguments.instruction[4];
    spv::Id secondId = arguments.instruction[5];

    usse::RegisterReference first = getRegister(firstId);
    usse::RegisterReference second = getRegister(secondId);

    usse::RegisterReference destination = builder.allocateRegister(
        usse::RegisterBank::Temporary, { usse::Type::Float32, 1, 1 });

    // Thank you xyz for fixing my Vita related problem.
    // e^(b*log(a))

    arguments.block.createLog(first, destination);
    arguments.block.createMul(destination, second, destination);
    arguments.block.createExp(destination, destination);

    writeRegister(result, { destination });
}

TranslatorArguments::TranslatorArguments(
    gxp::Block &block,
    const TranslatorCode &code,
    const uint32_t *instruction,
    uint32_t wordCount) :
    block(block),
    code(code),
    instruction(instruction),
    wordCount(wordCount)
    { }

void CompilerGXP::createTranslators() {
    // This could be a map but there are only so many undefineds so I couldn't bother.
    codes = {
        { spv::Op::OpNop, "OpNop", &CompilerGXP::unimplemented },
        { spv::Op::OpUndef, "OpUndef", &CompilerGXP::unimplemented },
        { spv::Op::OpSourceContinued, "OpSourceContinued", &CompilerGXP::unimplemented },
        { spv::Op::OpSource, "OpSource", &CompilerGXP::unimplemented },
        { spv::Op::OpSourceExtension, "OpSourceExtension", &CompilerGXP::unimplemented },
        { spv::Op::OpName, "OpName", &CompilerGXP::unimplemented },
        { spv::Op::OpMemberName, "OpMemberName", &CompilerGXP::unimplemented },
        { spv::Op::OpString, "OpString", &CompilerGXP::unimplemented },
        { spv::Op::OpLine, "OpLine", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(9), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpExtension, "OpExtension", &CompilerGXP::unimplemented },
        { spv::Op::OpExtInstImport, "OpExtInstImport", &CompilerGXP::unimplemented },
        { spv::Op::OpExtInst, "OpExtInst", &CompilerGXP::opExtInst },
        { static_cast<spv::Op>(13), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpMemoryModel, "OpMemoryModel", &CompilerGXP::unimplemented },
        { spv::Op::OpEntryPoint, "OpEntryPoint", &CompilerGXP::unimplemented },
        { spv::Op::OpExecutionMode, "OpExecutionMode", &CompilerGXP::unimplemented },
        { spv::Op::OpCapability, "OpCapability", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(18), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpTypeVoid, "OpTypeVoid", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeBool, "OpTypeBool", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeInt, "OpTypeInt", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeFloat, "OpTypeFloat", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeVector, "OpTypeVector", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeMatrix, "OpTypeMatrix", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeImage, "OpTypeImage", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeSampler, "OpTypeSampler", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeSampledImage, "OpTypeSampledImage", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeArray, "OpTypeArray", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeRuntimeArray, "OpTypeRuntimeArray", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeStruct, "OpTypeStruct", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeOpaque, "OpTypeOpaque", &CompilerGXP::unimplemented },
        { spv::Op::OpTypePointer, "OpTypePointer", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeFunction, "OpTypeFunction", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeEvent, "OpTypeEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeDeviceEvent, "OpTypeDeviceEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeReserveId, "OpTypeReserveId", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeQueue, "OpTypeQueue", &CompilerGXP::unimplemented },
        { spv::Op::OpTypePipe, "OpTypePipe", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeForwardPointer, "OpTypeForwardPointer", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(40), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpConstantTrue, "OpConstantTrue", &CompilerGXP::unimplemented },
        { spv::Op::OpConstantFalse, "OpConstantFalse", &CompilerGXP::unimplemented },
        { spv::Op::OpConstant, "OpConstant", &CompilerGXP::unimplemented },
        { spv::Op::OpConstantComposite, "OpConstantComposite", &CompilerGXP::unimplemented },
        { spv::Op::OpConstantSampler, "OpConstantSampler", &CompilerGXP::unimplemented },
        { spv::Op::OpConstantNull, "OpConstantNull", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(47), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpSpecConstantTrue, "OpSpecConstantTrue", &CompilerGXP::unimplemented },
        { spv::Op::OpSpecConstantFalse, "OpSpecConstantFalse", &CompilerGXP::unimplemented },
        { spv::Op::OpSpecConstant, "OpSpecConstant", &CompilerGXP::unimplemented },
        { spv::Op::OpSpecConstantComposite, "OpSpecConstantComposite", &CompilerGXP::unimplemented },
        { spv::Op::OpSpecConstantOp, "OpSpecConstantOp", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(53), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpFunction, "OpFunction", &CompilerGXP::unimplemented },
        { spv::Op::OpFunctionParameter, "OpFunctionParameter", &CompilerGXP::unimplemented },
        { spv::Op::OpFunctionEnd, "OpFunctionEnd", &CompilerGXP::unimplemented },
        { spv::Op::OpFunctionCall, "OpFunctionCall", &CompilerGXP::opFunctionCall },
        { static_cast<spv::Op>(58), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpVariable, "OpVariable", &CompilerGXP::unimplemented },
        { spv::Op::OpImageTexelPointer, "OpImageTexelPointer", &CompilerGXP::unimplemented },
        { spv::Op::OpLoad, "OpLoad", &CompilerGXP::opLoad },
        { spv::Op::OpStore, "OpStore", &CompilerGXP::opStore },
        { spv::Op::OpCopyMemory, "OpCopyMemory", &CompilerGXP::unimplemented },
        { spv::Op::OpCopyMemorySized, "OpCopyMemorySized", &CompilerGXP::unimplemented },
        { spv::Op::OpAccessChain, "OpAccessChain", &CompilerGXP::opAccessChain },
        { spv::Op::OpInBoundsAccessChain, "OpInBoundsAccessChain", &CompilerGXP::unimplemented },
        { spv::Op::OpPtrAccessChain, "OpPtrAccessChain", &CompilerGXP::unimplemented },
        { spv::Op::OpArrayLength, "OpArrayLength", &CompilerGXP::unimplemented },
        { spv::Op::OpGenericPtrMemSemantics, "OpGenericPtrMemSemantics", &CompilerGXP::unimplemented },
        { spv::Op::OpInBoundsPtrAccessChain, "OpInBoundsPtrAccessChain", &CompilerGXP::unimplemented },
        { spv::Op::OpDecorate, "OpDecorate", &CompilerGXP::unimplemented },
        { spv::Op::OpMemberDecorate, "OpMemberDecorate", &CompilerGXP::unimplemented },
        { spv::Op::OpDecorationGroup, "OpDecorationGroup", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupDecorate, "OpGroupDecorate", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupMemberDecorate, "OpGroupMemberDecorate", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(76), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpVectorExtractDynamic, "OpVectorExtractDynamic", &CompilerGXP::unimplemented },
        { spv::Op::OpVectorInsertDynamic, "OpVectorInsertDynamic", &CompilerGXP::unimplemented },
        { spv::Op::OpVectorShuffle, "OpVectorShuffle", &CompilerGXP::opVectorShuffle },
        { spv::Op::OpCompositeConstruct, "OpCompositeConstruct", &CompilerGXP::opCompositeConstruct },
        { spv::Op::OpCompositeExtract, "OpCompositeExtract", &CompilerGXP::opCompositeExtract },
        { spv::Op::OpCompositeInsert, "OpCompositeInsert", &CompilerGXP::unimplemented },
        { spv::Op::OpCopyObject, "OpCopyObject", &CompilerGXP::unimplemented },
        { spv::Op::OpTranspose, "OpTranspose", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(85), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpSampledImage, "OpSampledImage", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleImplicitLod, "OpImageSampleImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleExplicitLod, "OpImageSampleExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleDrefImplicitLod, "OpImageSampleDrefImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleDrefExplicitLod, "OpImageSampleDrefExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleProjImplicitLod, "OpImageSampleProjImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleProjExplicitLod, "OpImageSampleProjExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleProjDrefImplicitLod, "OpImageSampleProjDrefImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSampleProjDrefExplicitLod, "OpImageSampleProjDrefExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageFetch, "OpImageFetch", &CompilerGXP::unimplemented },
        { spv::Op::OpImageGather, "OpImageGather", &CompilerGXP::unimplemented },
        { spv::Op::OpImageDrefGather, "OpImageDrefGather", &CompilerGXP::unimplemented },
        { spv::Op::OpImageRead, "OpImageRead", &CompilerGXP::unimplemented },
        { spv::Op::OpImageWrite, "OpImageWrite", &CompilerGXP::unimplemented },
        { spv::Op::OpImage, "OpImage", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQueryFormat, "OpImageQueryFormat", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQueryOrder, "OpImageQueryOrder", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQuerySizeLod, "OpImageQuerySizeLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQuerySize, "OpImageQuerySize", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQueryLod, "OpImageQueryLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQueryLevels, "OpImageQueryLevels", &CompilerGXP::unimplemented },
        { spv::Op::OpImageQuerySamples, "OpImageQuerySamples", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(108), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpConvertFToU, "OpConvertFToU", &CompilerGXP::unimplemented },
        { spv::Op::OpConvertFToS, "OpConvertFToS", &CompilerGXP::unimplemented },
        { spv::Op::OpConvertSToF, "OpConvertSToF", &CompilerGXP::unimplemented },
        { spv::Op::OpConvertUToF, "OpConvertUToF", &CompilerGXP::unimplemented },
        { spv::Op::OpUConvert, "OpUConvert", &CompilerGXP::unimplemented },
        { spv::Op::OpSConvert, "OpSConvert", &CompilerGXP::unimplemented },
        { spv::Op::OpFConvert, "OpFConvert", &CompilerGXP::unimplemented },
        { spv::Op::OpQuantizeToF16, "OpQuantizeToF16", &CompilerGXP::unimplemented },
        { spv::Op::OpConvertPtrToU, "OpConvertPtrToU", &CompilerGXP::unimplemented },
        { spv::Op::OpSatConvertSToU, "OpSatConvertSToU", &CompilerGXP::unimplemented },
        { spv::Op::OpSatConvertUToS, "OpSatConvertUToS", &CompilerGXP::unimplemented },
        { spv::Op::OpConvertUToPtr, "OpConvertUToPtr", &CompilerGXP::unimplemented },
        { spv::Op::OpPtrCastToGeneric, "OpPtrCastToGeneric", &CompilerGXP::unimplemented },
        { spv::Op::OpGenericCastToPtr, "OpGenericCastToPtr", &CompilerGXP::unimplemented },
        { spv::Op::OpGenericCastToPtrExplicit, "OpGenericCastToPtrExplicit", &CompilerGXP::unimplemented },
        { spv::Op::OpBitcast, "OpBitcast", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(125), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpSNegate, "OpSNegate", &CompilerGXP::unimplemented },
        { spv::Op::OpFNegate, "OpFNegate", &CompilerGXP::opFNegate },
        { spv::Op::OpIAdd, "OpIAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpFAdd, "OpFAdd", &CompilerGXP::opFAdd },
        { spv::Op::OpISub, "OpISub", &CompilerGXP::unimplemented },
        { spv::Op::OpFSub, "OpFSub", &CompilerGXP::opFSub },
        { spv::Op::OpIMul, "OpIMul", &CompilerGXP::unimplemented },
        { spv::Op::OpFMul, "OpFMul", &CompilerGXP::opFMul },
        { spv::Op::OpUDiv, "OpUDiv", &CompilerGXP::unimplemented },
        { spv::Op::OpSDiv, "OpSDiv", &CompilerGXP::unimplemented },
        { spv::Op::OpFDiv, "OpFDiv", &CompilerGXP::unimplemented },
        { spv::Op::OpUMod, "OpUMod", &CompilerGXP::unimplemented },
        { spv::Op::OpSRem, "OpSRem", &CompilerGXP::unimplemented },
        { spv::Op::OpSMod, "OpSMod", &CompilerGXP::unimplemented },
        { spv::Op::OpFRem, "OpFRem", &CompilerGXP::unimplemented },
        { spv::Op::OpFMod, "OpFMod", &CompilerGXP::unimplemented },
        { spv::Op::OpVectorTimesScalar, "OpVectorTimesScalar", &CompilerGXP::opVectorTimesScalar },
        { spv::Op::OpMatrixTimesScalar, "OpMatrixTimesScalar", &CompilerGXP::unimplemented },
        { spv::Op::OpVectorTimesMatrix, "OpVectorTimesMatrix", &CompilerGXP::unimplemented },
        { spv::Op::OpMatrixTimesVector, "OpMatrixTimesVector", &CompilerGXP::opMatrixTimesVector },
        { spv::Op::OpMatrixTimesMatrix, "OpMatrixTimesMatrix", &CompilerGXP::unimplemented },
        { spv::Op::OpOuterProduct, "OpOuterProduct", &CompilerGXP::unimplemented },
        { spv::Op::OpDot, "OpDot", &CompilerGXP::opDot },
        { spv::Op::OpIAddCarry, "OpIAddCarry", &CompilerGXP::unimplemented },
        { spv::Op::OpISubBorrow, "OpISubBorrow", &CompilerGXP::unimplemented },
        { spv::Op::OpUMulExtended, "OpUMulExtended", &CompilerGXP::unimplemented },
        { spv::Op::OpSMulExtended, "OpSMulExtended", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(153), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpAny, "OpAny", &CompilerGXP::unimplemented },
        { spv::Op::OpAll, "OpAll", &CompilerGXP::unimplemented },
        { spv::Op::OpIsNan, "OpIsNan", &CompilerGXP::unimplemented },
        { spv::Op::OpIsInf, "OpIsInf", &CompilerGXP::unimplemented },
        { spv::Op::OpIsFinite, "OpIsFinite", &CompilerGXP::unimplemented },
        { spv::Op::OpIsNormal, "OpIsNormal", &CompilerGXP::unimplemented },
        { spv::Op::OpSignBitSet, "OpSignBitSet", &CompilerGXP::unimplemented },
        { spv::Op::OpLessOrGreater, "OpLessOrGreater", &CompilerGXP::unimplemented },
        { spv::Op::OpOrdered, "OpOrdered", &CompilerGXP::unimplemented },
        { spv::Op::OpUnordered, "OpUnordered", &CompilerGXP::unimplemented },
        { spv::Op::OpLogicalEqual, "OpLogicalEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpLogicalNotEqual, "OpLogicalNotEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpLogicalOr, "OpLogicalOr", &CompilerGXP::unimplemented },
        { spv::Op::OpLogicalAnd, "OpLogicalAnd", &CompilerGXP::unimplemented },
        { spv::Op::OpLogicalNot, "OpLogicalNot", &CompilerGXP::unimplemented },
        { spv::Op::OpSelect, "OpSelect", &CompilerGXP::unimplemented },
        { spv::Op::OpIEqual, "OpIEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpINotEqual, "OpINotEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpUGreaterThan, "OpUGreaterThan", &CompilerGXP::unimplemented },
        { spv::Op::OpSGreaterThan, "OpSGreaterThan", &CompilerGXP::unimplemented },
        { spv::Op::OpUGreaterThanEqual, "OpUGreaterThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpSGreaterThanEqual, "OpSGreaterThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpULessThan, "OpULessThan", &CompilerGXP::unimplemented },
        { spv::Op::OpSLessThan, "OpSLessThan", &CompilerGXP::unimplemented },
        { spv::Op::OpULessThanEqual, "OpULessThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpSLessThanEqual, "OpSLessThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdEqual, "OpFOrdEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordEqual, "OpFUnordEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdNotEqual, "OpFOrdNotEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordNotEqual, "OpFUnordNotEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdLessThan, "OpFOrdLessThan", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordLessThan, "OpFUnordLessThan", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdGreaterThan, "OpFOrdGreaterThan", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordGreaterThan, "OpFUnordGreaterThan", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdLessThanEqual, "OpFOrdLessThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordLessThanEqual, "OpFUnordLessThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFOrdGreaterThanEqual, "OpFOrdGreaterThanEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpFUnordGreaterThanEqual, "OpFUnordGreaterThanEqual", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(192), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(193), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpShiftRightLogical, "OpShiftRightLogical", &CompilerGXP::unimplemented },
        { spv::Op::OpShiftRightArithmetic, "OpShiftRightArithmetic", &CompilerGXP::unimplemented },
        { spv::Op::OpShiftLeftLogical, "OpShiftLeftLogical", &CompilerGXP::unimplemented },
        { spv::Op::OpBitwiseOr, "OpBitwiseOr", &CompilerGXP::unimplemented },
        { spv::Op::OpBitwiseXor, "OpBitwiseXor", &CompilerGXP::unimplemented },
        { spv::Op::OpBitwiseAnd, "OpBitwiseAnd", &CompilerGXP::unimplemented },
        { spv::Op::OpNot, "OpNot", &CompilerGXP::unimplemented },
        { spv::Op::OpBitFieldInsert, "OpBitFieldInsert", &CompilerGXP::unimplemented },
        { spv::Op::OpBitFieldSExtract, "OpBitFieldSExtract", &CompilerGXP::unimplemented },
        { spv::Op::OpBitFieldUExtract, "OpBitFieldUExtract", &CompilerGXP::unimplemented },
        { spv::Op::OpBitReverse, "OpBitReverse", &CompilerGXP::unimplemented },
        { spv::Op::OpBitCount, "OpBitCount", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(206), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpDPdx, "OpDPdx", &CompilerGXP::unimplemented },
        { spv::Op::OpDPdy, "OpDPdy", &CompilerGXP::unimplemented },
        { spv::Op::OpFwidth, "OpFwidth", &CompilerGXP::unimplemented },
        { spv::Op::OpDPdxFine, "OpDPdxFine", &CompilerGXP::unimplemented },
        { spv::Op::OpDPdyFine, "OpDPdyFine", &CompilerGXP::unimplemented },
        { spv::Op::OpFwidthFine, "OpFwidthFine", &CompilerGXP::unimplemented },
        { spv::Op::OpDPdxCoarse, "OpDPdxCoarse", &CompilerGXP::unimplemented },
        { spv::Op::OpDPdyCoarse, "OpDPdyCoarse", &CompilerGXP::unimplemented },
        { spv::Op::OpFwidthCoarse, "OpFwidthCoarse", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(216), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(217), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpEmitVertex, "OpEmitVertex", &CompilerGXP::unimplemented },
        { spv::Op::OpEndPrimitive, "OpEndPrimitive", &CompilerGXP::unimplemented },
        { spv::Op::OpEmitStreamVertex, "OpEmitStreamVertex", &CompilerGXP::unimplemented },
        { spv::Op::OpEndStreamPrimitive, "OpEndStreamPrimitive", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(222), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(223), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpControlBarrier, "OpControlBarrier", &CompilerGXP::unimplemented },
        { spv::Op::OpMemoryBarrier, "OpMemoryBarrier", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(226), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpAtomicLoad, "OpAtomicLoad", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicStore, "OpAtomicStore", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicExchange, "OpAtomicExchange", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicCompareExchange, "OpAtomicCompareExchange", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicCompareExchangeWeak, "OpAtomicCompareExchangeWeak", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicIIncrement, "OpAtomicIIncrement", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicIDecrement, "OpAtomicIDecrement", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicIAdd, "OpAtomicIAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicISub, "OpAtomicISub", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicSMin, "OpAtomicSMin", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicUMin, "OpAtomicUMin", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicSMax, "OpAtomicSMax", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicUMax, "OpAtomicUMax", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicAnd, "OpAtomicAnd", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicOr, "OpAtomicOr", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicXor, "OpAtomicXor", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(243), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(244), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpPhi, "OpPhi", &CompilerGXP::unimplemented },
        { spv::Op::OpLoopMerge, "OpLoopMerge", &CompilerGXP::unimplemented },
        { spv::Op::OpSelectionMerge, "OpSelectionMerge", &CompilerGXP::unimplemented },
        { spv::Op::OpLabel, "OpLabel", &CompilerGXP::unimplemented },
        { spv::Op::OpBranch, "OpBranch", &CompilerGXP::unimplemented },
        { spv::Op::OpBranchConditional, "OpBranchConditional", &CompilerGXP::unimplemented },
        { spv::Op::OpSwitch, "OpSwitch", &CompilerGXP::unimplemented },
        { spv::Op::OpKill, "OpKill", &CompilerGXP::unimplemented },
        { spv::Op::OpReturn, "OpReturn", &CompilerGXP::unimplemented },
        { spv::Op::OpReturnValue, "OpReturnValue", &CompilerGXP::unimplemented },
        { spv::Op::OpUnreachable, "OpUnreachable", &CompilerGXP::unimplemented },
        { spv::Op::OpLifetimeStart, "OpLifetimeStart", &CompilerGXP::unimplemented },
        { spv::Op::OpLifetimeStop, "OpLifetimeStop", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(258), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpGroupAsyncCopy, "OpGroupAsyncCopy", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupWaitEvents, "OpGroupWaitEvents", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupAll, "OpGroupAll", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupAny, "OpGroupAny", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupBroadcast, "OpGroupBroadcast", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupIAdd, "OpGroupIAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupFAdd, "OpGroupFAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupFMin, "OpGroupFMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupUMin, "OpGroupUMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupSMin, "OpGroupSMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupFMax, "OpGroupFMax", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupUMax, "OpGroupUMax", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupSMax, "OpGroupSMax", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(272), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(273), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpReadPipe, "OpReadPipe", &CompilerGXP::unimplemented },
        { spv::Op::OpWritePipe, "OpWritePipe", &CompilerGXP::unimplemented },
        { spv::Op::OpReservedReadPipe, "OpReservedReadPipe", &CompilerGXP::unimplemented },
        { spv::Op::OpReservedWritePipe, "OpReservedWritePipe", &CompilerGXP::unimplemented },
        { spv::Op::OpReserveReadPipePackets, "OpReserveReadPipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpReserveWritePipePackets, "OpReserveWritePipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpCommitReadPipe, "OpCommitReadPipe", &CompilerGXP::unimplemented },
        { spv::Op::OpCommitWritePipe, "OpCommitWritePipe", &CompilerGXP::unimplemented },
        { spv::Op::OpIsValidReserveId, "OpIsValidReserveId", &CompilerGXP::unimplemented },
        { spv::Op::OpGetNumPipePackets, "OpGetNumPipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpGetMaxPipePackets, "OpGetMaxPipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupReserveReadPipePackets, "OpGroupReserveReadPipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupReserveWritePipePackets, "OpGroupReserveWritePipePackets", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupCommitReadPipe, "OpGroupCommitReadPipe", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupCommitWritePipe, "OpGroupCommitWritePipe", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(289), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(290), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpEnqueueMarker, "OpEnqueueMarker", &CompilerGXP::unimplemented },
        { spv::Op::OpEnqueueKernel, "OpEnqueueKernel", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelNDrangeSubGroupCount, "OpGetKernelNDrangeSubGroupCount", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelNDrangeMaxSubGroupSize, "OpGetKernelNDrangeMaxSubGroupSize", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelWorkGroupSize, "OpGetKernelWorkGroupSize", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelPreferredWorkGroupSizeMultiple, "OpGetKernelPreferredWorkGroupSizeMultiple", &CompilerGXP::unimplemented },
        { spv::Op::OpRetainEvent, "OpRetainEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpReleaseEvent, "OpReleaseEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpCreateUserEvent, "OpCreateUserEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpIsValidEvent, "OpIsValidEvent", &CompilerGXP::unimplemented },
        { spv::Op::OpSetUserEventStatus, "OpSetUserEventStatus", &CompilerGXP::unimplemented },
        { spv::Op::OpCaptureEventProfilingInfo, "OpCaptureEventProfilingInfo", &CompilerGXP::unimplemented },
        { spv::Op::OpGetDefaultQueue, "OpGetDefaultQueue", &CompilerGXP::unimplemented },
        { spv::Op::OpBuildNDRange, "OpBuildNDRange", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleImplicitLod, "OpImageSparseSampleImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleExplicitLod, "OpImageSparseSampleExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleDrefImplicitLod, "OpImageSparseSampleDrefImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleDrefExplicitLod, "OpImageSparseSampleDrefExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleProjImplicitLod, "OpImageSparseSampleProjImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleProjExplicitLod, "OpImageSparseSampleProjExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleProjDrefImplicitLod, "OpImageSparseSampleProjDrefImplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseSampleProjDrefExplicitLod, "OpImageSparseSampleProjDrefExplicitLod", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseFetch, "OpImageSparseFetch", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseGather, "OpImageSparseGather", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseDrefGather, "OpImageSparseDrefGather", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseTexelsResident, "OpImageSparseTexelsResident", &CompilerGXP::unimplemented },
        { spv::Op::OpNoLine, "OpNoLine", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicFlagTestAndSet, "OpAtomicFlagTestAndSet", &CompilerGXP::unimplemented },
        { spv::Op::OpAtomicFlagClear, "OpAtomicFlagClear", &CompilerGXP::unimplemented },
        { spv::Op::OpImageSparseRead, "OpImageSparseRead", &CompilerGXP::unimplemented },
        { spv::Op::OpSizeOf, "OpSizeOf", &CompilerGXP::unimplemented },
        { spv::Op::OpTypePipeStorage, "OpTypePipeStorage", &CompilerGXP::unimplemented },
        { spv::Op::OpConstantPipeStorage, "OpConstantPipeStorage", &CompilerGXP::unimplemented },
        { spv::Op::OpCreatePipeFromPipeStorage, "OpCreatePipeFromPipeStorage", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelLocalSizeForSubgroupCount, "OpGetKernelLocalSizeForSubgroupCount", &CompilerGXP::unimplemented },
        { spv::Op::OpGetKernelMaxNumSubgroups, "OpGetKernelMaxNumSubgroups", &CompilerGXP::unimplemented },
        { spv::Op::OpTypeNamedBarrier, "OpTypeNamedBarrier", &CompilerGXP::unimplemented },
        { spv::Op::OpNamedBarrierInitialize, "OpNamedBarrierInitialize", &CompilerGXP::unimplemented },
        { spv::Op::OpMemoryNamedBarrier, "OpMemoryNamedBarrier", &CompilerGXP::unimplemented },
        { spv::Op::OpModuleProcessed, "OpModuleProcessed", &CompilerGXP::unimplemented },
        { spv::Op::OpExecutionModeId, "OpExecutionModeId", &CompilerGXP::unimplemented },
        { spv::Op::OpDecorateId, "OpDecorateId", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformElect, "OpGroupNonUniformElect", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformAll, "OpGroupNonUniformAll", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformAny, "OpGroupNonUniformAny", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformAllEqual, "OpGroupNonUniformAllEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBroadcast, "OpGroupNonUniformBroadcast", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBroadcastFirst, "OpGroupNonUniformBroadcastFirst", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBallot, "OpGroupNonUniformBallot", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformInverseBallot, "OpGroupNonUniformInverseBallot", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBallotBitExtract, "OpGroupNonUniformBallotBitExtract", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBallotBitCount, "OpGroupNonUniformBallotBitCount", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBallotFindLSB, "OpGroupNonUniformBallotFindLSB", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBallotFindMSB, "OpGroupNonUniformBallotFindMSB", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformShuffle, "OpGroupNonUniformShuffle", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformShuffleXor, "OpGroupNonUniformShuffleXor", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformShuffleUp, "OpGroupNonUniformShuffleUp", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformShuffleDown, "OpGroupNonUniformShuffleDown", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformIAdd, "OpGroupNonUniformIAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformFAdd, "OpGroupNonUniformFAdd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformIMul, "OpGroupNonUniformIMul", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformFMul, "OpGroupNonUniformFMul", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformSMin, "OpGroupNonUniformSMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformUMin, "OpGroupNonUniformUMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformFMin, "OpGroupNonUniformFMin", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformSMax, "OpGroupNonUniformSMax", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformUMax, "OpGroupNonUniformUMax", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformFMax, "OpGroupNonUniformFMax", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBitwiseAnd, "OpGroupNonUniformBitwiseAnd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBitwiseOr, "OpGroupNonUniformBitwiseOr", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformBitwiseXor, "OpGroupNonUniformBitwiseXor", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformLogicalAnd, "OpGroupNonUniformLogicalAnd", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformLogicalOr, "OpGroupNonUniformLogicalOr", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformLogicalXor, "OpGroupNonUniformLogicalXor", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformQuadBroadcast, "OpGroupNonUniformQuadBroadcast", &CompilerGXP::unimplemented },
        { spv::Op::OpGroupNonUniformQuadSwap, "OpGroupNonUniformQuadSwap", &CompilerGXP::unimplemented },
        { static_cast<spv::Op>(367), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(368), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(369), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(370), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(371), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(372), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(373), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(374), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(375), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(376), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(377), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(378), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(379), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(380), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(381), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(382), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(383), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(384), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(385), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(386), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(387), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(388), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(389), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(390), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(391), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(392), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(393), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(394), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(395), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(396), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(397), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(398), "OpUndefined", &CompilerGXP::undefined },
        { static_cast<spv::Op>(399), "OpUndefined", &CompilerGXP::undefined },
        { spv::Op::OpCopyLogical, "OpCopyLogical", &CompilerGXP::unimplemented },
        { spv::Op::OpPtrEqual, "OpPtrEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpPtrNotEqual, "OpPtrNotEqual", &CompilerGXP::unimplemented },
        { spv::Op::OpPtrDiff, "OpPtrDiff", &CompilerGXP::unimplemented },
    };
}

void CompilerGXP::createExtensions() {
    extensions = {
        {
            SPIRExtension::GLSL,
            {
                { GLSLstd450Normalize, &CompilerGXP::extGLSLNormalize },
                { GLSLstd450FMin, &CompilerGXP::extGLSLFMin },
                { GLSLstd450FMax, &CompilerGXP::extGLSLFMax },
                { GLSLstd450Reflect, &CompilerGXP::extGLSLReflect },
                { GLSLstd450Pow, &CompilerGXP::extGLSLPow }
            }
        }
    };
}
