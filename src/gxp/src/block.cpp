#include <gxp/block.h>

#include <gxp/disasm.h>
#include <gxp/builder.h>
#include <gxp/instructions.h>

#include <fmt/printf.h>

namespace gxp {
    void Block::printDisassembly(const std::string &name,
        const std::vector<usse::RegisterReference> &sources,
        const usse::RegisterReference *destination) {
        if (parent.config.printDisassembly)
            fmt::print("[disasm] {}\n", usse::disasm::disassemble(name, sources, destination));
    }

    void Block::createNop() {
        usse::disasm::disassemble("nop", { });
        instructions.push_back(usse::makeNOP());
    }

    void Block::createMov(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        assert(source.type.components <= 2 && destination.type.components <= 2);

        printDisassembly("mov", { source }, &destination);
        instructions.push_back(usse::makeVMOV(
            0, // pred
            0, // skipinv
            0, // test_bit_2
            0, // src0_comp_sel
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end_or_src0_bank_ext
            srcBankLayout.extension, // src1_bank_ext
            0, // src2_bank_ext
            0, // move_type
            0, // repeat_count
            0, // nosched
            static_cast<usse::Param>(destination.type.type) & 0b111u, // move_data_type
            0, // test_bit_1
            source.getSwizzleIndex(), // src0_swiz
            0, // src0_bank_sel
            destBankLayout.number, // dest_bank_sel
            srcBankLayout.number, // src1_bank_sel
            0, // src2_bank_sel
            destination.getSwizzleMask(), // dest_mask
            destBankLayout.getIndex(destination), // dest_n
            0, // src0_n
            srcBankLayout.getIndex(source), // src1_n
            0 // src2_n
        ));
    }

    void Block::createPack(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        if (destination.type.type == usse::Type::Signed32 || destination.type.type == usse::Type::Unsigned32
            || source.type.type == usse::Type::Signed32 || source.type.type == usse::Type::Unsigned32)
            throw std::runtime_error("Cannot pack S32/U32 type.");

        usse::Param typeTable[] = {
            1, // Signed8
            4, // Signed16
            0, // Signed32 - Unsupported
            7, // Fixed10
            5, // Float16
            6, // Float32
            0, // Unsigned8
            3, // Unsigned16
            0, // Unsigned32 - Unsupported
            2, // Output8
        };

        printDisassembly("pck", { source }, &destination);
        instructions.push_back(usse::makeVPCK(
            0, // pred
            0, // skipinv
            0, // nosched
            0, // unknown
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            srcBankLayout.extension, // src2_bank_ext
            0, // repeat_count
            typeTable[static_cast<uint32_t>(source.type.type)], // src_fmt
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_fmt
            destination.getSwizzleMask(), // dest_mask
            destBankLayout.number, // dest_bank_sel
            srcBankLayout.number, // src1_bank_sel
            srcBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination, false, 7), // dest_n
            destination.type.components > 3 ? static_cast<uint32_t>(destination.swizzle[3]) & 0b11u : 0, // comp_sel_3
            false, // scale
            static_cast<uint32_t>(destination.swizzle[1]) & 0b11u, // comp_sel_1
            static_cast<uint32_t>(destination.swizzle[2]) & 0b11u, // comp_sel_2
            srcBankLayout.getIndex(source.getHalf(0)), // src1_n
            static_cast<uint32_t>(destination.swizzle[0]) & 0b10u >> 1u, // comp0_sel_bit1
            srcBankLayout.getIndex(source.getHalf(1)), // src2_n
            static_cast<uint32_t>(destination.swizzle[0]) & 0b01u // comp_sel_0_bit0
        ));
    }


    void Block::createDot(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        assert(second.bank == usse::RegisterBank::Internal);

        printDisassembly("dot", { first, second }, &destination);
        instructions.push_back(usse::makeVDP(
            0, // pred
            0, // skipinv
            0, // clip_plane_enable
            first.type.components == 4, // opcode2
            destBankLayout.extension, // dest_use_bank_ext
            0, // end
            firstBankLayout.extension, // src1_bank_ext
            3, /* Seems to be the normal value... */ // increment_mode
            0, // gpi0_abs
            0, // repeat_count
            0, // nosched
            destination.getSwizzleMask(), // write_mask
            0, // src1_neg
            0, // src1_abs
            0, // clip_plane_n
            destBankLayout.number, // dest_bank
            firstBankLayout.number, // src1_bank
            second.index, // gpi0_n
            destBankLayout.getIndex(destination), // dest_n
            second.getSwizzleIndex(), // gpi0_swiz
            first.type.components > 3 ? static_cast<usse::Param>(first.swizzle[3]) : 0, // src1_swiz_w
            static_cast<usse::Param>(first.swizzle[2]), // src1_swiz_z
            static_cast<usse::Param>(first.swizzle[1]), // src1_swiz_y
            static_cast<usse::Param>(first.swizzle[0]), // src1_swiz_x
            firstBankLayout.getIndex(first) // src1_n
            ));
    }

    void Block::createAdd(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t swizzleIndex = 0;
        uint32_t firstSwizzle = 0;
        uint8_t destMask = destination.getSwizzleMask();
        for (uint32_t a = 0; a < 4; a++) {
            if (destMask & (1u << a)) {
                usse::SwizzleChannel channel = first.swizzle[swizzleIndex++];

                // Swizzle does not have appropriate value for writing to destination.
                assert(channel != usse::SwizzleChannel::DontCare);

                firstSwizzle |= static_cast<uint32_t>(channel) << (a * 3);
            }
        }

        printDisassembly("add", { first, second }, &destination);
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Add), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    void Block::createSub(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t swizzleIndex = 0;
        uint32_t firstSwizzle = 0;
        uint8_t destMask = destination.getSwizzleMask();
        for (uint32_t a = 0; a < 4; a++) {
            if (destMask & (1u << a)) {
                usse::SwizzleChannel channel = first.swizzle[swizzleIndex++];

                // Swizzle does not have appropriate value for writing to destination.
                assert(channel != usse::SwizzleChannel::DontCare);

                firstSwizzle |= static_cast<uint32_t>(channel) << (a * 3);
            }
        }

        // First/Second sources are flipped so negative effect can be applied to src1. -x + y = y - x
        printDisassembly("sub", { first, second }, &destination);
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            secondBankLayout.extension, // src1_bank_ext
            firstBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b01, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            secondBankLayout.number, // src1_bank_sel
            firstBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Add), // op2
            secondBankLayout.getIndex(second), // src1_n
            firstBankLayout.getIndex(first) // src2_n
        ));
    }

    void Block::createMul(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        printDisassembly("mul", { first, second }, &destination);
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Multiply), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    void Block::createExp(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        usse::Param typeTable[] = {
            0, // Signed8 - Unsupported
            0, // Signed16 - Unsupported
            0, // Signed32 - Unsupported
            2, // Fixed10
            1, // Float16
            0, // Float32
            0, // Unsigned8 - Unsupported
            0, // Unsigned16 - Unsupported
            0, // Unsigned32 - Unsupported
            0, // Output8 - Unsupported
        };

        printDisassembly("exp", { source }, &destination);
        instructions.push_back(usse::makeVCOMP(
            0, // pred
            0, // skipinv
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_type
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            0, // repeat_count
            0, // nosched
            static_cast<usse::Param>(usse::InstructionVCOMPOp::Exponent), // op2
            typeTable[static_cast<uint32_t>(source.type.type)], // src_type
            0b00, // src1_mod
            static_cast<usse::Param>(source.swizzle[0]), // src_comp
            destBankLayout.number, // dest_bank
            srcBankLayout.number, // src1_bank
            destBankLayout.getIndex(destination), // dest_n
            srcBankLayout.getIndex(source), // src1_n
            destination.getSwizzleMask() // write_mask
        ));
    }

    void Block::createLog(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        usse::Param typeTable[] = {
            0, // Signed8 - Unsupported
            0, // Signed16 - Unsupported
            0, // Signed32 - Unsupported
            2, // Fixed10
            1, // Float16
            0, // Float32
            0, // Unsigned8 - Unsupported
            0, // Unsigned16 - Unsupported
            0, // Unsigned32 - Unsupported
            0, // Output8 - Unsupported
        };

        printDisassembly("log", { source }, &destination);
        instructions.push_back(usse::makeVCOMP(
            0, // pred
            0, // skipinv
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_type
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            0, // repeat_count
            0, // nosched
            static_cast<usse::Param>(usse::InstructionVCOMPOp::Logarithm), // op2
            typeTable[static_cast<uint32_t>(source.type.type)], // src_type
            0b00, // src1_mod
            static_cast<usse::Param>(source.swizzle[0]), // src_comp
            destBankLayout.number, // dest_bank
            srcBankLayout.number, // src1_bank
            destBankLayout.getIndex(destination), // dest_n
            srcBankLayout.getIndex(source), // src1_n
            destination.getSwizzleMask() // write_mask
        ));
    }

    void Block::createReverseSquareRoot(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        usse::Param typeTable[] = {
            0, // Signed8 - Unsupported
            0, // Signed16 - Unsupported
            0, // Signed32 - Unsupported
            2, // Fixed10
            1, // Float16
            0, // Float32
            0, // Unsigned8 - Unsupported
            0, // Unsigned16 - Unsupported
            0, // Unsigned32 - Unsupported
            0, // Output8 - Unsupported
        };

        printDisassembly("rsq", { source }, &destination);
        instructions.push_back(usse::makeVCOMP(
            0, // pred
            0, // skipinv
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_type
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            0, // repeat_count
            0, // nosched
            static_cast<usse::Param>(usse::InstructionVCOMPOp::ReverseSquareRoot), // op2
            typeTable[static_cast<uint32_t>(source.type.type)], // src_type
            0b00, // src1_mod
            static_cast<usse::Param>(source.swizzle[0]), // src_comp
            destBankLayout.number, // dest_bank
            srcBankLayout.number, // src1_bank
            destBankLayout.getIndex(destination, false, 7), // dest_n
            srcBankLayout.getIndex(source, false, 7), // src1_n
            destination.getSwizzleMask() // write_mask
        ));
    }

    void Block::createMin(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        printDisassembly("min", { first, second }, &destination);
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Min), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    void Block::createMax(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        printDisassembly("max", { first, second }, &destination);
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Max), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    Block::Block(gxp::Builder &parent) : parent(parent) { }
}
