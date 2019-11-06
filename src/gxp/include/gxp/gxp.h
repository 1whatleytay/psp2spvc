#pragma once

#include <gxp/usse.h>

#include <vector>

namespace gxp {
    // GXP\0
    constexpr uint32_t gxpMagic = 0x00505847;

    enum class ParameterCategory : uint8_t {
        Attribute,
        Uniform,
        Sampler,
        AuxiliarySurface,
        UniformBuffer,
    };

    enum class ParameterType : uint8_t {
        Float32,
        Float16,
        Fixed10,
        Unsigned32,
        Signed32,
        Unsigned16,
        Signed16,
        Unsigned8,
        Signed8,
        Aggregate,
    };

    ParameterType getParameterTypeFromUSSEType(usse::Type type);
    uint32_t getParameterTypeSize(ParameterType type);

    enum class ParameterSemantic : uint16_t {
        None,
        Attr,
        Bcol,
        BiNormal,
        BlendIndices,
        BlendWeight,
        Color,
        Diffuse,
        FogCoord,
        Normal,
        PointSize,
        Position,
        Specular,
        Tangent,
        TexCoord,
    };

    enum class ProgramVaryingVertexBits : uint32_t {
        Clip0 = 0x0001,
        Clip1 = 0x0002,
        Clip2 = 0x0004,
        Clip3 = 0x0008,
        Clip4 = 0x0010,
        Clip5 = 0x0020,
        Clip6 = 0x0040,
        Clip7 = 0x0080,
        PointSize = 0x100,
        Fog = 0x0200,
        Color1 = 0x0400,
        Color0 = 0x0800,
    };

    enum class ProgramTexCoordVertexMasks : uint32_t {
        TexCoord0 = 0b111u << (3u * 0u),
        TexCoord1 = 0b111u << (3u * 1u),
        TexCoord2 = 0b111u << (3u * 2u),
        TexCoord3 = 0b111u << (3u * 3u),
        TexCoord4 = 0b111u << (3u * 4u),
        TexCoord5 = 0b111u << (3u * 5u),
        TexCoord6 = 0b111u << (3u * 6u),
        TexCoord7 = 0b111u << (3u * 7u),
        TexCoord8 = 0b111u << (3u * 8u),
        TexCoord9 = 0b111u << (3u * 9u),
    };

    enum class ProgramVaryingFragmentBits : uint32_t {
        Position = 0xD000,
        Fog = 0xC000,
        Color0 = 0xA000,
        Color1 = 0xB000,
        TexCoord0 = 0x0000,
        TexCoord1 = 0x1000,
        TexCoord2 = 0x2000,
        TexCoord3 = 0x3000,
        TexCoord4 = 0x4000,
        TexCoord5 = 0x5000,
        TexCoord6 = 0x6000,
        TexCoord7 = 0x7000,
        TexCoord8 = 0x8000,
        TexCoord9 = 0x9000,
    };

    enum class ProgramVarying {
        None,
        Position,
        Fog,
        Color0,
        Color1,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
        TexCoord4,
        TexCoord5,
        TexCoord6,
        TexCoord7,
        TexCoord8,
        TexCoord9,
        PointSize,
        Clip0,
        Clip1,
        Clip2,
        Clip3,
        Clip4,
        Clip5,
        Clip6,
        Clip7,
    };

    std::string getVaryingName(ProgramVarying varying);
    bool iClipVarying(ProgramVarying varying);
    bool isTexCoordVarying(ProgramVarying varying);
    uint32_t getVertexVaryingBits(ProgramVarying varying);
    uint32_t getFragmentVaryingBits(ProgramVarying varying);

    enum class ContainerIndex : uint16_t {
        Buffer0,
        Buffer1,
        Buffer2,
        Buffer3,
        Buffer4,
        Buffer5,
        Buffer6,
        Buffer7,
        Buffer8,
        Buffer9,
        Buffer10,
        Buffer12,
        Buffer13,
        Default,
        Texture,
        Literal,
        Scratch,
        Thread,
        Data,
    };

    class ProgramVectorInfo {
    public:
        ProgramVarying varying;
        uint32_t components;
    };

    class ProgramFragmentInputInfo {
    public:
        uint32_t attributeInfo = 0;
        uint32_t resourceIndex = 0;
        uint32_t size = 0;
        uint32_t componentInfo = 0;
    };

    class ProgramContainerInfo {
    public:
        uint16_t containerIndex;
        uint16_t unk02;
        uint16_t secondaryOffset;
        uint16_t maxResourceIndex;
    };

    class ProgramParameterInfo {
    public:
        int32_t nameOffset = 0;
        uint16_t config = 0;
        uint16_t semantic = 0;
        uint32_t arraySize = 0;
        uint32_t resourceIndex = 0;
    };

    class ProgramVaryings {
    public:
        std::uint8_t unk0[10] = { };
        std::uint8_t output_param_type = 0;
        std::uint8_t output_comp_count = 0;

        std::uint16_t varyings_count = 0;
        std::uint16_t pad0 = 0;
        std::uint32_t vertex_outputs1 = 0;
        std::uint32_t vertex_outputs2 = 0;
    };

    // Vita3K's SceGxmProgram structure
    class ProgramHeader {
    public:
        uint32_t magic = 0;

        uint8_t majorVersion = 0;
        uint8_t minorVersion = 0;
        uint16_t unk6 = 0;

        uint32_t size = 0;
        uint32_t unkC = 0;

        uint16_t unk10 = 0;
        uint8_t unk12 = 0;
        uint8_t unk13 = 0;

        uint8_t type = 0;
        uint8_t unk15 = 0;
        uint8_t unk16 = 0;
        uint8_t unk17 = 0;

        uint32_t unk18 = 0;
        uint32_t unk1C = 0;
        uint32_t unk20 = 0;

        uint32_t parameterCount = 0;
        uint32_t parametersOffset = 0;
        uint32_t varyingsOffset = 0;

        uint16_t primaryRegCount = 0;
        uint16_t secondaryRegCount = 0;
        uint16_t tempRegCount1 = 0;
        uint16_t unk36 = 0;
        uint16_t tempRegCount2 = 0;
        uint16_t unk3A = 0;

        uint32_t primaryProgramInstructionCount = 0;
        uint32_t primaryProgramOffset = 0;

        uint32_t unk44 = 0;

        uint32_t secondaryProgramOffset = 0;
        uint32_t secondaryProgramOffsetEnd = 0;

        uint32_t unk50 = 0;
        uint32_t unk54 = 0;
        uint32_t unk58 = 0;
        uint32_t unk5C = 0;

        uint32_t unk60 = 0;
        uint32_t defaultUniformBufferCount = 0;
        uint32_t unk68 = 0;
        uint32_t unk6C = 0;

        uint32_t literalsCount = 0;
        uint32_t literalsOffset = 0;
        uint32_t unk78 = 0;
        uint32_t alternativeParametersOffset = 0;

        uint32_t dependentSamplerCount = 0;
        uint32_t dependentSamplerOffset = 0;
        uint32_t unk88 = 0;
        uint32_t unk8C = 0;
        uint32_t containerCount = 0;
        uint32_t containerOffset = 0;
    };

    uint16_t createParameterConfig(ParameterCategory category, ParameterType type,
        uint32_t components, ContainerIndex containerIndex);
}
