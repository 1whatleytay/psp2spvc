#include <gxp/builder.h>

#include <gxp/usse.h>
#include <gxp/block.h>
#include <gxp/instructions.h>

#include <util/util.h>

#include <fmt/format.h>

#define OFFSET_OF(parent, child) (reinterpret_cast<uintptr_t>(&parent.child) - reinterpret_cast<uintptr_t>(&parent))

namespace gxp {
    void Builder::setType(gxp::ShaderType type) {
        header.type = static_cast<uint8_t>(type);
    }

    ShaderType Builder::getType() {
        return static_cast<ShaderType>(header.type);
    }

    usse::RegisterBank Parameter::getBank() {
        switch (category) {
        case ParameterCategory::Attribute:
            return usse::RegisterBank::Primary;
        case ParameterCategory::Uniform:
            return usse::RegisterBank::Secondary;
        default:
            return usse::RegisterBank::Invalid;
        }
    }

    Block *Builder::createPrimaryBlock() {
        size_t index = primaryBlocks.size();
        primaryBlocks.push_back(std::unique_ptr<Block>(new Block(*this)));

        return primaryBlocks[index].get();
    }

    Block *Builder::createSecondaryBlock() {
        size_t index = secondaryBlocks.size();
        secondaryBlocks.push_back(std::unique_ptr<Block>(new Block(*this)));

        return secondaryBlocks[index].get();
    }

    // Odd index can only reference vec3 (.yzw?)
    // For vec4, index must be even.
    static bool needsAllocOffset(uint32_t index, uint32_t size) {
        return size == 4 && index % 2 == 1;
    }

    usse::RegisterReference Builder::allocateRegister(usse::RegisterBank bank, usse::DataType type) {
        uint32_t index = 0;
        uint32_t size = usse::getTypeSize(type.type) * type.components * type.arraySize / 4;

        switch (bank) {
        case usse::RegisterBank::Primary:
            index = paRegPointer + needsAllocOffset(paRegPointer, size);
            paRegPointer += size + needsAllocOffset(paRegPointer, size);
            break;
        case usse::RegisterBank::Secondary:
            index = saRegPointer + needsAllocOffset(saRegPointer, size);
            saRegPointer += size + needsAllocOffset(saRegPointer, size);
            break;
        case usse::RegisterBank::Output:
            index = oRegPointer + needsAllocOffset(oRegPointer, size);
            oRegPointer += size + needsAllocOffset(oRegPointer, size);
            break;
        case usse::RegisterBank::Temporary:
            index = allocate(tRegSpace.data(), tRegSpace.size(), size, size == 4 ? 2 : 1);
            if (index == ~0u)
                throw std::runtime_error(fmt::format("Cannot allocate space for temporary register size {}.", size));
            memset(&tRegSpace[index], true, sizeof(bool) * size);
            if (tMaxRegs < index + size)
                tMaxRegs = index + size;
            break;
        case usse::RegisterBank::Internal:
            // 1 internal register = 1 vec4
            index = iRegPointer++;
            assert(iRegPointer <= 4);
            break;
        default:
            throw std::runtime_error("Missing allocation method for bank.");
        }

        if (config.printAllocations) {
            fmt::print("[alloc] {} registers of type {} (vec{}[{}]), size {} at index {}.\n",
                usse::getBankName(bank), usse::getTypeName(type.type), type.components, type.arraySize, size, index);
        }

        return usse::RegisterReference(type, bank, index);
    }

    void Builder::freeRegister(usse::RegisterReference reg) {
        switch (reg.bank) {
        case usse::RegisterBank::Internal:
            if (reg.index + 1 == iRegPointer)
                iRegPointer--;
            else
                throw std::runtime_error("Latest internal register must be freed first.");
            break;
        case usse::RegisterBank::Temporary:
            memset(&tRegSpace[reg.index], false, sizeof(bool) * reg.size);
            break;
        default:
            assert(false);
            break;
        }
    }

    usse::RegisterReference Builder::registerParameter(const Parameter &parameter) {
        size_t index = parameters.size();
        parameters.push_back(parameter);

        usse::RegisterReference reg = allocateRegister(parameters[index].getBank(), parameter.type);
        parameters[index].resourceIndex = reg.index;
        parameters[index].containerIndex = 0;

        return reg;
    }

    usse::RegisterReference Builder::registerLiteral(const std::vector<float> &literal) {
        assert(!literal.empty() && literal.size() <= 4);

        usse::RegisterReference reg({ usse::Type::Float32, static_cast<uint32_t>(literal.size()), 1 },
            usse::RegisterBank::Secondary, literals.size());

        for (float a : literal)
            literals.push_back(a);

        return reg;
    }

    std::map<ProgramVarying, usse::RegisterReference> Builder::registerVertexVaryings(
        const std::vector<ProgramVarying> &outputs, const std::vector<ProgramVectorInfo> &texCoords) {
        varyings.varyings_count = outputs.size() + texCoords.size();

        varyings.vertex_outputs1 = 0;
        varyings.vertex_outputs2 = 0;

        for (ProgramVarying output : outputs) {
            if (output == ProgramVarying::Position) continue;
            if (isTexCoordVarying(output))
                throw std::runtime_error("TexCoord passed as regular output to createVaryings.");

            varyings.vertex_outputs1 |= getVertexVaryingBits(output);
        }

        for (ProgramVectorInfo texCoord : texCoords) {
            if (!isTexCoordVarying(texCoord.varying))
                throw std::runtime_error("Non-TexCoord passed as TexCoord to createVaryings.");

            uint32_t texCoordIndex = static_cast<uint32_t>(texCoord.varying)
                - static_cast<uint32_t>(ProgramVarying::TexCoord0);

            uint32_t texCoordBits = 0;
            assert(texCoord.components > 1);
            texCoordBits |= (0b111u << (texCoord.components - 1)) >> 3u;

            varyings.vertex_outputs2 |= texCoordBits << (texCoordIndex * 3u);
        }

        std::map<ProgramVarying, usse::RegisterReference> references;

        for (auto a = static_cast<uint32_t>(ProgramVarying::Position);
            a < static_cast<uint32_t>(ProgramVarying::TexCoord0); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varying == ProgramVarying::Position || varyings.vertex_outputs1 & getVertexVaryingBits(varying)) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, 4, 1 });
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::TexCoord0);
            a <= static_cast<uint32_t>(ProgramVarying::TexCoord9); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            auto texCoordInfo = std::find_if(texCoords.begin(), texCoords.end(),
                [varying](const ProgramVectorInfo &info) { return info.varying == varying; });

            if (texCoordInfo != texCoords.end()) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    {usse::Type::Float32, texCoordInfo->components, 1 });
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::PointSize);
             a <= static_cast<uint32_t>(ProgramVarying::Clip7); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varyings.vertex_outputs1 & getVertexVaryingBits(varying)) {
                uint32_t varyingSize = varying == ProgramVarying::PointSize ? 1 : 4;

                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, varyingSize, 1 });
            }
        }

        return references;
    }

    std::map<ProgramVarying, usse::RegisterReference> Builder::registerFragmentVaryings(
        const std::vector<ProgramVectorInfo> &inputs) {
        std::map<ProgramVarying, usse::RegisterReference> references;

        for (ProgramVectorInfo varying : inputs) {
            usse::DataType type = {usse::Type::Float32, varying.components, 1 };
            usse::RegisterReference reference = allocateRegister(usse::RegisterBank::Primary, type);

            // What the heck is going on with fragment inputs!?!?
            ProgramFragmentInputInfo input;
            input.size = (reference.size - 1) << 4u;
            input.componentInfo = 0b11u << 4u; // 0b11 = Float, 0b10 = Half?
            input.resourceIndex = reference.index;

            input.attributeInfo |= getFragmentVaryingBits(varying.varying); // Id
            input.attributeInfo |= 0x10A000u; // 0x20000000 = Half, 0x10000000 = Fixed, 0x10A000 = Float...
            input.attributeInfo |= (reference.type.components - 1) << 22u; // Component Count
            input.attributeInfo |= 0xFu; // Not a Sampler!

            // Samplers are not yet supported.

            fragmentInputs.push_back(input);

            references[varying.varying] = reference;
        }

        return references;
    }

    usse::RegisterReference Builder::createFragmentOutput(usse::Type type, uint32_t components) {
        varyings.output_comp_count = components;
        varyings.output_param_type = static_cast<uint8_t>(getParameterTypeFromUSSEType(type));

        return usse::RegisterReference({ type, components, 1 }, usse::RegisterBank::Primary, 0);
    }

    std::vector<uint8_t> Builder::build() {
        std::vector<uint8_t> data(sizeof(ProgramHeader) + sizeof(ProgramVaryings));
        header.varyingsOffset = sizeof(ProgramHeader) - OFFSET_OF(header, varyingsOffset);

        // Strings
        class StringEntry {
        public:
            size_t index = 0;
            std::string text;
        };

        std::vector<StringEntry> stringDB;

        for (const Parameter &param : parameters) {
            StringEntry entry;
            entry.index = data.size();
            entry.text = param.name;
            data.insert(data.end(), entry.text.begin(), entry.text.end());
            data.insert(data.end(), '\0');
            stringDB.push_back(entry);
        }

        std::vector<ProgramContainerInfo> containers;

        // Default Container
        containers.push_back({
            static_cast<uint16_t>(ContainerIndex::Default), // Container Index
            0, // ??
            0, // Register Index
            static_cast<uint16_t>(saRegPointer) // Register Count
        });

        // Literals
        if (!literals.empty()) {
            containers.push_back({
                static_cast<uint16_t>(ContainerIndex::Literal),
                0, // ??
                static_cast<uint16_t>(saRegPointer), // Register Index
                static_cast<uint16_t>(literals.size())
            });

            header.literalsCount = literals.size();
            header.literalsOffset = data.size() - OFFSET_OF(header, literalsOffset);
            for (uint32_t a = 0; a < literals.size(); a++) {
                float literal = literals[a];

                data.insert(data.end(),
                    reinterpret_cast<uint8_t *>(&a),
                    reinterpret_cast<uint8_t *>(&a) + sizeof(uint32_t));
                data.insert(data.end(),
                    reinterpret_cast<uint8_t *>(&literal),
                    reinterpret_cast<uint8_t *>(&literal) + sizeof(float));
            }
        }

        // Containers
        header.containerCount = containers.size();
        header.containerOffset = data.size() - OFFSET_OF(header, containerOffset);
        {
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(containers.data()),
                reinterpret_cast<uint8_t *>(containers.data())
                + containers.size() * sizeof(ProgramContainerInfo));
        }

        // Parameters
        header.parameterCount = parameters.size();
        header.parametersOffset = data.size() - OFFSET_OF(header, parametersOffset);
        for (const Parameter &param : parameters) {
            ProgramParameterInfo parameter;
            parameter.resourceIndex = param.resourceIndex;
            parameter.arraySize = param.type.arraySize;
            parameter.semantic = static_cast<uint16_t>(param.semantic);
            parameter.config = createParameterConfig(param.category, getParameterTypeFromUSSEType(param.type.type),
                param.type.components, ContainerIndex::Default);

            auto stringEntry = std::find_if(stringDB.begin(), stringDB.end(), [param](const StringEntry &entry) {
                return entry.text == param.name;
            });
            if (stringEntry == stringDB.end())
                throw std::runtime_error(fmt::format("String \"{}\" is missing from gxp db.", param.name));

            parameter.nameOffset = stringEntry->index - (data.size() + OFFSET_OF(parameter, nameOffset));

            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&parameter),
                reinterpret_cast<uint8_t *>(&parameter)
                + sizeof(parameter));
        }

        // Varyings
        if (getType() == ShaderType::Fragment && !fragmentInputs.empty()) {
            varyings.varyings_count = fragmentInputs.size();
            varyings.vertex_outputs1 = data.size()
                - (sizeof(ProgramHeader) + OFFSET_OF(varyings, varyings_count) + sizeof(uint32_t));
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(fragmentInputs.data()),
                reinterpret_cast<uint8_t *>(fragmentInputs.data())
                + fragmentInputs.size() * sizeof(ProgramFragmentInputInfo));
        }

        // Code
        header.primaryRegCount = paRegPointer;
        header.secondaryRegCount = saRegPointer + literals.size();
        header.tempRegCount1 = tMaxRegs;
        header.tempRegCount2 = tMaxRegs; // Difference between both reg counts?
        {
            header.secondaryProgramOffset = data.size() - OFFSET_OF(header, secondaryProgramOffset);
            for (const std::unique_ptr<Block> &block : secondaryBlocks) {
                data.insert(data.end(),
                    reinterpret_cast<const uint8_t *>(block->instructions.data()),
                    reinterpret_cast<const uint8_t *>(block->instructions.data())
                    + block->instructions.size() * sizeof(usse::Instruction));
            }
            header.secondaryProgramOffsetEnd = data.size() - OFFSET_OF(header, secondaryProgramOffsetEnd);

            header.primaryProgramOffset = data.size() - OFFSET_OF(header, primaryProgramOffset);
            usse::Instruction phase = usse::makePHAS(
                0, // sprvv
                true, // end
                true, // imm
                0, // src1_bank_ext
                0, // src2_bank_ext
                0, // mode
                0, // rate_hi
                0, // rate_lo_or_nosched
                0, // wait_cond
                0, // temp_count
                0, // src1_bank
                0, // src2_bank
                0, // exe_addr_high
                0, // src1_n_or_exe_addr_mid
                0 // src2_n_or_exe_addr_low
            );
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&phase),
                reinterpret_cast<uint8_t *>(&phase) + sizeof(phase));

            header.primaryProgramInstructionCount++;
            for (const std::unique_ptr<Block> &block : primaryBlocks) {
                header.primaryProgramInstructionCount += block->instructions.size();
                data.insert(data.end(),
                            reinterpret_cast<const uint8_t *>(block->instructions.data()),
                            reinterpret_cast<const uint8_t *>(block->instructions.data())
                            + block->instructions.size() * sizeof(usse::Instruction));
            }

            usse::BankLayout emitLayout = usse::BankLayout::srcLayout(usse::RegisterBank::Immediate);

            usse::Instruction emit = usse::makeEMIT(
                0, // sideband_high
                0, // src0_bank_ext
                true, // end
                0, // src1_bank_ext
                emitLayout.extension, // src2_bank_ext
                1, /* Target = MTE */ // target
                0, // task_start_or_mte_hi
                1, /* MTE = Vertex */ // task_end_or_mte_lo
                0, // nosched
                0, // sideband_mid
                0, // src0_bank
                0, // incp
                0, // src1_bank
                emitLayout.number, // src2_bank
                0, // sideband_low
                true, // freep
                0, // src0_n
                0, // src1_n
                0 // src2_n
            );
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&emit),
                reinterpret_cast<uint8_t *>(&emit) + sizeof(emit));
        }

        std::memcpy(data.data(), &header, sizeof(ProgramHeader));
        std::memcpy(data.data() + sizeof(ProgramHeader), &varyings, sizeof(ProgramVaryings));

        return data;
    }

    Builder::Builder() : Builder(BuilderConfig { }) { }
    Builder::Builder(BuilderConfig config) : config(config) {
        header.magic = gxpMagic;
        header.majorVersion = 1;
        header.minorVersion = 4;
    }
}
