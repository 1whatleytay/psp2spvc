#pragma once

#include <gxp/gxp.h>

#include <string>
#include <unordered_map>

namespace gxp {
    typedef uint64_t Instruction;

    class Builder;

    enum class ShaderType : uint8_t {
        Vertex = 0,
        Fragment = 1,
    };

    class Block {
        std::vector<Instruction> instructions;

        explicit Block(Builder &parent);
        friend class gxp::Builder;
    public:
        Builder &parent;

        void createNop();
        void createMov(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createPack(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createDot(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createSub(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
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

        uint32_t paRegPointer = 0;
        uint32_t saRegPointer = 0;
        uint32_t oRegPointer = 0;
        uint32_t tRegPointer = 0;
        uint32_t iRegPointer = 0;

        std::vector<Block> primaryBlocks;
        std::vector<Block> secondaryBlocks;
        std::vector<Parameter> parameters;
        std::vector<ProgramFragmentInputInfo> fragmentInputs;
    public:
        void setType(ShaderType type);
        ShaderType getType();

        Block *createPrimaryBlock();
        Block *createSecondaryBlock();

        usse::RegisterReference allocateRegister(usse::RegisterBank bank, usse::DataType type);
        void freeRegister(usse::RegisterReference reg);

        usse::RegisterReference registerParameter(const Parameter &parameter);

        std::unordered_map<ProgramVarying, usse::RegisterReference> registerVertexVaryings(
            const std::vector<ProgramVarying> &outputs, const std::vector<ProgramVectorInfo> &texCoords);
        std::unordered_map<ProgramVarying, usse::RegisterReference> registerFragmentVaryings(
            const std::vector<ProgramVectorInfo> &inputs /*, samplers...*/);

        usse::RegisterReference createFragmentOutput(usse::Type type, uint32_t components);

        std::vector<uint8_t> build();

        Builder();
    };
}
