#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstdint>

namespace usse {
    enum class RegisterBank {
        Temporary,
        Primary,
        Output,
        Secondary,
        Internal,
        Special,
        Global,
        FloatConstant,
        Immediate,
        Index,
        Indexed1,
        Indexed2,
        Predicate,

        Invalid,
    };

    enum class SwizzleChannel : uint8_t {
        X,
        Y,
        Z,
        W,
        Zero,
        One,
        Two,
        Half,

        DontCare,
    };

    enum class Type : uint8_t {
        Signed8,
        Signed16,
        Signed32,
        Fixed10,
        Float16,
        Float32,
        Unsigned8,
        Unsigned16,
        Unsigned32,
        Output8,
    };

    enum class InstructionVNMADOp : uint8_t {
        Multiply,
        Add,
        Fractional,
        DSX,
        DSY,
        Min,
        Max,
        Dot,
    };

    enum class InstructionVCOMPOp : uint8_t {
        Reciprocal,
        ReverseSquareRoot,
        Logarithm,
        Exponent,
    };

    typedef std::array<SwizzleChannel, 3> SwizzleVec3;
    typedef std::array<SwizzleChannel, 4> SwizzleVec4;

    bool areSwizzlesInMatchingHalf(SwizzleChannel x, SwizzleChannel y);
    int32_t getFPConstantIndex(float constant);
    int32_t getSwizzleScalarIndex(SwizzleChannel element);
    int32_t getSwizzleVec3Index(SwizzleVec3 elements, bool extended = false);
    int32_t getSwizzleVec4Index(SwizzleVec4 elements, bool extended = false);

    inline SwizzleVec3 getSwizzleVec3All(SwizzleChannel channel) {
        return { channel, channel, channel };
    }

    inline SwizzleVec4 getSwizzleVec4All(SwizzleChannel channel) {
        return { channel, channel, channel, channel };
    }

    inline SwizzleVec3 getSwizzleVec3Default() {
        return { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z };
    }

    inline SwizzleVec4 getwizzleVec4Default() {
        return { SwizzleChannel::X, SwizzleChannel::Y, SwizzleChannel::Z, SwizzleChannel::W };
    }

    class DataType {
    public:
        Type type = usse::Type::Float32;
        uint32_t components = 1;
        uint32_t arraySize = 1;
    };

    class RegisterReference {
    public:
        DataType type;
        RegisterBank bank = RegisterBank::Invalid;
        uint32_t index = 0;
        uint32_t size = 1;
        bool lockSwizzle = false;
        usse::SwizzleVec4 swizzle = usse::getSwizzleVec4All(usse::SwizzleChannel::DontCare);

        uint32_t getSwizzleMask();
        int32_t getSwizzleIndex(bool extended = false);
        uint32_t getEffectiveIndex();
        RegisterReference getHalf(uint32_t half);
        RegisterReference getComponents(uint32_t component, uint32_t count);
        RegisterReference getElement(uint32_t element);
        RegisterReference getExpanded(uint32_t count);

        RegisterReference() = default;
        RegisterReference(DataType type, RegisterBank bank, uint32_t regIndex);
    };

    class BankLayout {
    public:
        usse::RegisterBank bank = usse::RegisterBank::Invalid;

        uint8_t extension = 0;
        uint8_t number = 0;

        bool isHalf(Type type);
        uint32_t getIndex(RegisterReference reference, uint32_t bits = 7);

        static BankLayout destLayout(RegisterBank bank);
        static BankLayout src0Layout(RegisterBank bank);
        static BankLayout srcLayout(RegisterBank bank);
    };

    std::string getTypeName(Type type);
    uint32_t getTypeSize(Type type);
    std::string getBankName(RegisterBank bank);
}
