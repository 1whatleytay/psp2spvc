#pragma once

#include <gxp/builder.h>

#include <util/spirv.h>

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

class TranslatorConfig {
public:
    bool printDisassembly = false;
    bool printAllocations = false;

    bool optimizeRegisterSpace = false;

    bool logDebug = false;
};

class CompilerGXP : public Compiler {
    gxp::Builder builder;
    TranslatorConfig config;

    std::vector<TranslatorCode> codes;
    std::map<SPIRExtension::Extension, std::unordered_map<GLSLstd450, TranslatorImplementation>> extensions;

    std::map<spv::Id, gxp::ProgramVarying> idVaryings;
    std::map<spv::Id, TranslatorReference> idRegisters;
    std::map<spv::Id, uint32_t> idUseCounts;
    std::map<spv::Id, uint32_t> idUsesLeft;
    std::map<spv::Id, spv::Id> idAliases;
    std::vector<spv::Id> idsCleaned;
    std::map<gxp::ProgramVarying, usse::RegisterReference> varyingReferences;

    static usse::Type translateType(SPIRType::BaseType baseType);
    static usse::DataType translateType(const SPIRType &type);
    static gxp::ParameterSemantic translateDecorations(spv::BuiltIn builtIn);
    static gxp::ProgramVarying translateVarying(spv::BuiltIn builtIn);

    static gxp::ProgramVarying allocateVarying(
        std::vector<gxp::ProgramVarying> &availableVaryings,
        std::vector<gxp::ProgramVarying> &availableTexCoords,
        uint32_t components);

    void createIdUseCounts(const SPIRFunction &function);

    TranslatorReference createVariable(usse::RegisterBank bank, const SPIRType &type);
    TranslatorReference createParameter(gxp::ParameterCategory category, const SPIRType &type,
        const std::string &name);

    spv::Id resolveAlias(spv::Id id);
    void useReference(spv::Id id);
    usse::RegisterReference getReference(spv::Id id);
    void writeReference(spv::Id id, TranslatorReference reg);
    void aliasReference(spv::Id empty, spv::Id value);
    void cleanupRegisters();

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

    explicit CompilerGXP(const std::vector<uint32_t> &data, TranslatorConfig config);
};
