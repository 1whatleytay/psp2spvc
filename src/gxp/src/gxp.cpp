#include <gxp/gxp.h>

#include <fmt/format.h>

namespace gxp {
    ParameterType getParameterTypeFromUSSEType(usse::Type type) {
        switch (type) {
        case usse::Type::Signed8: return ParameterType::Signed8;
        case usse::Type::Signed16: return ParameterType::Signed16;
        case usse::Type::Signed32: return ParameterType::Signed32;
        case usse::Type::Fixed10: return ParameterType::Fixed10;
        case usse::Type::Float16: return ParameterType::Float16;
        case usse::Type::Float32: return ParameterType::Float32;
        case usse::Type::Unsigned8: return ParameterType::Unsigned8;
        case usse::Type::Unsigned16: return ParameterType::Unsigned16;
        case usse::Type::Unsigned32: return ParameterType::Unsigned32;
        default:
            throw std::runtime_error("");
        }
    }

    std::string getVaryingName(ProgramVarying varying) {
        switch (varying) {
        case ProgramVarying::Position: return "Position";
        case ProgramVarying::Fog: return "Fog";
        case ProgramVarying::Color0: return "Color0";
        case ProgramVarying::Color1: return "Color1";
        case ProgramVarying::TexCoord0: return "TexCoord0";
        case ProgramVarying::TexCoord1: return "TexCoord1";
        case ProgramVarying::TexCoord2: return "TexCoord2";
        case ProgramVarying::TexCoord3: return "TexCoord3";
        case ProgramVarying::TexCoord4: return "TexCoord4";
        case ProgramVarying::TexCoord5: return "TexCoord5";
        case ProgramVarying::TexCoord6: return "TexCoord6";
        case ProgramVarying::TexCoord7: return "TexCoord7";
        case ProgramVarying::TexCoord8: return "TexCoord8";
        case ProgramVarying::TexCoord9: return "TexCoord9";
        case ProgramVarying::PointSize: return "PointSize";
        case ProgramVarying::Clip0: return "Clip0";
        case ProgramVarying::Clip1: return "Clip1";
        case ProgramVarying::Clip2: return "Clip2";
        case ProgramVarying::Clip3: return "Clip3";
        case ProgramVarying::Clip4: return "Clip4";
        case ProgramVarying::Clip5: return "Clip5";
        case ProgramVarying::Clip6: return "Clip6";
        case ProgramVarying::Clip7: return "Clip7";
        case ProgramVarying::None:
        default:
            return "Invalid";
        }
    }

    bool iClipVarying(ProgramVarying varying) {
        auto varyingNum = static_cast<uint32_t>(varying);
        return varyingNum >= static_cast<uint32_t>(ProgramVarying::Clip0)
            && varyingNum <= static_cast<uint32_t>(ProgramVarying::Clip7);
    }
    bool isTexCoordVarying(ProgramVarying varying) {
        auto varyingNum = static_cast<uint32_t>(varying);
        return varyingNum >= static_cast<uint32_t>(ProgramVarying::TexCoord0)
            && varyingNum <= static_cast<uint32_t>(ProgramVarying::TexCoord9);
    }

    uint32_t getVertexVaryingBits(ProgramVarying varying) {
        switch (varying) {
        case ProgramVarying::Position: return static_cast<uint32_t>(ProgramVaryingVertexBits::Position);

        case ProgramVarying::Fog: return static_cast<uint32_t>(ProgramVaryingVertexBits::Fog);
        case ProgramVarying::Color0: return static_cast<uint32_t>(ProgramVaryingVertexBits::Color0);
        case ProgramVarying::Color1: return static_cast<uint32_t>(ProgramVaryingVertexBits::Color1);
        case ProgramVarying::PointSize: return static_cast<uint32_t>(ProgramVaryingVertexBits::PointSize);
        case ProgramVarying::Clip0: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip0);
        case ProgramVarying::Clip1: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip1);
        case ProgramVarying::Clip2: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip2);
        case ProgramVarying::Clip3: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip3);
        case ProgramVarying::Clip4: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip4);
        case ProgramVarying::Clip5: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip5);
        case ProgramVarying::Clip6: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip6);
        case ProgramVarying::Clip7: return static_cast<uint32_t>(ProgramVaryingVertexBits::Clip7);

        case ProgramVarying::TexCoord0: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord0);
        case ProgramVarying::TexCoord1: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord1);
        case ProgramVarying::TexCoord2: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord2);
        case ProgramVarying::TexCoord3: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord3);
        case ProgramVarying::TexCoord4: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord4);
        case ProgramVarying::TexCoord5: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord5);
        case ProgramVarying::TexCoord6: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord6);
        case ProgramVarying::TexCoord7: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord7);
        case ProgramVarying::TexCoord8: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord8);
        case ProgramVarying::TexCoord9: return static_cast<uint32_t>(ProgramTexCoordVertexMasks::TexCoord9);

        default: return 0;
        }
    }

    uint32_t getFragmentVaryingBits(ProgramVarying varying) {
        switch (varying) {
        case ProgramVarying::Position: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Position);
        case ProgramVarying::Fog: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Fog);
        case ProgramVarying::Color0: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Color0);
        case ProgramVarying::Color1: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Color1);
        case ProgramVarying::TexCoord0: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord0);
        case ProgramVarying::TexCoord1: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord1);
        case ProgramVarying::TexCoord2: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord2);
        case ProgramVarying::TexCoord3: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord3);
        case ProgramVarying::TexCoord4: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord4);
        case ProgramVarying::TexCoord5: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord5);
        case ProgramVarying::TexCoord6: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord6);
        case ProgramVarying::TexCoord7: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord7);
        case ProgramVarying::TexCoord8: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord8);
        case ProgramVarying::TexCoord9: return static_cast<uint32_t>(ProgramVaryingFragmentBits::TexCoord9);
//        case ProgramVarying::PointSize: return static_cast<uint32_t>(ProgramVaryingFragmentBits::PointSize);
//        case ProgramVarying::Clip0: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip0);
//        case ProgramVarying::Clip1: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip1);
//        case ProgramVarying::Clip2: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip2);
//        case ProgramVarying::Clip3: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip3);
//        case ProgramVarying::Clip4: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip4);
//        case ProgramVarying::Clip5: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip5);
//        case ProgramVarying::Clip6: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip6);
//        case ProgramVarying::Clip7: return static_cast<uint32_t>(ProgramVaryingFragmentBits::Clip7);

        default: return 0;
        }
    }

    uint16_t createParameterConfig(ParameterCategory category, ParameterType type,
        uint32_t components, ContainerIndex containerIndex) {
        uint16_t config = 0;
        config |= (static_cast<uint16_t>(containerIndex) & 0b1111u) << 12u;
        config |= (components & 0b1111u) << 8u;
        config |= (static_cast<uint16_t>(type) & 0b1111u) << 4u;
        config |= static_cast<uint16_t>(category) & 0b1111u;
        return config;
    }
}
