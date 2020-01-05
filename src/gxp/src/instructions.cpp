#include <gxp/instructions.h>

#include <stdexcept>

namespace usse {
	Instruction makeVMOV(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ test_bit_2,
		Param/*1*/ src0_comp_sel,
		Param/*1*/ syncstart,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ end_or_src0_bank_ext,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*2*/ move_type,
		Param/*2*/ repeat_count,
		Param/*1*/ nosched,
		Param/*3*/ move_data_type,
		Param/*1*/ test_bit_1,
		Param/*4*/ src0_swiz,
		Param/*1*/ src0_bank_sel,
		Param/*2*/ dest_bank_sel,
		Param/*2*/ src1_bank_sel,
		Param/*2*/ src2_bank_sel,
		Param/*4*/ dest_mask,
		Param/*6*/ dest_n,
		Param/*6*/ src0_n,
		Param/*6*/ src1_n,
		Param/*6*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b00111ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VMOV out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VMOV out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((test_bit_2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_bit_2 for VMOV out of bounds.");
		inst |= (test_bit_2 & 0b1ull) << 54u;
		if ((src0_comp_sel & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_comp_sel for VMOV out of bounds.");
		inst |= (src0_comp_sel & 0b1ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VMOV out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for VMOV out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((end_or_src0_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end_or_src0_bank_ext for VMOV out of bounds.");
		inst |= (end_or_src0_bank_ext & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VMOV out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VMOV out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((move_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field move_type for VMOV out of bounds.");
		inst |= (move_type & 0b11ull) << 46u;
		if ((repeat_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VMOV out of bounds.");
		inst |= (repeat_count & 0b11ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VMOV out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((move_data_type & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field move_data_type for VMOV out of bounds.");
		inst |= (move_data_type & 0b111ull) << 40u;
		if ((test_bit_1 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_bit_1 for VMOV out of bounds.");
		inst |= (test_bit_1 & 0b1ull) << 39u;
		if ((src0_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field src0_swiz for VMOV out of bounds.");
		inst |= (src0_swiz & 0b1111ull) << 35u;
		if ((src0_bank_sel & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank_sel for VMOV out of bounds.");
		inst |= (src0_bank_sel & 0b1ull) << 34u;
		if ((dest_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_sel for VMOV out of bounds.");
		inst |= (dest_bank_sel & 0b11ull) << 32u;
		if ((src1_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_sel for VMOV out of bounds.");
		inst |= (src1_bank_sel & 0b11ull) << 30u;
		if ((src2_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_sel for VMOV out of bounds.");
		inst |= (src2_bank_sel & 0b11ull) << 28u;
		if ((dest_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field dest_mask for VMOV out of bounds.");
		inst |= (dest_mask & 0b1111ull) << 24u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VMOV out of bounds.");
		inst |= (dest_n & 0b111111ull) << 18u;
		if ((src0_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src0_n for VMOV out of bounds.");
		inst |= (src0_n & 0b111111ull) << 12u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VMOV out of bounds.");
		inst |= (src1_n & 0b111111ull) << 6u;
		if ((src2_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VMOV out of bounds.");
		inst |= (src2_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVMAD(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ gpi1_swiz_ext,
		Param/*1*/ opcode2,
		Param/*1*/ dest_use_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*2*/ increment_mode,
		Param/*1*/ gpi0_abs,
		Param/*2*/ repeat_count,
		Param/*1*/ nosched,
		Param/*4*/ write_mask,
		Param/*1*/ src1_neg,
		Param/*1*/ src1_abs,
		Param/*1*/ gpi1_neg,
		Param/*1*/ gpi1_abs,
		Param/*1*/ gpi0_swiz_ext,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ gpi0_n,
		Param/*6*/ dest_n,
		Param/*4*/ gpi0_swiz,
		Param/*4*/ gpi1_swiz,
		Param/*2*/ gpi1_n,
		Param/*1*/ gpi0_neg,
		Param/*1*/ src1_swiz_ext,
		Param/*4*/ src1_swiz,
		Param/*6*/ src1_n) {
		Instruction inst = 0;
		inst |= 0b00011ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VMAD out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VMAD out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((gpi1_swiz_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi1_swiz_ext for VMAD out of bounds.");
		inst |= (gpi1_swiz_ext & 0b1ull) << 54u;
		inst |= 0b1ull << 53u;
		if ((opcode2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field opcode2 for VMAD out of bounds.");
		inst |= (opcode2 & 0b1ull) << 52u;
		if ((dest_use_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_use_bank_ext for VMAD out of bounds.");
		inst |= (dest_use_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for VMAD out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VMAD out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((increment_mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field increment_mode for VMAD out of bounds.");
		inst |= (increment_mode & 0b11ull) << 47u;
		if ((gpi0_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi0_abs for VMAD out of bounds.");
		inst |= (gpi0_abs & 0b1ull) << 46u;
		if ((repeat_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VMAD out of bounds.");
		inst |= (repeat_count & 0b11ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VMAD out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((write_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field write_mask for VMAD out of bounds.");
		inst |= (write_mask & 0b1111ull) << 39u;
		if ((src1_neg & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_neg for VMAD out of bounds.");
		inst |= (src1_neg & 0b1ull) << 38u;
		if ((src1_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_abs for VMAD out of bounds.");
		inst |= (src1_abs & 0b1ull) << 37u;
		if ((gpi1_neg & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi1_neg for VMAD out of bounds.");
		inst |= (gpi1_neg & 0b1ull) << 36u;
		if ((gpi1_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi1_abs for VMAD out of bounds.");
		inst |= (gpi1_abs & 0b1ull) << 35u;
		if ((gpi0_swiz_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi0_swiz_ext for VMAD out of bounds.");
		inst |= (gpi0_swiz_ext & 0b1ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VMAD out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VMAD out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((gpi0_n & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field gpi0_n for VMAD out of bounds.");
		inst |= (gpi0_n & 0b11ull) << 28u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VMAD out of bounds.");
		inst |= (dest_n & 0b111111ull) << 22u;
		if ((gpi0_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field gpi0_swiz for VMAD out of bounds.");
		inst |= (gpi0_swiz & 0b1111ull) << 18u;
		if ((gpi1_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field gpi1_swiz for VMAD out of bounds.");
		inst |= (gpi1_swiz & 0b1111ull) << 14u;
		if ((gpi1_n & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field gpi1_n for VMAD out of bounds.");
		inst |= (gpi1_n & 0b11ull) << 12u;
		if ((gpi0_neg & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi0_neg for VMAD out of bounds.");
		inst |= (gpi0_neg & 0b1ull) << 11u;
		if ((src1_swiz_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_ext for VMAD out of bounds.");
		inst |= (src1_swiz_ext & 0b1ull) << 10u;
		if ((src1_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz for VMAD out of bounds.");
		inst |= (src1_swiz & 0b1111ull) << 6u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VMAD out of bounds.");
		inst |= (src1_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVMAD2(
		Param/*1*/ dat_fmt,
		Param/*2*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ src0_swiz_bits2,
		Param/*1*/ syncstart,
		Param/*1*/ src0_abs,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*3*/ src2_swiz,
		Param/*1*/ src1_swiz_bit2,
		Param/*1*/ nosched,
		Param/*4*/ dest_mask,
		Param/*2*/ src1_mod,
		Param/*2*/ src2_mod,
		Param/*1*/ src0_bank,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*6*/ dest_n,
		Param/*2*/ src1_swiz_bits01,
		Param/*2*/ src0_swiz_bits01,
		Param/*6*/ src0_n,
		Param/*6*/ src1_n,
		Param/*6*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b00000ull << 59u;
		if ((dat_fmt & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dat_fmt for VMAD2 out of bounds.");
		inst |= (dat_fmt & 0b1ull) << 58u;
		if ((pred & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field pred for VMAD2 out of bounds.");
		inst |= (pred & 0b11ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VMAD2 out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((src0_swiz_bits2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_swiz_bits2 for VMAD2 out of bounds.");
		inst |= (src0_swiz_bits2 & 0b1ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VMAD2 out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((src0_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_abs for VMAD2 out of bounds.");
		inst |= (src0_abs & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VMAD2 out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VMAD2 out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((src2_swiz & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src2_swiz for VMAD2 out of bounds.");
		inst |= (src2_swiz & 0b111ull) << 45u;
		if ((src1_swiz_bit2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_bit2 for VMAD2 out of bounds.");
		inst |= (src1_swiz_bit2 & 0b1ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VMAD2 out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((dest_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field dest_mask for VMAD2 out of bounds.");
		inst |= (dest_mask & 0b1111ull) << 39u;
		if ((src1_mod & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_mod for VMAD2 out of bounds.");
		inst |= (src1_mod & 0b11ull) << 37u;
		if ((src2_mod & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_mod for VMAD2 out of bounds.");
		inst |= (src2_mod & 0b11ull) << 35u;
		if ((src0_bank & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank for VMAD2 out of bounds.");
		inst |= (src0_bank & 0b1ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VMAD2 out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VMAD2 out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for VMAD2 out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VMAD2 out of bounds.");
		inst |= (dest_n & 0b111111ull) << 22u;
		if ((src1_swiz_bits01 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_bits01 for VMAD2 out of bounds.");
		inst |= (src1_swiz_bits01 & 0b11ull) << 20u;
		if ((src0_swiz_bits01 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src0_swiz_bits01 for VMAD2 out of bounds.");
		inst |= (src0_swiz_bits01 & 0b11ull) << 18u;
		if ((src0_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src0_n for VMAD2 out of bounds.");
		inst |= (src0_n & 0b111111ull) << 12u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VMAD2 out of bounds.");
		inst |= (src1_n & 0b111111ull) << 6u;
		if ((src2_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VMAD2 out of bounds.");
		inst |= (src2_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVDP(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ clip_plane_enable,
		Param/*1*/ opcode2,
		Param/*1*/ dest_use_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*2*/ increment_mode,
		Param/*1*/ gpi0_abs,
		Param/*2*/ repeat_count,
		Param/*1*/ nosched,
		Param/*4*/ write_mask,
		Param/*1*/ src1_neg,
		Param/*1*/ src1_abs,
		Param/*3*/ clip_plane_n,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ gpi0_n,
		Param/*6*/ dest_n,
		Param/*4*/ gpi0_swiz,
		Param/*3*/ src1_swiz_w,
		Param/*3*/ src1_swiz_z,
		Param/*3*/ src1_swiz_y,
		Param/*3*/ src1_swiz_x,
		Param/*6*/ src1_n) {
		Instruction inst = 0;
		inst |= 0b00011ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VDP out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VDP out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((clip_plane_enable & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field clip_plane_enable for VDP out of bounds.");
		inst |= (clip_plane_enable & 0b1ull) << 54u;
		inst |= 0b0ull << 53u;
		if ((opcode2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field opcode2 for VDP out of bounds.");
		inst |= (opcode2 & 0b1ull) << 52u;
		if ((dest_use_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_use_bank_ext for VDP out of bounds.");
		inst |= (dest_use_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for VDP out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VDP out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((increment_mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field increment_mode for VDP out of bounds.");
		inst |= (increment_mode & 0b11ull) << 47u;
		if ((gpi0_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field gpi0_abs for VDP out of bounds.");
		inst |= (gpi0_abs & 0b1ull) << 46u;
		if ((repeat_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VDP out of bounds.");
		inst |= (repeat_count & 0b11ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VDP out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((write_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field write_mask for VDP out of bounds.");
		inst |= (write_mask & 0b1111ull) << 39u;
		if ((src1_neg & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_neg for VDP out of bounds.");
		inst |= (src1_neg & 0b1ull) << 38u;
		if ((src1_abs & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_abs for VDP out of bounds.");
		inst |= (src1_abs & 0b1ull) << 37u;
		if ((clip_plane_n & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field clip_plane_n for VDP out of bounds.");
		inst |= (clip_plane_n & 0b111ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VDP out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VDP out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((gpi0_n & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field gpi0_n for VDP out of bounds.");
		inst |= (gpi0_n & 0b11ull) << 28u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VDP out of bounds.");
		inst |= (dest_n & 0b111111ull) << 22u;
		if ((gpi0_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field gpi0_swiz for VDP out of bounds.");
		inst |= (gpi0_swiz & 0b1111ull) << 18u;
		if ((src1_swiz_w & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_w for VDP out of bounds.");
		inst |= (src1_swiz_w & 0b111ull) << 15u;
		if ((src1_swiz_z & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_z for VDP out of bounds.");
		inst |= (src1_swiz_z & 0b111ull) << 12u;
		if ((src1_swiz_y & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_y for VDP out of bounds.");
		inst |= (src1_swiz_y & 0b111ull) << 9u;
		if ((src1_swiz_x & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_x for VDP out of bounds.");
		inst |= (src1_swiz_x & 0b111ull) << 6u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VDP out of bounds.");
		inst |= (src1_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVNMAD32(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*2*/ src1_swiz_10_11,
		Param/*1*/ syncstart,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ src1_swiz_9,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*4*/ src2_swiz,
		Param/*1*/ nosched,
		Param/*4*/ dest_mask,
		Param/*2*/ src1_mod,
		Param/*1*/ src2_mod,
		Param/*2*/ src1_swiz_7_8,
		Param/*2*/ dest_bank_sel,
		Param/*2*/ src1_bank_sel,
		Param/*2*/ src2_bank_sel,
		Param/*6*/ dest_n,
		Param/*7*/ src1_swiz_0_6,
		Param/*3*/ op2,
		Param/*6*/ src1_n,
		Param/*6*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b00001ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VNMAD32 out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VNMAD32 out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((src1_swiz_10_11 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_10_11 for VNMAD32 out of bounds.");
		inst |= (src1_swiz_10_11 & 0b11ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VNMAD32 out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for VNMAD32 out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((src1_swiz_9 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_9 for VNMAD32 out of bounds.");
		inst |= (src1_swiz_9 & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VNMAD32 out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VNMAD32 out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((src2_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field src2_swiz for VNMAD32 out of bounds.");
		inst |= (src2_swiz & 0b1111ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VNMAD32 out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((dest_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field dest_mask for VNMAD32 out of bounds.");
		inst |= (dest_mask & 0b1111ull) << 39u;
		if ((src1_mod & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_mod for VNMAD32 out of bounds.");
		inst |= (src1_mod & 0b11ull) << 37u;
		if ((src2_mod & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_mod for VNMAD32 out of bounds.");
		inst |= (src2_mod & 0b1ull) << 36u;
		if ((src1_swiz_7_8 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_7_8 for VNMAD32 out of bounds.");
		inst |= (src1_swiz_7_8 & 0b11ull) << 34u;
		if ((dest_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_sel for VNMAD32 out of bounds.");
		inst |= (dest_bank_sel & 0b11ull) << 32u;
		if ((src1_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_sel for VNMAD32 out of bounds.");
		inst |= (src1_bank_sel & 0b11ull) << 30u;
		if ((src2_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_sel for VNMAD32 out of bounds.");
		inst |= (src2_bank_sel & 0b11ull) << 28u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VNMAD32 out of bounds.");
		inst |= (dest_n & 0b111111ull) << 22u;
		if ((src1_swiz_0_6 & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_0_6 for VNMAD32 out of bounds.");
		inst |= (src1_swiz_0_6 & 0b1111111ull) << 15u;
		if ((op2 & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field op2 for VNMAD32 out of bounds.");
		inst |= (op2 & 0b111ull) << 12u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VNMAD32 out of bounds.");
		inst |= (src1_n & 0b111111ull) << 6u;
		if ((src2_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VNMAD32 out of bounds.");
		inst |= (src2_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVNMAD16(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*2*/ src1_swiz_10_11,
		Param/*1*/ syncstart,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ src1_swiz_9,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*4*/ src2_swiz,
		Param/*1*/ nosched,
		Param/*4*/ dest_mask,
		Param/*2*/ src1_mod,
		Param/*1*/ src2_mod,
		Param/*2*/ src1_swiz_7_8,
		Param/*2*/ dest_bank_sel,
		Param/*2*/ src1_bank_sel,
		Param/*2*/ src2_bank_sel,
		Param/*6*/ dest_n,
		Param/*7*/ src1_swiz_0_6,
		Param/*3*/ op2,
		Param/*6*/ src1_n,
		Param/*6*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b00010ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VNMAD16 out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VNMAD16 out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((src1_swiz_10_11 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_10_11 for VNMAD16 out of bounds.");
		inst |= (src1_swiz_10_11 & 0b11ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VNMAD16 out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for VNMAD16 out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((src1_swiz_9 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_9 for VNMAD16 out of bounds.");
		inst |= (src1_swiz_9 & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VNMAD16 out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VNMAD16 out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((src2_swiz & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field src2_swiz for VNMAD16 out of bounds.");
		inst |= (src2_swiz & 0b1111ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VNMAD16 out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((dest_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field dest_mask for VNMAD16 out of bounds.");
		inst |= (dest_mask & 0b1111ull) << 39u;
		if ((src1_mod & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_mod for VNMAD16 out of bounds.");
		inst |= (src1_mod & 0b11ull) << 37u;
		if ((src2_mod & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_mod for VNMAD16 out of bounds.");
		inst |= (src2_mod & 0b1ull) << 36u;
		if ((src1_swiz_7_8 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_7_8 for VNMAD16 out of bounds.");
		inst |= (src1_swiz_7_8 & 0b11ull) << 34u;
		if ((dest_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_sel for VNMAD16 out of bounds.");
		inst |= (dest_bank_sel & 0b11ull) << 32u;
		if ((src1_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_sel for VNMAD16 out of bounds.");
		inst |= (src1_bank_sel & 0b11ull) << 30u;
		if ((src2_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_sel for VNMAD16 out of bounds.");
		inst |= (src2_bank_sel & 0b11ull) << 28u;
		if ((dest_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VNMAD16 out of bounds.");
		inst |= (dest_n & 0b111111ull) << 22u;
		if ((src1_swiz_0_6 & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_swiz_0_6 for VNMAD16 out of bounds.");
		inst |= (src1_swiz_0_6 & 0b1111111ull) << 15u;
		if ((op2 & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field op2 for VNMAD16 out of bounds.");
		inst |= (op2 & 0b111ull) << 12u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VNMAD16 out of bounds.");
		inst |= (src1_n & 0b111111ull) << 6u;
		if ((src2_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VNMAD16 out of bounds.");
		inst |= (src2_n & 0b111111ull) << 0u;
		return inst;
	}

	Instruction makeVLDST(
		Param/*2*/ op1,
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ nosched,
		Param/*1*/ moe_expand,
		Param/*1*/ sync_start,
		Param/*1*/ cache_ext,
		Param/*1*/ src0_bank_ext,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*4*/ mask_count,
		Param/*2*/ addr_mode,
		Param/*2*/ mode,
		Param/*1*/ dest_bank_primattr,
		Param/*1*/ range_enable,
		Param/*2*/ data_type,
		Param/*1*/ increment_or_decrement,
		Param/*1*/ src0_bank,
		Param/*1*/ cache_by_pass12,
		Param/*1*/ drc_sel,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*7*/ src0_n,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b111ull << 61u;
		if ((op1 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field op1 for VLDST out of bounds.");
		inst |= (op1 & 0b11ull) << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VLDST out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VLDST out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VLDST out of bounds.");
		inst |= (nosched & 0b1ull) << 54u;
		if ((moe_expand & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field moe_expand for VLDST out of bounds.");
		inst |= (moe_expand & 0b1ull) << 53u;
		if ((sync_start & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field sync_start for VLDST out of bounds.");
		inst |= (sync_start & 0b1ull) << 52u;
		if ((cache_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field cache_ext for VLDST out of bounds.");
		inst |= (cache_ext & 0b1ull) << 51u;
		if ((src0_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank_ext for VLDST out of bounds.");
		inst |= (src0_bank_ext & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VLDST out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VLDST out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((mask_count & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field mask_count for VLDST out of bounds.");
		inst |= (mask_count & 0b1111ull) << 44u;
		if ((addr_mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field addr_mode for VLDST out of bounds.");
		inst |= (addr_mode & 0b11ull) << 42u;
		if ((mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field mode for VLDST out of bounds.");
		inst |= (mode & 0b11ull) << 40u;
		if ((dest_bank_primattr & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_primattr for VLDST out of bounds.");
		inst |= (dest_bank_primattr & 0b1ull) << 39u;
		if ((range_enable & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field range_enable for VLDST out of bounds.");
		inst |= (range_enable & 0b1ull) << 38u;
		if ((data_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field data_type for VLDST out of bounds.");
		inst |= (data_type & 0b11ull) << 36u;
		if ((increment_or_decrement & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field increment_or_decrement for VLDST out of bounds.");
		inst |= (increment_or_decrement & 0b1ull) << 35u;
		if ((src0_bank & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank for VLDST out of bounds.");
		inst |= (src0_bank & 0b1ull) << 34u;
		if ((cache_by_pass12 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field cache_by_pass12 for VLDST out of bounds.");
		inst |= (cache_by_pass12 & 0b1ull) << 33u;
		if ((drc_sel & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field drc_sel for VLDST out of bounds.");
		inst |= (drc_sel & 0b1ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VLDST out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for VLDST out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VLDST out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((src0_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src0_n for VLDST out of bounds.");
		inst |= (src0_n & 0b1111111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VLDST out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VLDST out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeVTST(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ onceonly,
		Param/*1*/ syncstart,
		Param/*1*/ dest_ext,
		Param/*1*/ src1_neg,
		Param/*1*/ src1_ext,
		Param/*1*/ src2_ext,
		Param/*1*/ prec,
		Param/*1*/ src2_vscomp,
		Param/*2*/ rpt_count,
		Param/*2*/ sign_test,
		Param/*2*/ zero_test,
		Param/*1*/ test_crcomb_and,
		Param/*3*/ chan_cc,
		Param/*2*/ pdst_n,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*1*/ test_wben,
		Param/*2*/ alu_sel,
		Param/*4*/ alu_op,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b01001ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VTST out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VTST out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((onceonly & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field onceonly for VTST out of bounds.");
		inst |= (onceonly & 0b1ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VTST out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_ext for VTST out of bounds.");
		inst |= (dest_ext & 0b1ull) << 51u;
		if ((src1_neg & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_neg for VTST out of bounds.");
		inst |= (src1_neg & 0b1ull) << 50u;
		if ((src1_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_ext for VTST out of bounds.");
		inst |= (src1_ext & 0b1ull) << 49u;
		if ((src2_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_ext for VTST out of bounds.");
		inst |= (src2_ext & 0b1ull) << 48u;
		if ((prec & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field prec for VTST out of bounds.");
		inst |= (prec & 0b1ull) << 47u;
		if ((src2_vscomp & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_vscomp for VTST out of bounds.");
		inst |= (src2_vscomp & 0b1ull) << 46u;
		if ((rpt_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field rpt_count for VTST out of bounds.");
		inst |= (rpt_count & 0b11ull) << 44u;
		if ((sign_test & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field sign_test for VTST out of bounds.");
		inst |= (sign_test & 0b11ull) << 42u;
		if ((zero_test & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field zero_test for VTST out of bounds.");
		inst |= (zero_test & 0b11ull) << 40u;
		if ((test_crcomb_and & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_crcomb_and for VTST out of bounds.");
		inst |= (test_crcomb_and & 0b1ull) << 39u;
		if ((chan_cc & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field chan_cc for VTST out of bounds.");
		inst |= (chan_cc & 0b111ull) << 36u;
		if ((pdst_n & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field pdst_n for VTST out of bounds.");
		inst |= (pdst_n & 0b11ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VTST out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VTST out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for VTST out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VTST out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((test_wben & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_wben for VTST out of bounds.");
		inst |= (test_wben & 0b1ull) << 20u;
		if ((alu_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field alu_sel for VTST out of bounds.");
		inst |= (alu_sel & 0b11ull) << 18u;
		if ((alu_op & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field alu_op for VTST out of bounds.");
		inst |= (alu_op & 0b1111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VTST out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VTST out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeVTSTMSK(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ onceonly,
		Param/*1*/ syncstart,
		Param/*1*/ dest_ext,
		Param/*1*/ test_flag_2,
		Param/*1*/ src1_ext,
		Param/*1*/ src2_ext,
		Param/*1*/ prec,
		Param/*1*/ src2_vscomp,
		Param/*2*/ rpt_count,
		Param/*2*/ sign_test,
		Param/*2*/ zero_test,
		Param/*1*/ test_crcomb_and,
		Param/*2*/ tst_mask_type,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*1*/ test_wben,
		Param/*2*/ alu_sel,
		Param/*4*/ alu_op,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b01111ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VTSTMSK out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VTSTMSK out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((onceonly & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field onceonly for VTSTMSK out of bounds.");
		inst |= (onceonly & 0b1ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VTSTMSK out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_ext for VTSTMSK out of bounds.");
		inst |= (dest_ext & 0b1ull) << 51u;
		if ((test_flag_2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_flag_2 for VTSTMSK out of bounds.");
		inst |= (test_flag_2 & 0b1ull) << 50u;
		if ((src1_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_ext for VTSTMSK out of bounds.");
		inst |= (src1_ext & 0b1ull) << 49u;
		if ((src2_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_ext for VTSTMSK out of bounds.");
		inst |= (src2_ext & 0b1ull) << 48u;
		if ((prec & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field prec for VTSTMSK out of bounds.");
		inst |= (prec & 0b1ull) << 47u;
		if ((src2_vscomp & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_vscomp for VTSTMSK out of bounds.");
		inst |= (src2_vscomp & 0b1ull) << 46u;
		if ((rpt_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field rpt_count for VTSTMSK out of bounds.");
		inst |= (rpt_count & 0b11ull) << 44u;
		if ((sign_test & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field sign_test for VTSTMSK out of bounds.");
		inst |= (sign_test & 0b11ull) << 42u;
		if ((zero_test & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field zero_test for VTSTMSK out of bounds.");
		inst |= (zero_test & 0b11ull) << 40u;
		if ((test_crcomb_and & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_crcomb_and for VTSTMSK out of bounds.");
		inst |= (test_crcomb_and & 0b1ull) << 39u;
		if ((tst_mask_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field tst_mask_type for VTSTMSK out of bounds.");
		inst |= (tst_mask_type & 0b11ull) << 36u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VTSTMSK out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VTSTMSK out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for VTSTMSK out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VTSTMSK out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((test_wben & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field test_wben for VTSTMSK out of bounds.");
		inst |= (test_wben & 0b1ull) << 20u;
		if ((alu_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field alu_sel for VTSTMSK out of bounds.");
		inst |= (alu_sel & 0b11ull) << 18u;
		if ((alu_op & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field alu_op for VTSTMSK out of bounds.");
		inst |= (alu_op & 0b1111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VTSTMSK out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VTSTMSK out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeVPCK(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ nosched,
		Param/*1*/ unknown,
		Param/*1*/ syncstart,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*3*/ repeat_count,
		Param/*3*/ src_fmt,
		Param/*3*/ dest_fmt,
		Param/*4*/ dest_mask,
		Param/*2*/ dest_bank_sel,
		Param/*2*/ src1_bank_sel,
		Param/*2*/ src2_bank_sel,
		Param/*7*/ dest_n,
		Param/*2*/ comp_sel_3,
		Param/*1*/ scale,
		Param/*2*/ comp_sel_1,
		Param/*2*/ comp_sel_2,
		Param/*6*/ src1_n,
		Param/*1*/ comp0_sel_bit1,
		Param/*6*/ src2_n,
		Param/*1*/ comp_sel_0_bit0) {
		Instruction inst = 0;
		inst |= 0b01000ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VPCK out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VPCK out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VPCK out of bounds.");
		inst |= (nosched & 0b1ull) << 54u;
		if ((unknown & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field unknown for VPCK out of bounds.");
		inst |= (unknown & 0b1ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VPCK out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for VPCK out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for VPCK out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VPCK out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for VPCK out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((repeat_count & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VPCK out of bounds.");
		inst |= (repeat_count & 0b111ull) << 44u;
		if ((src_fmt & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field src_fmt for VPCK out of bounds.");
		inst |= (src_fmt & 0b111ull) << 41u;
		if ((dest_fmt & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field dest_fmt for VPCK out of bounds.");
		inst |= (dest_fmt & 0b111ull) << 38u;
		if ((dest_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field dest_mask for VPCK out of bounds.");
		inst |= (dest_mask & 0b1111ull) << 34u;
		if ((dest_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_sel for VPCK out of bounds.");
		inst |= (dest_bank_sel & 0b11ull) << 32u;
		if ((src1_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_sel for VPCK out of bounds.");
		inst |= (src1_bank_sel & 0b11ull) << 30u;
		if ((src2_bank_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_sel for VPCK out of bounds.");
		inst |= (src2_bank_sel & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VPCK out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((comp_sel_3 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field comp_sel_3 for VPCK out of bounds.");
		inst |= (comp_sel_3 & 0b11ull) << 19u;
		if ((scale & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field scale for VPCK out of bounds.");
		inst |= (scale & 0b1ull) << 18u;
		if ((comp_sel_1 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field comp_sel_1 for VPCK out of bounds.");
		inst |= (comp_sel_1 & 0b11ull) << 16u;
		if ((comp_sel_2 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field comp_sel_2 for VPCK out of bounds.");
		inst |= (comp_sel_2 & 0b11ull) << 14u;
		if ((src1_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VPCK out of bounds.");
		inst |= (src1_n & 0b111111ull) << 8u;
		if ((comp0_sel_bit1 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field comp0_sel_bit1 for VPCK out of bounds.");
		inst |= (comp0_sel_bit1 & 0b1ull) << 7u;
		if ((src2_n & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VPCK out of bounds.");
		inst |= (src2_n & 0b111111ull) << 1u;
		if ((comp_sel_0_bit0 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field comp_sel_0_bit0 for VPCK out of bounds.");
		inst |= (comp_sel_0_bit0 & 0b1ull) << 0u;
		return inst;
	}

	Instruction makeVBW(
		Param/*3*/ op1,
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ nosched,
		Param/*1*/ repeat_count,
		Param/*1*/ sync_start,
		Param/*1*/ dest_ext,
		Param/*1*/ end,
		Param/*1*/ src1_ext,
		Param/*1*/ src2_ext,
		Param/*4*/ mask_count,
		Param/*1*/ src2_invert,
		Param/*5*/ src2_rot,
		Param/*2*/ src2_exth,
		Param/*1*/ op2,
		Param/*1*/ bitwise_partial,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*7*/ src2_sel,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b01ull << 62u;
		if ((op1 & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field op1 for VBW out of bounds.");
		inst |= (op1 & 0b111ull) << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VBW out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VBW out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VBW out of bounds.");
		inst |= (nosched & 0b1ull) << 54u;
		if ((repeat_count & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VBW out of bounds.");
		inst |= (repeat_count & 0b1ull) << 53u;
		if ((sync_start & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field sync_start for VBW out of bounds.");
		inst |= (sync_start & 0b1ull) << 52u;
		if ((dest_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_ext for VBW out of bounds.");
		inst |= (dest_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for VBW out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_ext for VBW out of bounds.");
		inst |= (src1_ext & 0b1ull) << 49u;
		if ((src2_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_ext for VBW out of bounds.");
		inst |= (src2_ext & 0b1ull) << 48u;
		if ((mask_count & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field mask_count for VBW out of bounds.");
		inst |= (mask_count & 0b1111ull) << 44u;
		if ((src2_invert & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_invert for VBW out of bounds.");
		inst |= (src2_invert & 0b1ull) << 43u;
		if ((src2_rot & ~0b11111ull) != 0)
			throw std::runtime_error("Instruction field src2_rot for VBW out of bounds.");
		inst |= (src2_rot & 0b11111ull) << 38u;
		if ((src2_exth & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_exth for VBW out of bounds.");
		inst |= (src2_exth & 0b11ull) << 36u;
		if ((op2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field op2 for VBW out of bounds.");
		inst |= (op2 & 0b1ull) << 35u;
		if ((bitwise_partial & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field bitwise_partial for VBW out of bounds.");
		inst |= (bitwise_partial & 0b1ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VBW out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VBW out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for VBW out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VBW out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((src2_sel & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_sel for VBW out of bounds.");
		inst |= (src2_sel & 0b1111111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VBW out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for VBW out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeSMP(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*1*/ nosched,
		Param/*1*/ syncstart,
		Param/*1*/ minpack,
		Param/*1*/ src0_ext,
		Param/*1*/ src1_ext,
		Param/*1*/ src2_ext,
		Param/*2*/ fconv_type,
		Param/*2*/ mask_count,
		Param/*2*/ dim,
		Param/*2*/ lod_mode,
		Param/*1*/ dest_use_pa,
		Param/*2*/ sb_mode,
		Param/*2*/ src0_type,
		Param/*1*/ src0_bank,
		Param/*2*/ drc_sel,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*7*/ src0_n,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b11100ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for SMP out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for SMP out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for SMP out of bounds.");
		inst |= (nosched & 0b1ull) << 54u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for SMP out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((minpack & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field minpack for SMP out of bounds.");
		inst |= (minpack & 0b1ull) << 51u;
		if ((src0_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_ext for SMP out of bounds.");
		inst |= (src0_ext & 0b1ull) << 50u;
		if ((src1_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_ext for SMP out of bounds.");
		inst |= (src1_ext & 0b1ull) << 49u;
		if ((src2_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_ext for SMP out of bounds.");
		inst |= (src2_ext & 0b1ull) << 48u;
		if ((fconv_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field fconv_type for SMP out of bounds.");
		inst |= (fconv_type & 0b11ull) << 46u;
		if ((mask_count & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field mask_count for SMP out of bounds.");
		inst |= (mask_count & 0b11ull) << 44u;
		if ((dim & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dim for SMP out of bounds.");
		inst |= (dim & 0b11ull) << 42u;
		if ((lod_mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field lod_mode for SMP out of bounds.");
		inst |= (lod_mode & 0b11ull) << 40u;
		if ((dest_use_pa & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_use_pa for SMP out of bounds.");
		inst |= (dest_use_pa & 0b1ull) << 39u;
		if ((sb_mode & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field sb_mode for SMP out of bounds.");
		inst |= (sb_mode & 0b11ull) << 37u;
		if ((src0_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src0_type for SMP out of bounds.");
		inst |= (src0_type & 0b11ull) << 35u;
		if ((src0_bank & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank for SMP out of bounds.");
		inst |= (src0_bank & 0b1ull) << 34u;
		if ((drc_sel & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field drc_sel for SMP out of bounds.");
		inst |= (drc_sel & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for SMP out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for SMP out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for SMP out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((src0_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src0_n for SMP out of bounds.");
		inst |= (src0_n & 0b1111111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for SMP out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for SMP out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeVCOMP(
		Param/*3*/ pred,
		Param/*1*/ skipinv,
		Param/*2*/ dest_type,
		Param/*1*/ syncstart,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*4*/ repeat_count,
		Param/*1*/ nosched,
		Param/*2*/ op2,
		Param/*2*/ src_type,
		Param/*2*/ src1_mod,
		Param/*2*/ src_comp,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*7*/ dest_n,
		Param/*7*/ src1_n,
		Param/*4*/ write_mask) {
		Instruction inst = 0;
		inst |= 0b00110ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for VCOMP out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for VCOMP out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((dest_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_type for VCOMP out of bounds.");
		inst |= (dest_type & 0b11ull) << 53u;
		if ((syncstart & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncstart for VCOMP out of bounds.");
		inst |= (syncstart & 0b1ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for VCOMP out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for VCOMP out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for VCOMP out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((repeat_count & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field repeat_count for VCOMP out of bounds.");
		inst |= (repeat_count & 0b1111ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for VCOMP out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((op2 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field op2 for VCOMP out of bounds.");
		inst |= (op2 & 0b11ull) << 41u;
		if ((src_type & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src_type for VCOMP out of bounds.");
		inst |= (src_type & 0b11ull) << 39u;
		if ((src1_mod & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_mod for VCOMP out of bounds.");
		inst |= (src1_mod & 0b11ull) << 37u;
		if ((src_comp & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src_comp for VCOMP out of bounds.");
		inst |= (src_comp & 0b11ull) << 35u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for VCOMP out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for VCOMP out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for VCOMP out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for VCOMP out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((write_mask & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field write_mask for VCOMP out of bounds.");
		inst |= (write_mask & 0b1111ull) << 0u;
		return inst;
	}

	Instruction makeSOP2(
		Param/*2*/ pred,
		Param/*1*/ cmod1,
		Param/*1*/ skipinv,
		Param/*1*/ nosched,
		Param/*2*/ asel1,
		Param/*1*/ dest_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*1*/ cmod2,
		Param/*3*/ count,
		Param/*1*/ amod1,
		Param/*2*/ asel2,
		Param/*3*/ csel1,
		Param/*3*/ csel2,
		Param/*1*/ amod2,
		Param/*2*/ dest_bank,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*7*/ dest_n,
		Param/*1*/ src1_mod,
		Param/*2*/ cop,
		Param/*2*/ aop,
		Param/*1*/ asrc1_mod,
		Param/*1*/ dest_mod,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b10000ull << 59u;
		if ((pred & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field pred for SOP2 out of bounds.");
		inst |= (pred & 0b11ull) << 57u;
		if ((cmod1 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field cmod1 for SOP2 out of bounds.");
		inst |= (cmod1 & 0b1ull) << 56u;
		if ((skipinv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field skipinv for SOP2 out of bounds.");
		inst |= (skipinv & 0b1ull) << 55u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for SOP2 out of bounds.");
		inst |= (nosched & 0b1ull) << 54u;
		if ((asel1 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field asel1 for SOP2 out of bounds.");
		inst |= (asel1 & 0b11ull) << 52u;
		if ((dest_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_bank_ext for SOP2 out of bounds.");
		inst |= (dest_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for SOP2 out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for SOP2 out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for SOP2 out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((cmod2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field cmod2 for SOP2 out of bounds.");
		inst |= (cmod2 & 0b1ull) << 47u;
		if ((count & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field count for SOP2 out of bounds.");
		inst |= (count & 0b111ull) << 44u;
		if ((amod1 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field amod1 for SOP2 out of bounds.");
		inst |= (amod1 & 0b1ull) << 43u;
		if ((asel2 & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field asel2 for SOP2 out of bounds.");
		inst |= (asel2 & 0b11ull) << 41u;
		if ((csel1 & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field csel1 for SOP2 out of bounds.");
		inst |= (csel1 & 0b111ull) << 38u;
		if ((csel2 & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field csel2 for SOP2 out of bounds.");
		inst |= (csel2 & 0b111ull) << 35u;
		if ((amod2 & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field amod2 for SOP2 out of bounds.");
		inst |= (amod2 & 0b1ull) << 34u;
		if ((dest_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field dest_bank for SOP2 out of bounds.");
		inst |= (dest_bank & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for SOP2 out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for SOP2 out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((dest_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field dest_n for SOP2 out of bounds.");
		inst |= (dest_n & 0b1111111ull) << 21u;
		if ((src1_mod & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_mod for SOP2 out of bounds.");
		inst |= (src1_mod & 0b1ull) << 20u;
		if ((cop & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field cop for SOP2 out of bounds.");
		inst |= (cop & 0b11ull) << 18u;
		if ((aop & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field aop for SOP2 out of bounds.");
		inst |= (aop & 0b11ull) << 16u;
		if ((asrc1_mod & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field asrc1_mod for SOP2 out of bounds.");
		inst |= (asrc1_mod & 0b1ull) << 15u;
		if ((dest_mod & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_mod for SOP2 out of bounds.");
		inst |= (dest_mod & 0b1ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for SOP2 out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for SOP2 out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeBR(
		Param/*3*/ pred,
		Param/*1*/ syncend,
		Param/*1*/ exception,
		Param/*1*/ pwait,
		Param/*1*/ sync_ext,
		Param/*1*/ nosched,
		Param/*1*/ br_monitor,
		Param/*1*/ save_link,
		Param/*1*/ br_type,
		Param/*1*/ any_inst,
		Param/*1*/ all_inst,
		Param/*20*/ br_off) {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		if ((pred & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field pred for BR out of bounds.");
		inst |= (pred & 0b111ull) << 56u;
		if ((syncend & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field syncend for BR out of bounds.");
		inst |= (syncend & 0b1ull) << 55u;
		inst |= 0b0ull << 54u;
		inst |= 0b00ull << 52u;
		if ((exception & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field exception for BR out of bounds.");
		inst |= (exception & 0b1ull) << 51u;
		if ((pwait & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field pwait for BR out of bounds.");
		inst |= (pwait & 0b1ull) << 45u;
		if ((sync_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field sync_ext for BR out of bounds.");
		inst |= (sync_ext & 0b1ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for BR out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((br_monitor & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field br_monitor for BR out of bounds.");
		inst |= (br_monitor & 0b1ull) << 42u;
		if ((save_link & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field save_link for BR out of bounds.");
		inst |= (save_link & 0b1ull) << 41u;
		inst |= 0b00ull << 39u;
		if ((br_type & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field br_type for BR out of bounds.");
		inst |= (br_type & 0b1ull) << 38u;
		if ((any_inst & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field any_inst for BR out of bounds.");
		inst |= (any_inst & 0b1ull) << 21u;
		if ((all_inst & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field all_inst for BR out of bounds.");
		inst |= (all_inst & 0b1ull) << 20u;
		if ((br_off & ~0b11111111111111111111ull) != 0)
			throw std::runtime_error("Instruction field br_off for BR out of bounds.");
		inst |= (br_off & 0b11111111111111111111ull) << 0u;
		return inst;
	}

	Instruction makePHAS(
		Param/*1*/ sprvv,
		Param/*1*/ end,
		Param/*1*/ imm,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*1*/ mode,
		Param/*1*/ rate_hi,
		Param/*1*/ rate_lo_or_nosched,
		Param/*3*/ wait_cond,
		Param/*8*/ temp_count,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*6*/ exe_addr_high,
		Param/*7*/ src1_n_or_exe_addr_mid,
		Param/*7*/ src2_n_or_exe_addr_low) {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		inst |= 0b010ull << 56u;
		if ((sprvv & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field sprvv for PHAS out of bounds.");
		inst |= (sprvv & 0b1ull) << 55u;
		inst |= 0b100ull << 52u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for PHAS out of bounds.");
		inst |= (end & 0b1ull) << 51u;
		if ((imm & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field imm for PHAS out of bounds.");
		inst |= (imm & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for PHAS out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for PHAS out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((mode & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field mode for PHAS out of bounds.");
		inst |= (mode & 0b1ull) << 45u;
		if ((rate_hi & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field rate_hi for PHAS out of bounds.");
		inst |= (rate_hi & 0b1ull) << 44u;
		if ((rate_lo_or_nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field rate_lo_or_nosched for PHAS out of bounds.");
		inst |= (rate_lo_or_nosched & 0b1ull) << 43u;
		if ((wait_cond & ~0b111ull) != 0)
			throw std::runtime_error("Instruction field wait_cond for PHAS out of bounds.");
		inst |= (wait_cond & 0b111ull) << 40u;
		if ((temp_count & ~0b11111111ull) != 0)
			throw std::runtime_error("Instruction field temp_count for PHAS out of bounds.");
		inst |= (temp_count & 0b11111111ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for PHAS out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for PHAS out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((exe_addr_high & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field exe_addr_high for PHAS out of bounds.");
		inst |= (exe_addr_high & 0b111111ull) << 14u;
		if ((src1_n_or_exe_addr_mid & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n_or_exe_addr_mid for PHAS out of bounds.");
		inst |= (src1_n_or_exe_addr_mid & 0b1111111ull) << 7u;
		if ((src2_n_or_exe_addr_low & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n_or_exe_addr_low for PHAS out of bounds.");
		inst |= (src2_n_or_exe_addr_low & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeNOP() {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		inst |= 0b0ull << 54u;
		inst |= 0b00ull << 52u;
		inst |= 0b101ull << 38u;
		return inst;
	}

	Instruction makeSMLSI(
		Param/*1*/ nosched,
		Param/*4*/ temp_limit,
		Param/*4*/ pa_limit,
		Param/*4*/ sa_limit,
		Param/*1*/ dest_inc_mode,
		Param/*1*/ src0_inc_mode,
		Param/*1*/ src1_inc_mode,
		Param/*1*/ src2_inc_mode,
		Param/*8*/ dest_inc,
		Param/*8*/ src0_inc,
		Param/*8*/ src1_inc,
		Param/*8*/ src2_inc) {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		inst |= 0b010ull << 56u;
		inst |= 0b01ull << 52u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for SMLSI out of bounds.");
		inst |= (nosched & 0b1ull) << 50u;
		if ((temp_limit & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field temp_limit for SMLSI out of bounds.");
		inst |= (temp_limit & 0b1111ull) << 44u;
		if ((pa_limit & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field pa_limit for SMLSI out of bounds.");
		inst |= (pa_limit & 0b1111ull) << 40u;
		if ((sa_limit & ~0b1111ull) != 0)
			throw std::runtime_error("Instruction field sa_limit for SMLSI out of bounds.");
		inst |= (sa_limit & 0b1111ull) << 36u;
		if ((dest_inc_mode & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field dest_inc_mode for SMLSI out of bounds.");
		inst |= (dest_inc_mode & 0b1ull) << 35u;
		if ((src0_inc_mode & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_inc_mode for SMLSI out of bounds.");
		inst |= (src0_inc_mode & 0b1ull) << 34u;
		if ((src1_inc_mode & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_inc_mode for SMLSI out of bounds.");
		inst |= (src1_inc_mode & 0b1ull) << 33u;
		if ((src2_inc_mode & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_inc_mode for SMLSI out of bounds.");
		inst |= (src2_inc_mode & 0b1ull) << 32u;
		if ((dest_inc & ~0b11111111ull) != 0)
			throw std::runtime_error("Instruction field dest_inc for SMLSI out of bounds.");
		inst |= (dest_inc & 0b11111111ull) << 24u;
		if ((src0_inc & ~0b11111111ull) != 0)
			throw std::runtime_error("Instruction field src0_inc for SMLSI out of bounds.");
		inst |= (src0_inc & 0b11111111ull) << 16u;
		if ((src1_inc & ~0b11111111ull) != 0)
			throw std::runtime_error("Instruction field src1_inc for SMLSI out of bounds.");
		inst |= (src1_inc & 0b11111111ull) << 8u;
		if ((src2_inc & ~0b11111111ull) != 0)
			throw std::runtime_error("Instruction field src2_inc for SMLSI out of bounds.");
		inst |= (src2_inc & 0b11111111ull) << 0u;
		return inst;
	}

	Instruction makeEMIT(
		Param/*2*/ sideband_high,
		Param/*1*/ src0_bank_ext,
		Param/*1*/ end,
		Param/*1*/ src1_bank_ext,
		Param/*1*/ src2_bank_ext,
		Param/*2*/ target,
		Param/*1*/ task_start_or_mte_hi,
		Param/*1*/ task_end_or_mte_lo,
		Param/*1*/ nosched,
		Param/*6*/ sideband_mid,
		Param/*1*/ src0_bank,
		Param/*2*/ incp,
		Param/*2*/ src1_bank,
		Param/*2*/ src2_bank,
		Param/*6*/ sideband_low,
		Param/*1*/ freep,
		Param/*7*/ src0_n,
		Param/*7*/ src1_n,
		Param/*7*/ src2_n) {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		inst |= 0b011ull << 56u;
		if ((sideband_high & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field sideband_high for EMIT out of bounds.");
		inst |= (sideband_high & 0b11ull) << 54u;
		inst |= 0b10ull << 52u;
		if ((src0_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank_ext for EMIT out of bounds.");
		inst |= (src0_bank_ext & 0b1ull) << 51u;
		if ((end & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field end for EMIT out of bounds.");
		inst |= (end & 0b1ull) << 50u;
		if ((src1_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src1_bank_ext for EMIT out of bounds.");
		inst |= (src1_bank_ext & 0b1ull) << 49u;
		if ((src2_bank_ext & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src2_bank_ext for EMIT out of bounds.");
		inst |= (src2_bank_ext & 0b1ull) << 48u;
		if ((target & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field target for EMIT out of bounds.");
		inst |= (target & 0b11ull) << 46u;
		if ((task_start_or_mte_hi & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field task_start_or_mte_hi for EMIT out of bounds.");
		inst |= (task_start_or_mte_hi & 0b1ull) << 45u;
		if ((task_end_or_mte_lo & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field task_end_or_mte_lo for EMIT out of bounds.");
		inst |= (task_end_or_mte_lo & 0b1ull) << 44u;
		if ((nosched & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field nosched for EMIT out of bounds.");
		inst |= (nosched & 0b1ull) << 43u;
		if ((sideband_mid & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field sideband_mid for EMIT out of bounds.");
		inst |= (sideband_mid & 0b111111ull) << 35u;
		if ((src0_bank & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field src0_bank for EMIT out of bounds.");
		inst |= (src0_bank & 0b1ull) << 34u;
		if ((incp & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field incp for EMIT out of bounds.");
		inst |= (incp & 0b11ull) << 32u;
		if ((src1_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src1_bank for EMIT out of bounds.");
		inst |= (src1_bank & 0b11ull) << 30u;
		if ((src2_bank & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field src2_bank for EMIT out of bounds.");
		inst |= (src2_bank & 0b11ull) << 28u;
		if ((sideband_low & ~0b111111ull) != 0)
			throw std::runtime_error("Instruction field sideband_low for EMIT out of bounds.");
		inst |= (sideband_low & 0b111111ull) << 22u;
		if ((freep & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field freep for EMIT out of bounds.");
		inst |= (freep & 0b1ull) << 21u;
		if ((src0_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src0_n for EMIT out of bounds.");
		inst |= (src0_n & 0b1111111ull) << 14u;
		if ((src1_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src1_n for EMIT out of bounds.");
		inst |= (src1_n & 0b1111111ull) << 7u;
		if ((src2_n & ~0b1111111ull) != 0)
			throw std::runtime_error("Instruction field src2_n for EMIT out of bounds.");
		inst |= (src2_n & 0b1111111ull) << 0u;
		return inst;
	}

	Instruction makeSPEC(
		Param/*1*/ special,
		Param/*2*/ category) {
		Instruction inst = 0;
		inst |= 0b11111ull << 59u;
		if ((special & ~0b1ull) != 0)
			throw std::runtime_error("Instruction field special for SPEC out of bounds.");
		inst |= (special & 0b1ull) << 54u;
		if ((category & ~0b11ull) != 0)
			throw std::runtime_error("Instruction field category for SPEC out of bounds.");
		inst |= (category & 0b11ull) << 52u;
		return inst;
	}

}
