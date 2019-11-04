#include <gxp/usse.h>

#include <fmt/format.h>

#include <cmath>
#include <stdexcept>

namespace usse {
    float fpConstants[] = {
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        2.0f,
        8.0f,
        32.0f,
        128.0f,
        512.0f,
        2048.0f,
        8192.0f,
        32768.0f,
        0.5f,
        0.125f,
        0.03125f,
        0.0078125f,
        0.001953125f,
        0.00048828125f,
        0.0001220703125f,
        3.0517578125e-05f,
        2.7182817459106445f,
        1.4142135381698608f,
        3.1415927410125732f,
        0.7853981852531433f,
        6.2831854820251465f,
        25.132741928100586f,
        1.52587890625e-05f,
        1.5259021893143654e-05f,
        1.5500992276429315e-06f,
        0.0002604166802484542f,
        0.02083333395421505f,
        0.5f,
        0.0f,
        0.0f,
        0.007826805114746094f,
        513.0f,
        2.204391964672e+12f,
        9.472403081849855e+21f,
        4.07034684917033e+31f,
        1.1941301636397839e-07f,
        2.7789456933519086e-17f,
        6.467081701001855e-27f,
        1.50500011103716e-36f,
        1.68573558312346e-06f,
        0.0003208939451724291f,
        0.1955653429031372f,
        3281298.0f,
        0.0f,
        0.0f,
        4.661918073800564e-10f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        -NAN,
        -NAN,
        -NAN,
        -NAN,
        NAN,
        NAN,
        NAN,
        NAN,
    };

    const uint32_t swizzleStandardSize = 16;
    SwizzleVec4 swizzleVector4[2][swizzleStandardSize] = {
        {
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Y },
            { SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Z },
            { SwizzleChannel::W, SwizzleChannel::W, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::W },
            { SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::Z },
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::W, SwizzleChannel::Z },
            { SwizzleChannel::Z, SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::W },
            { SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::Z, SwizzleChannel::W },
            { SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::X, SwizzleChannel::Z },
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::One },
        },
        {
            { SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::X, SwizzleChannel::W },
            { SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::Y },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::W, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::W },
            { SwizzleChannel::W, SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::Z },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::X },
            { SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::W, SwizzleChannel::Z, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Z },
            { SwizzleChannel::X, SwizzleChannel::Z, SwizzleChannel::Y, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::Z, SwizzleChannel::Y, SwizzleChannel::X, SwizzleChannel::W },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::Z },
            { SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Y },
        },
    };

    SwizzleVec3 swizzleVector3[2][swizzleStandardSize] = {
        {
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Y },
            { SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Z },
            { SwizzleChannel::W, SwizzleChannel::W, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z },
            { SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::W },
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::X },
            { SwizzleChannel::Y, SwizzleChannel::Y, SwizzleChannel::Z },
            { SwizzleChannel::Z, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Z, SwizzleChannel::Y },
            { SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::X },
            { SwizzleChannel::Z, SwizzleChannel::Y, SwizzleChannel::X },
            { SwizzleChannel::Z, SwizzleChannel::Z, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::One },
        },
        {
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Y },
            { SwizzleChannel::Y, SwizzleChannel::X, SwizzleChannel::Y },
            { SwizzleChannel::X, SwizzleChannel::X, SwizzleChannel::Z },
            { SwizzleChannel::Y, SwizzleChannel::X, SwizzleChannel::X },
            { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Zero },
            { SwizzleChannel::X, SwizzleChannel::One, SwizzleChannel::Zero },
            { SwizzleChannel::Zero, SwizzleChannel::Zero, SwizzleChannel::Zero },
            { SwizzleChannel::One, SwizzleChannel::One, SwizzleChannel::One },
            { SwizzleChannel::Half, SwizzleChannel::Half, SwizzleChannel::Half },
            { SwizzleChannel::Two, SwizzleChannel::Two, SwizzleChannel::Two },
            { SwizzleChannel::X, SwizzleChannel::Zero, SwizzleChannel::Zero },
            { /* Undefined */ },
            { /* Undefined */ },
            { /* Undefined */ },
            { /* Undefined */ },
        },
    };

    bool BankLayout::isHalfType(Type type) {
        return (
            type == Type::Float32 ||
            type == Type::Float16 ||
            type == Type::Fixed10) &&

            bank != usse::RegisterBank::FloatConstant &&
            bank != usse::RegisterBank::Immediate;
    }

    uint32_t BankLayout::getIndex(RegisterReference reference, bool enableDoubleRegs, uint32_t bits) {
        uint32_t index = reference.index;
        bool doubleReg = enableDoubleRegs && isHalfType(reference.type.type);

        if (bank == RegisterBank::Internal)
            index += (60 + (doubleReg ? 0 : 2)) * (bits == 7 ? 2 : 1);
        else if (doubleReg)
            index /= 2;

        return index;
    }

    BankLayout BankLayout::destLayout(RegisterBank bank) {
        switch (bank) {
        case RegisterBank::Primary: return { bank, 0, 2 };
        case RegisterBank::Secondary: return { bank, 1, 0 };
        case RegisterBank::Output: return { bank, 0, 1 };
        case RegisterBank::Internal:
        case RegisterBank::Temporary: return {bank, 0, 0 };
        case RegisterBank::Special: return { bank, 1, 1 };
        case RegisterBank::Index: return { bank, 1, 2 };
        case RegisterBank::Indexed1: return { bank, 0, 3 };
        case RegisterBank::Indexed2: return { bank, 1, 3 };
        default:
            throw std::runtime_error("Unsupported dest bank.");
        }
    }
    BankLayout BankLayout::src0Layout(RegisterBank bank) {
        switch (bank) {
        case RegisterBank::Primary: return { bank, 0, 1 };
        case RegisterBank::Secondary: return { bank, 1, 1 };
        case RegisterBank::Output: return { bank, 1, 0 };
        case RegisterBank::Temporary: return {bank, 0, 0 };
        default:
            throw std::runtime_error("Unsupported src0 bank.");
        }
    }
    BankLayout BankLayout::srcLayout(RegisterBank bank) {
        switch (bank) {
        case RegisterBank::Internal:
        case RegisterBank::Temporary: return {bank, 0, 0 };
        case RegisterBank::Primary: return { bank, 0, 2 };
        case RegisterBank::Output: return { bank, 0, 1 };
        case RegisterBank::Secondary: return { bank, 0, 3 };
        case RegisterBank::FloatConstant:
        case RegisterBank::Special: return { bank, 1, 1 };
        case RegisterBank::Immediate: return { bank, 1, 2 };
        case RegisterBank::Indexed1: return { bank, 1, 0 };
        case RegisterBank::Indexed2: return { bank, 1, 3 };
        default:
            throw std::runtime_error("Unsupported src bank.");
        }
    }

    uint32_t RegisterReference::getSwizzleMask() {
        uint32_t mask = 0;

        for (uint32_t a = 0; a < type.components; a++) {
            SwizzleChannel channel = swizzle[a];
            switch (channel) {
            case SwizzleChannel::X:
                mask |= 0b0001u;
                break;
            case SwizzleChannel::Y:
                mask |= 0b0010u;
                break;
            case SwizzleChannel::Z:
                mask |= 0b0100u;
                break;
            case SwizzleChannel::W:
                mask |= 0b1000u;
                break;
            case SwizzleChannel::DontCare:
                break;
            default:
                throw std::runtime_error("Unimplemented swizzle.");
            }
        }

        return mask;
    }

    int32_t RegisterReference::getSwizzleIndex(bool extended, int32_t components) {
        switch (components == -1 ? type.components : components) {
        case 4: {
            usse::SwizzleVec4 vec;
            std::copy(swizzle.begin(), swizzle.end(), vec.begin());
            return usse::getSwizzleVec4Index(vec, extended);
        }
        case 3: {
            usse::SwizzleVec3 vec;
            bool firstIsBlank = swizzle[0] == usse::SwizzleChannel::DontCare;
            std::copy(swizzle.begin() + firstIsBlank, swizzle.end() - !firstIsBlank, vec.begin());
            return usse::getSwizzleVec3Index(vec, extended);
        }
        case 1:
            return usse::getSwizzleScalarIndex(swizzle[0]);
        default:
            throw std::runtime_error("Invalid component count for swizzle.");
        }
    }

    uint32_t RegisterReference::getEffectiveIndex() {
        return index + static_cast<uint32_t>(swizzle[0]);
    }

    RegisterReference RegisterReference::getHalf(uint32_t half) {
        uint32_t width = (type.components - 1) / 2 + 1;

        return getComponents(width * half, width);
    }

    RegisterReference RegisterReference::getComponents(uint32_t component, uint32_t count) {
        RegisterReference reg = *this;

        int32_t swizzleOffset = 0;

        if (component >= 2) {
            reg.index += 2;
            swizzleOffset = -2;
        }

        reg.type.components = count;
        reg.type.arraySize = 1;

        reg.swizzle = usse::getSwizzleVec4All(SwizzleChannel::DontCare);
        for (uint32_t a = 0; a < count; a++) {
            if (lockSwizzle)
                reg.swizzle[a] = swizzle[component + a];
            else
                reg.swizzle[a] = static_cast<SwizzleChannel>(component + a + swizzleOffset);
        }

        return reg;
    }

    RegisterReference RegisterReference::getElement(uint32_t element) {
        if (element >= type.arraySize)
            throw std::runtime_error("Register reference array out of bounds.");

        return RegisterReference({ type.type, type.components, 1 }, bank, index + size / type.arraySize * element);
    }

    RegisterReference RegisterReference::getExpanded(uint32_t count) {
        RegisterReference reg = *this;

        reg.lockSwizzle = true;
        reg.swizzle = usse::getSwizzleVec4All(swizzle[0]);
        reg.type.components = count;

        return reg;
    }

    RegisterReference RegisterReference::getAligned(uint8_t writeMask) {
        RegisterReference reg = *this;

        reg.swizzle = usse::getSwizzleVec4All(usse::SwizzleChannel::DontCare);

        uint32_t swizzleIndex = 0;
        for (uint32_t a = 0; a < 4; a++) {
            if (writeMask & (1u << a)) {
                reg.swizzle[a] = swizzle[swizzleIndex++];
            }
        }

        return reg;
    }

    RegisterReference::RegisterReference(DataType type, RegisterBank bank, uint32_t regIndex)
        : type(type), bank(bank), size(getTypeSize(type.type) * type.components * type.arraySize / 4) {
        bool swizzleUp = false;

        if (bank != usse::RegisterBank::Internal && regIndex % 2 == 1) {
            regIndex--;
            swizzleUp = true;
        }

        for (uint32_t a = 0; a < type.components; a++) {
            swizzle[a] = static_cast<usse::SwizzleChannel>(a + swizzleUp);
        }
        index = regIndex;
    }

    std::string getTypeName(Type type) {
        switch (type) {
        case Type::Signed8: return "Byte";
        case Type::Signed16: return "Short";
        case Type::Signed32: return "Int";
        case Type::Fixed10: return "Fixed";
        case Type::Float16: return "Half";
        case Type::Float32: return "Float";
        case Type::Unsigned8: return "Ubyte";
        case Type::Unsigned16: return "Ushort";
        case Type::Unsigned32: return "Uint";
        case Type::Output8: return "Output";
        default: return "Invalid";
        }
    }

    uint32_t getTypeSize(Type type) {
        switch (type) {
        case Type::Signed8: return 1;
        case Type::Signed16: return 2;
        case Type::Signed32: return 4;
        case Type::Fixed10: return 2; // Oops
        case Type::Float16: return 2;
        case Type::Float32: return 4;
        case Type::Unsigned8: return 1;
        case Type::Unsigned16: return 2;
        case Type::Unsigned32: return 4;
        case Type::Output8: return 1;
        default:
            throw std::runtime_error("Unknown type.");
        }
    }

    std::string getBankName(RegisterBank bank) {
        switch (bank) {
        case RegisterBank::Temporary: return "Temp";
        case RegisterBank::Primary: return "Primary";
        case RegisterBank::Output: return "Output";
        case RegisterBank::Secondary: return "Secondary";
        case RegisterBank::Internal: return "Internal";
        case RegisterBank::Special: return "Special";
        case RegisterBank::Global: return "Global";
        case RegisterBank::FloatConstant: return "Float Constant";
        case RegisterBank::Immediate: return "Immediate";
        case RegisterBank::Index: return "Index";
        case RegisterBank::Indexed1: return "Indexed 1";
        case RegisterBank::Indexed2: return "Indexed 2";
        case RegisterBank::Predicate: return "Predicate";
        case RegisterBank::Invalid:
        default:
            return "Invalid";
        }
    }

    int32_t getFPConstantIndex(float constant) {
        for (size_t a = 0; a < (sizeof(fpConstants) / sizeof(fpConstants[0])); a++) {
            if (fpConstants[a] == constant)
                return a;
        }

        return -1;
    }

    bool areSwizzlesInMatchingHalf(SwizzleChannel x, SwizzleChannel y) {
        return
            ((x == SwizzleChannel::X || x == SwizzleChannel::Y) &&
                (y == SwizzleChannel::X || y == SwizzleChannel::Y)) ||
            ((x == SwizzleChannel::Z || x == SwizzleChannel::W) &&
                (y == SwizzleChannel::Z || y == SwizzleChannel::W));
    }

    int32_t getSwizzleScalarIndex(SwizzleChannel element) {
        return static_cast<int32_t>(element);
    }
    int32_t getSwizzleVec3Index(std::array<SwizzleChannel, 3> elements, bool extended) {
        for (uint32_t a = 0; a < swizzleStandardSize; a++) {
            bool matches = true;

            for (uint32_t b = 0; b < 3; b++) {
                if (elements[b] != SwizzleChannel::DontCare) {
                    if (swizzleVector3[extended][a][b] != elements[b]) {
                        matches = false;
                        break;
                    }
                }
            }

            if (matches)
                return a;
        }

        throw std::runtime_error("Missing swizzle index for vec3.");
    }
    int32_t getSwizzleVec4Index(std::array<SwizzleChannel, 4> elements, bool extended) {
        for (uint32_t a = 0; a < swizzleStandardSize; a++) {
            bool matches = true;

            for (uint32_t b = 0; b < 4; b++) {
                if (elements[b] != SwizzleChannel::DontCare) {
                    if (swizzleVector4[extended][a][b] != elements[b]) {
                        matches = false;
                        break;
                    }
                }
            }

            if (matches)
                return a;
        }

        throw std::runtime_error("Missing swizzle index for vec4.");
    }
}
