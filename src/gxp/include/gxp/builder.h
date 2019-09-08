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
        void createMov(usse::RegisterReference source, usse::RegisterReference destination);
        void createPack(usse::RegisterReference source, usse::RegisterReference destination);
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

        std::vector<Block> primaryBlocks;
        std::vector<Block> secondaryBlocks;
        std::vector<Parameter> parameters;
    public:
        void setType(ShaderType type);

        Block *createPrimaryBlock();
        Block *createSecondaryBlock();

        usse::RegisterReference allocateRegister(usse::RegisterBank bank, usse::DataType type);

        usse::RegisterReference registerParameter(const Parameter &parameter);
        std::unordered_map<ProgramVarying, usse::RegisterReference> registerVaryings(
            const std::vector<ProgramVarying> &outputs, const std::vector<ProgramTexCoordInfo> &texCoords);

        std::vector<uint8_t> build();

        Builder();
    };
}
