#pragma once

#include <util/spirv.h>
#include <gxp/builder.h>

#include <unordered_map>

namespace gxp { class Block; }
class CompilerGXP;
class TranslatorArguments;

typedef void(CompilerGXP::*TranslatorImplementation)(const TranslatorArguments &arguments);

class TranslatorCode {
public:
    spv::Op op;
    std::string name;
    TranslatorImplementation implementation;
};

class TranslatorArguments {
public:
    gxp::Block &block;
    const TranslatorCode &code;
    const uint32_t *instruction;
    uint32_t wordCount;

    TranslatorArguments(
        gxp::Block &block,
        const TranslatorCode &code,
        const uint32_t *instruction,
        uint32_t wordCount);
};

class CompilerGXP : public Compiler {
    gxp::Builder builder;

    std::vector<TranslatorCode> codes;
    std::unordered_map<spv::Id, gxp::ProgramVarying> idVaryings;
    std::unordered_map<spv::Id, usse::RegisterReference> idRegisters;
    std::unordered_map<gxp::ProgramVarying, usse::RegisterReference> varyingReferences;

    static usse::Type translateType(SPIRType::BaseType baseType);
    static usse::DataType translateType(const SPIRType &type);
    static gxp::ParameterSemantic translateDecorations(spv::BuiltIn builtIn);
    static gxp::ProgramVarying translateVarying(spv::BuiltIn builtIn);

    void createBlock(const SPIRBlock &block);
    void createFunction(const SPIRFunction &function);
    void createShaderResources();
    void createTranslators();

    // SPRIV Translation OPs
    void unimplemented(const TranslatorArguments &arguments);
    void undefined(const TranslatorArguments &arguments);

    void opLoad(const TranslatorArguments &arguments);
    void opStore(const TranslatorArguments &arguments);
    void opConvertUToF(const TranslatorArguments &arguments);
    void opCompositeExtract(const TranslatorArguments &arguments);
    void opCompositeConstruct(const TranslatorArguments &arguments);
    void opAccessChain(const TranslatorArguments &arguments);
public:

    std::vector<uint8_t> compileData();

    explicit CompilerGXP(const std::vector<uint32_t> &data);
};
