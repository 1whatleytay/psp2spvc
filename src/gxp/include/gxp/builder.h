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

    class Builder {
        ProgramHeader header;
        ProgramVaryings varyings;

        bool printDisassembly = false;
        bool printAllocations = false;

        uint32_t paRegPointer = 0;
        uint32_t saRegPointer = 0;
        uint32_t oRegPointer = 0;
        uint32_t tRegPointer = 0;
        uint32_t iRegPointer = 0;

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
        Builder(bool printDisassembly, bool printAllocations);
    };
}
