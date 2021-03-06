#pragma once

#include <cstdint>

namespace usse {
    typedef uint64_t Instruction;
    typedef uint64_t Param;

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
		Param/*6*/ src2_n);

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
		Param/*6*/ src1_n);

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
		Param/*6*/ src2_n);

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
		Param/*6*/ src1_n);

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
		Param/*6*/ src2_n);

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
		Param/*6*/ src2_n);

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
		Param/*7*/ src2_n);

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
		Param/*7*/ src2_n);

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
		Param/*7*/ src2_n);

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
		Param/*1*/ comp_sel_0_bit0);

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
		Param/*7*/ src2_n);

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
		Param/*7*/ src2_n);

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
		Param/*4*/ write_mask);

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
		Param/*7*/ src2_n);

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
		Param/*20*/ br_off);

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
		Param/*7*/ src2_n_or_exe_addr_low);

	Instruction makeNOP();

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
		Param/*8*/ src2_inc);

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
		Param/*7*/ src2_n);

	Instruction makeSPEC(
		Param/*1*/ special,
		Param/*2*/ category);

}
