#include <gxp/disasm.h>

#include <fmt/format.h>

#include <sstream>

namespace usse::disasm {
    static std::string getBankRegisterName(usse::RegisterBank bank) {
        switch (bank) {
        case RegisterBank::Temporary: return "r";
        case RegisterBank::Primary: return "pa";
        case RegisterBank::Output: return "o";
        case RegisterBank::Secondary: return "sa";
        case RegisterBank::Internal: return "i";
        case RegisterBank::Special: return "s";
        case RegisterBank::Global: return "g";
        case RegisterBank::FloatConstant: return "fp";
        case RegisterBank::Immediate: return "#";
        case RegisterBank::Index: return "index";
        case RegisterBank::Indexed1: return "indexed[1]";
        case RegisterBank::Indexed2: return "indexed[2]";
        case RegisterBank::Predicate: return "p";
        case RegisterBank::Invalid: return "invalid";
        default: return "unknown";
        }
    }

    static std::string getSwizzleName(usse::SwizzleChannel channel) {
        switch (channel) {
        case SwizzleChannel::X: return "x";
        case SwizzleChannel::Y: return "y";
        case SwizzleChannel::Z: return "z";
        case SwizzleChannel::W: return "w";
        case SwizzleChannel::Zero: return "0";
        case SwizzleChannel::One: return "1";
        case SwizzleChannel::Two: return "2";
        case SwizzleChannel::Half: return "h";
        case SwizzleChannel::DontCare: return "*";
        }
    }

    static std::string getFullSwizzleName(const SwizzleVec4 &swizzle, uint32_t count) {
        std::stringstream stream;

        for (uint32_t a = 0; a < count; a++) {
            stream << getSwizzleName(swizzle[a]);
        }

        return stream.str();
    }

    std::string disassembleReference(usse::RegisterReference reference) {
        return fmt::format("{}{}.{}", getBankRegisterName(reference.bank), reference.index,
            getFullSwizzleName(reference.swizzle, reference.type.components));
    }

    std::string disassemble(const std::string &name,
        const std::vector<usse::RegisterReference> &sources,
        const usse::RegisterReference *destination) {
        std::stringstream stream;

        stream << name;

        if (destination) {
            stream << " " << disassembleReference(*destination);
        }

        for (const usse::RegisterReference &source : sources) {
            stream << " " << disassembleReference(source);
        }

        return stream.str();
    }
}
