#pragma once

#include <util/spirv.h>
#include <gxp/builder.h>

#include <map>

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
    explicit TranslatorArguments(
        gxp::Block &block,
        const TranslatorCode &code,
        const uint32_t *instruction,
        uint32_t wordCount);
    friend class CompilerGXP;
public:
    gxp::Block &block;
    const TranslatorCode &code;
    const uint32_t *instruction;
    uint32_t wordCount;
};

class TranslatorReference {
public:
    usse::RegisterReference reference;
    std::vector<TranslatorReference> subreferences;

    bool isStruct();
};

class CompilerGXP : public Compiler {
    gxp::Builder builder;

    std::vector<TranslatorCode> codes;
    std::map<SPIRExtension::Extension, std::unordered_map<GLSLstd450, TranslatorImplementation>> extensions;

    std::map<spv::Id, gxp::ProgramVarying> idVaryings;
    std::map<spv::Id, TranslatorReference> idRegisters;
    std::map<gxp::ProgramVarying, usse::RegisterReference> varyingReferences;

    static usse::Type translateType(SPIRType::BaseType baseType);
    static usse::DataType translateType(const SPIRType &type);
    static gxp::ParameterSemantic translateDecorations(spv::BuiltIn builtIn);
    static gxp::ProgramVarying translateVarying(spv::BuiltIn builtIn);

    static gxp::ProgramVarying allocateVarying(
        std::vector<gxp::ProgramVarying> &availableVaryings,
        std::vector<gxp::ProgramVarying> &availableTexCoords,
        uint32_t components);

    TranslatorReference createVariable(usse::RegisterBank bank, const SPIRType &type);
    TranslatorReference createParameter(gxp::ParameterCategory category, const SPIRType &type,
        const std::string &name);

    usse::RegisterReference getRegister(spv::Id id);

    spv::Id createBlock(const SPIRBlock &block);
    spv::Id createFunction(const SPIRFunction &function);
    void createVertexShaderResources();
    void createFragmentShaderResources();

    void createTranslators();
    void createExtensions();

    // SPRIV Translation OPs
    void unimplemented(const TranslatorArguments &arguments);
    void undefined(const TranslatorArguments &arguments);

    void opLoad(const TranslatorArguments &arguments);
    void opStore(const TranslatorArguments &arguments);
    void opMatrixTimesVector(const TranslatorArguments &arguments);
    void opVectorTimesScalar(const TranslatorArguments &arguments);
    void opConvertUToF(const TranslatorArguments &arguments);
    void opCompositeExtract(const TranslatorArguments &arguments);
    void opCompositeConstruct(const TranslatorArguments &arguments);
    void opAccessChain(const TranslatorArguments &arguments);
    void opVectorShuffle(const TranslatorArguments &arguments);
    void opFNegate(const TranslatorArguments &arguments);
    void opFAdd(const TranslatorArguments &arguments);
    void opFSub(const TranslatorArguments &arguments);
    void opFMul(const TranslatorArguments &arguments);
    void opDot(const TranslatorArguments &arguments);
    void opFunctionCall(const TranslatorArguments &arguments);
    void opExtInst(const TranslatorArguments &arguments);

    // SPIRV Extension OPs
    void extGLSLNormalize(const TranslatorArguments &arguments);
    void extGLSLFMin(const TranslatorArguments &arguments);
    void extGLSLFMax(const TranslatorArguments &arguments);
    void extGLSLReflect(const TranslatorArguments &arguments);
    void extGLSLPow(const TranslatorArguments &arguments);
public:

    std::vector<uint8_t> compileData();

    explicit CompilerGXP(const std::vector<uint32_t> &data);
};
