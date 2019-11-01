#pragma once

#include <gxp/gxp.h>
#include <gxp/block.h>

#include <map>
#include <string>

namespace gxp {
    typedef uint64_t Instruction;

    enum class ShaderType : uint8_t {
        Vertex = 0,
        Fragment = 1,
    };

    // Should be 64 (some instructions only have 6 bit index), at most 128. Set higher for non -Oreg-space shaders.
    constexpr size_t maxTemporaryRegisters = 128;

    class Parameter {
    public:
        std::string name;
        ParameterCategory category = ParameterCategory::Uniform;
        ParameterSemantic semantic = ParameterSemantic::None;

        usse::DataType type;

        // Will be filled out by Builder.
        uint32_t resourceIndex = 0;
        uint32_t containerIndex = 0;

        usse::RegisterBank getBank();
    };

    class BuilderConfig {
    public:
        bool printDisassembly = false;
        bool printAllocations = false;
    };

    class Builder {
        BuilderConfig config;

        ProgramHeader header;
        ProgramVaryings varyings;

        uint32_t paRegPointer = 0;
        uint32_t saRegPointer = 0;
        uint32_t oRegPointer = 0;
        uint32_t iRegPointer = 0;

        std::array<bool, maxTemporaryRegisters> tRegSpace;
        uint32_t tMaxRegs = 0;

        std::vector<std::unique_ptr<Block>> primaryBlocks;
        std::vector<std::unique_ptr<Block>> secondaryBlocks;
        std::vector<Parameter> parameters;
        std::vector<ProgramFragmentInputInfo> fragmentInputs;

        friend class Block;
    public:
        void setType(ShaderType type);
        ShaderType getType();

        Block *createPrimaryBlock();
        Block *createSecondaryBlock();

        usse::RegisterReference allocateRegister(usse::RegisterBank bank, usse::DataType type);
        void freeRegister(usse::RegisterReference reg);

        usse::RegisterReference registerParameter(const Parameter &parameter);

        std::map<ProgramVarying, usse::RegisterReference> registerVertexVaryings(
            const std::vector<ProgramVarying> &outputs, const std::vector<ProgramVectorInfo> &texCoords);
        std::map<ProgramVarying, usse::RegisterReference> registerFragmentVaryings(
            const std::vector<ProgramVectorInfo> &inputs /*, samplers...*/);

        usse::RegisterReference createFragmentOutput(usse::Type type, uint32_t components);

        std::vector<uint8_t> build();

        Builder();
        explicit Builder(BuilderConfig config);
    };
}
