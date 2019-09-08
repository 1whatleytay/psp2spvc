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

    uint32_t getParameterTypeSize(ParameterType type) {
        switch (type) {
        case ParameterType::Unsigned32:
        case ParameterType::Signed32:
        case ParameterType::Float32:
            return 4;
        case ParameterType::Unsigned16:
        case ParameterType::Signed16:
        case ParameterType::Float16:
            return 2;
        case ParameterType::Unsigned8:
        case ParameterType::Signed8:
            return 1;
        default:
            throw std::runtime_error(
                fmt::format("Parameter type {} has no defined size.", static_cast<uint32_t>(type)));
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

    uint32_t getVaryingBits(ProgramVarying varying) {
        switch (varying) {
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

        case ProgramVarying::TexCoord0: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord0);
        case ProgramVarying::TexCoord1: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord1);
        case ProgramVarying::TexCoord2: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord2);
        case ProgramVarying::TexCoord3: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord3);
        case ProgramVarying::TexCoord4: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord4);
        case ProgramVarying::TexCoord5: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord5);
        case ProgramVarying::TexCoord6: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord6);
        case ProgramVarying::TexCoord7: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord7);
        case ProgramVarying::TexCoord8: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord8);
        case ProgramVarying::TexCoord9: return static_cast<uint32_t>(ProgramTexCoordMasks::TexCoord9);

        default: return 0;
        }
    }
    
    void ParameterConfig::setCategory(ParameterCategory category) {
        config &= ~0b1111u;
        config |= static_cast<uint16_t>(category) & 0b1111u;
    }

    void ParameterConfig::setType(ParameterType type) {
        config &= ~(0b1111u << 4u);
        config |= (static_cast<uint16_t>(type) & 0b1111u) << 4u;
    }

    void ParameterConfig::setComponentCount(uint32_t componentCount) {
        config &= ~(0b1111u << 8u);
        config |= (componentCount & 0b1111u) << 8u;
    }

    void ParameterConfig::setContainerIndex(uint32_t containerIndex) {
        config &= ~(0b1111u << 12u);
        config |= (containerIndex & 0b1111u) << 12u;
    }

    ParameterCategory ParameterConfig::getCategory() {
        return static_cast<ParameterCategory>(config & 0b1111u);
    }

    ParameterType ParameterConfig::getType() {
        return static_cast<ParameterType>((config & (0b1111u << 4u)) >> 4u);
    }

    uint32_t ParameterConfig::getComponentCount() {
        return (config & (0b1111u << 8u)) >> 8u;
    }

    uint32_t ParameterConfig::getContainerIndex() {
        return (config & (0b1111u << 12u)) >> 12u;
    }
}
