#include <translator/translator.h>

#include <gxp/builder.h>

#include <fmt/format.h>

#define VERTEX_POSITION_OUTPUT_ONLY

// Position, PointSize, Clip0 and Clip1 are removed so they are not allocated over.
const std::vector<gxp::ProgramVarying> allVaryings = {
    gxp::ProgramVarying::Color0,
    gxp::ProgramVarying::Color1,
// Afaik Clips don't work as a varying to fragment shader.
//    gxp::ProgramVarying::Clip0,
//    gxp::ProgramVarying::Clip1,
//    gxp::ProgramVarying::Clip2,
//    gxp::ProgramVarying::Clip3,
//    gxp::ProgramVarying::Clip4,
//    gxp::ProgramVarying::Clip5,
//    gxp::ProgramVarying::Clip6,
//    gxp::ProgramVarying::Clip7,
    gxp::ProgramVarying::Fog,
};

const std::vector<gxp::ProgramVarying> allTexCoords = {
    gxp::ProgramVarying::TexCoord0,
    gxp::ProgramVarying::TexCoord1,
    gxp::ProgramVarying::TexCoord2,
    gxp::ProgramVarying::TexCoord3,
    gxp::ProgramVarying::TexCoord4,
    gxp::ProgramVarying::TexCoord5,
    gxp::ProgramVarying::TexCoord6,
    gxp::ProgramVarying::TexCoord7,
    gxp::ProgramVarying::TexCoord8,
    gxp::ProgramVarying::TexCoord9,
};

usse::Type CompilerGXP::translateType(SPIRType::BaseType baseType) {
    switch (baseType) {
    case SPIRType::BaseType::Boolean:
        return usse::Type::Unsigned8;
    case SPIRType::BaseType::SByte:
        return usse::Type::Signed8;
    case SPIRType::BaseType::UByte:
        return usse::Type::Unsigned8;
    case SPIRType::BaseType::Short:
        return usse::Type::Signed16;
    case SPIRType::BaseType::UShort:
        return usse::Type::Unsigned16;
    case SPIRType::BaseType::Int:
        return usse::Type::Signed32;
    case SPIRType::BaseType::UInt:
        return usse::Type::Unsigned32;
    case SPIRType::BaseType::Half:
        return usse::Type::Float16;
    case SPIRType::BaseType::Float:
        return usse::Type::Float32;
    default:
        throw std::runtime_error(fmt::format("Unknown base type {}.", baseType));
    }
}

usse::DataType CompilerGXP::translateType(const SPIRType &type) {
    usse::DataType dataType = { };

    dataType.type = translateType(type.basetype);
    dataType.components = type.vecsize;
    dataType.arraySize = type.columns;

    return dataType;
}

gxp::ParameterSemantic CompilerGXP::translateDecorations(spv::BuiltIn builtIn) {
    switch (builtIn) {
    case spv::BuiltInPosition:
        return gxp::ParameterSemantic::Position;
    case spv::BuiltInPointSize:
        return gxp::ParameterSemantic::PointSize;
    default:
        return gxp::ParameterSemantic::None;
    }
}

gxp::ProgramVarying CompilerGXP::translateVarying(spv::BuiltIn builtIn) {
    switch (builtIn) {
    case spv::BuiltInPosition:
        return gxp::ProgramVarying::Position;
#ifndef VERTEX_POSITION_OUTPUT_ONLY
    case spv::BuiltInPointSize:
        return gxp::ProgramVarying::PointSize;
    case spv::BuiltInClipDistance:
        return gxp::ProgramVarying::Clip0;
    case spv::BuiltInCullDistance:
        return gxp::ProgramVarying::Clip1;
#endif
    default:
        return gxp::ProgramVarying::None;
    }
}

gxp::ProgramVarying CompilerGXP::allocateVarying(
    std::vector<gxp::ProgramVarying> &availableVaryings,
    std::vector<gxp::ProgramVarying> &availableTexCoords,
    uint32_t components) {
    gxp::ProgramVarying selected = gxp::ProgramVarying::None;

    if (components == 4 && !availableVaryings.empty()) {
        selected = availableVaryings[0];
        availableVaryings.erase(availableVaryings.begin());
    } else if (!availableTexCoords.empty()) {
        selected = availableTexCoords[0];
        availableTexCoords.erase(availableTexCoords.begin());
    }

    return selected;
}

void CompilerGXP::createBlock(const SPIRBlock &block) {
    gxp::Block *gxpBlock = builder.createPrimaryBlock();

    for (Instruction instruction : block.ops) {
        const auto &code = codes[instruction.op];

        const TranslatorArguments arguments(
            *gxpBlock,
            code,
            &ir.spirv[instruction.offset],
            instruction.count
            );

        (this->*code.implementation)(arguments);
    }
}

void CompilerGXP::createFunction(const SPIRFunction &function) {
    for (uint32_t blockId : function.blocks) {
        auto &block = get<SPIRBlock>(blockId);
        createBlock(block);
    }
}

void CompilerGXP::createVertexShaderResources() {
    ShaderResources resources = get_shader_resources();

    for (const auto &input : resources.stage_inputs) {
        const SPIRType &type = get_type(input.type_id);

        gxp::Parameter parameter;
        parameter.name = input.name;
        parameter.category = gxp::ParameterCategory::Attribute;
        parameter.type = translateType(type);
        idRegisters[input.id] = builder.registerParameter(parameter);
    }

    for (const auto &uniform : resources.uniform_buffers) {
        const SPIRType &type = get_type(uniform.type_id);

        if (type.member_types.size() != 1)
            throw std::runtime_error("Uniform blocks are not supported.");

        gxp::Parameter parameter;
        parameter.name = uniform.name;
        parameter.category = gxp::ParameterCategory::Uniform;
        parameter.type = translateType(get_type(type.member_types[0]));
        idRegisters[uniform.id] = builder.registerParameter(parameter);
    }

    std::vector<gxp::ProgramVarying> varyings;
    std::vector<gxp::ProgramVectorInfo> texCoords;

    std::vector<gxp::ProgramVarying> availableVaryings = allVaryings;
    std::vector<gxp::ProgramVarying> availableTexCoords = allTexCoords;

    for (const auto &output : resources.stage_outputs) {
        const SPIRType &type = get_type(output.type_id);

        gxp::ProgramVarying varying = allocateVarying(availableVaryings, availableTexCoords, type.vecsize);
        if (varying == gxp::ProgramVarying::None)
            throw std::runtime_error("No available space for varying.");

        if (gxp::isTexCoordVarying(varying)) {
            gxp::ProgramVectorInfo info = { };
            info.varying = varying;
            info.components = type.vecsize;
            texCoords.push_back(info);
        } else
            varyings.push_back(varying);

        idVaryings[output.id] = varying;
    }

    // TODO: Better solution that works with more layouts.
    auto variables = get_active_interface_variables();
    for (const auto &variableId : variables) {
        const SPIRVariable &variable = get<SPIRVariable>(variableId);
        const SPIRType &type = get_type(variable.basetype);

        if (variable.storage != spv::StorageClassOutput) continue;

        if (type.basetype == SPIRType::Struct) {
            // TODO: struct member should be able to identify itself via idVaryings? OpAccessChain might be it.
            for (size_t a = 0; a < type.member_types.size(); a++) {
                uint32_t id = type.member_types[a];
                SPIRType memberType = get_type(id);

                spv::BuiltIn builtIn;
                if (is_member_builtin(type, a, &builtIn)) {
                    gxp::ProgramVarying varying = translateVarying(builtIn);

                    if (varying != gxp::ProgramVarying::None)
                        varyings.push_back(varying);
                }
            }
        } else {
            gxp::ProgramVarying varying = translateVarying(ir.meta[variableId].decoration.builtin_type);

            if (varying != gxp::ProgramVarying::None) {
                varyings.push_back(varying);
                idVaryings[variableId] = varying;
            }
        }
    }

    varyingReferences = builder.registerVertexVaryings(varyings, texCoords);
}

void CompilerGXP::createFragmentShaderResources() {
    ShaderResources resources = get_shader_resources();

    if (resources.stage_outputs.size() == 1) {
        Resource resource = resources.stage_outputs[0];
        SPIRType type = get_type(resource.type_id);

        idRegisters[resource.id] = builder.createFragmentOutput(translateType(type.basetype), type.vecsize);
    } else {
        throw std::runtime_error("Only one output is allowed for a fragment shader.");
    }

    for (const auto &uniform : resources.uniform_buffers) {
        const SPIRType &type = get_type(uniform.type_id);

        if (type.member_types.size() != 1)
            throw std::runtime_error("Uniform blocks are not supported.");

        gxp::Parameter parameter;
        parameter.name = uniform.name;
        parameter.category = gxp::ParameterCategory::Uniform;
        parameter.type = translateType(get_type(type.member_types[0]));
        idRegisters[uniform.id] = builder.registerParameter(parameter);
    }

    std::vector<gxp::ProgramVectorInfo> varyings;

    std::vector<gxp::ProgramVarying> availableVaryings = allVaryings;
    std::vector<gxp::ProgramVarying> availableTexCoords = allTexCoords;

    for (const auto &input : resources.stage_inputs) {
        const SPIRType &type = get_type(input.type_id);

        gxp::ProgramVarying varying = allocateVarying(availableVaryings, availableTexCoords, type.vecsize);
        if (varying == gxp::ProgramVarying::None)
            throw std::runtime_error("No available space for varying.");

        varyings.push_back({ varying, type.vecsize });

        idVaryings[input.id] = varying;
    }

    varyingReferences = builder.registerFragmentVaryings(varyings);
}

std::vector<uint8_t> CompilerGXP::compileData() {
    auto entryPoints = get_entry_points_and_stages();
    if (entryPoints.size() != 1)
        throw std::runtime_error("Number of entry points must be 1.");
    switch (entryPoints[0].execution_model) {
        case spv::ExecutionModelVertex:
            builder.setType(gxp::ShaderType::Vertex);
            createVertexShaderResources();
            break;
        case spv::ExecutionModelFragment:
            builder.setType(gxp::ShaderType::Fragment);
            createFragmentShaderResources();
            break;
        default:
            throw std::runtime_error("Entry point must be of type vertex or fragment.");
    }

    SPIREntryPoint entryPoint = get_entry_point(entryPoints[0].name, entryPoints[0].execution_model);
    SPIRFunction entryFunction = get<SPIRFunction>(entryPoint.self);

    // addFunction should recursively call the other functions.
//    try {
        createFunction(entryFunction);
//    } catch (std::runtime_error &e) {
//        fmt::print("{}\n", e.what());
//    }

    return builder.build();
}

CompilerGXP::CompilerGXP(const std::vector<uint32_t> &data) : Compiler(data) {
    createTranslators();
}
