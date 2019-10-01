#include <gxp/instructions.h>

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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (test_bit_2 & 0b1ull) << 54u;
        inst |= (src0_comp_sel & 0b1ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (end_or_src0_bank_ext & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (move_type & 0b11ull) << 46u;
        inst |= (repeat_count & 0b11ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (move_data_type & 0b111ull) << 40u;
        inst |= (test_bit_1 & 0b1ull) << 39u;
        inst |= (src0_swiz & 0b1111ull) << 35u;
        inst |= (src0_bank_sel & 0b1ull) << 34u;
        inst |= (dest_bank_sel & 0b11ull) << 32u;
        inst |= (src1_bank_sel & 0b11ull) << 30u;
        inst |= (src2_bank_sel & 0b11ull) << 28u;
        inst |= (dest_mask & 0b1111ull) << 24u;
        inst |= (dest_n & 0b111111ull) << 18u;
        inst |= (src0_n & 0b111111ull) << 12u;
        inst |= (src1_n & 0b111111ull) << 6u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (gpi1_swiz_ext & 0b1ull) << 54u;
        inst |= 0b1ull << 53u;
        inst |= (opcode2 & 0b1ull) << 52u;
        inst |= (dest_use_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (increment_mode & 0b11ull) << 47u;
        inst |= (gpi0_abs & 0b1ull) << 46u;
        inst |= (repeat_count & 0b11ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (write_mask & 0b1111ull) << 39u;
        inst |= (src1_neg & 0b1ull) << 38u;
        inst |= (src1_abs & 0b1ull) << 37u;
        inst |= (gpi1_neg & 0b1ull) << 36u;
        inst |= (gpi1_abs & 0b1ull) << 35u;
        inst |= (gpi0_swiz_ext & 0b1ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (gpi0_n & 0b11ull) << 28u;
        inst |= (dest_n & 0b111111ull) << 22u;
        inst |= (gpi0_swiz & 0b1111ull) << 18u;
        inst |= (gpi1_swiz & 0b1111ull) << 14u;
        inst |= (gpi1_n & 0b11ull) << 12u;
        inst |= (gpi0_neg & 0b1ull) << 11u;
        inst |= (src1_swiz_ext & 0b1ull) << 10u;
        inst |= (src1_swiz & 0b1111ull) << 6u;
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
        inst |= (dat_fmt & 0b1ull) << 58u;
        inst |= (pred & 0b11ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (src0_swiz_bits2 & 0b1ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (src0_abs & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (src2_swiz & 0b111ull) << 45u;
        inst |= (src1_swiz_bit2 & 0b1ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (dest_mask & 0b1111ull) << 39u;
        inst |= (src1_mod & 0b11ull) << 37u;
        inst |= (src2_mod & 0b11ull) << 35u;
        inst |= (src0_bank & 0b1ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b111111ull) << 22u;
        inst |= (src1_swiz_bits01 & 0b11ull) << 20u;
        inst |= (src0_swiz_bits01 & 0b11ull) << 18u;
        inst |= (src0_n & 0b111111ull) << 12u;
        inst |= (src1_n & 0b111111ull) << 6u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (clip_plane_enable & 0b1ull) << 54u;
        inst |= 0b0ull << 53u;
        inst |= (opcode2 & 0b1ull) << 52u;
        inst |= (dest_use_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (increment_mode & 0b11ull) << 47u;
        inst |= (gpi0_abs & 0b1ull) << 46u;
        inst |= (repeat_count & 0b11ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (write_mask & 0b1111ull) << 39u;
        inst |= (src1_neg & 0b1ull) << 38u;
        inst |= (src1_abs & 0b1ull) << 37u;
        inst |= (clip_plane_n & 0b111ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (gpi0_n & 0b11ull) << 28u;
        inst |= (dest_n & 0b111111ull) << 22u;
        inst |= (gpi0_swiz & 0b1111ull) << 18u;
        inst |= (src1_swiz_w & 0b111ull) << 15u;
        inst |= (src1_swiz_z & 0b111ull) << 12u;
        inst |= (src1_swiz_y & 0b111ull) << 9u;
        inst |= (src1_swiz_x & 0b111ull) << 6u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (src1_swiz_10_11 & 0b11ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (src1_swiz_9 & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (src2_swiz & 0b1111ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (dest_mask & 0b1111ull) << 39u;
        inst |= (src1_mod & 0b11ull) << 37u;
        inst |= (src2_mod & 0b1ull) << 36u;
        inst |= (src1_swiz_7_8 & 0b11ull) << 34u;
        inst |= (dest_bank_sel & 0b11ull) << 32u;
        inst |= (src1_bank_sel & 0b11ull) << 30u;
        inst |= (src2_bank_sel & 0b11ull) << 28u;
        inst |= (dest_n & 0b111111ull) << 22u;
        inst |= (src1_swiz_0_6 & 0b1111111ull) << 15u;
        inst |= (op2 & 0b111ull) << 12u;
        inst |= (src1_n & 0b111111ull) << 6u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (src1_swiz_10_11 & 0b11ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (src1_swiz_9 & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (src2_swiz & 0b1111ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (dest_mask & 0b1111ull) << 39u;
        inst |= (src1_mod & 0b11ull) << 37u;
        inst |= (src2_mod & 0b1ull) << 36u;
        inst |= (src1_swiz_7_8 & 0b11ull) << 34u;
        inst |= (dest_bank_sel & 0b11ull) << 32u;
        inst |= (src1_bank_sel & 0b11ull) << 30u;
        inst |= (src2_bank_sel & 0b11ull) << 28u;
        inst |= (dest_n & 0b111111ull) << 22u;
        inst |= (src1_swiz_0_6 & 0b1111111ull) << 15u;
        inst |= (op2 & 0b111ull) << 12u;
        inst |= (src1_n & 0b111111ull) << 6u;
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
        inst |= (op1 & 0b11ull) << 59u;
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (nosched & 0b1ull) << 54u;
        inst |= (moe_expand & 0b1ull) << 53u;
        inst |= (sync_start & 0b1ull) << 52u;
        inst |= (cache_ext & 0b1ull) << 51u;
        inst |= (src0_bank_ext & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (mask_count & 0b1111ull) << 44u;
        inst |= (addr_mode & 0b11ull) << 42u;
        inst |= (mode & 0b11ull) << 40u;
        inst |= (dest_bank_primattr & 0b1ull) << 39u;
        inst |= (range_enable & 0b1ull) << 38u;
        inst |= (data_type & 0b11ull) << 36u;
        inst |= (increment_or_decrement & 0b1ull) << 35u;
        inst |= (src0_bank & 0b1ull) << 34u;
        inst |= (cache_by_pass12 & 0b1ull) << 33u;
        inst |= (drc_sel & 0b1ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (src0_n & 0b1111111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (onceonly & 0b1ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_ext & 0b1ull) << 51u;
        inst |= (src1_neg & 0b1ull) << 50u;
        inst |= (src1_ext & 0b1ull) << 49u;
        inst |= (src2_ext & 0b1ull) << 48u;
        inst |= (prec & 0b1ull) << 47u;
        inst |= (src2_vscomp & 0b1ull) << 46u;
        inst |= (rpt_count & 0b11ull) << 44u;
        inst |= (sign_test & 0b11ull) << 42u;
        inst |= (zero_test & 0b11ull) << 40u;
        inst |= (test_crcomb_and & 0b1ull) << 39u;
        inst |= (chan_cc & 0b111ull) << 36u;
        inst |= (pdst_n & 0b11ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (test_wben & 0b1ull) << 20u;
        inst |= (alu_sel & 0b11ull) << 18u;
        inst |= (alu_op & 0b1111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (onceonly & 0b1ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_ext & 0b1ull) << 51u;
        inst |= (test_flag_2 & 0b1ull) << 50u;
        inst |= (src1_ext & 0b1ull) << 49u;
        inst |= (src2_ext & 0b1ull) << 48u;
        inst |= (prec & 0b1ull) << 47u;
        inst |= (src2_vscomp & 0b1ull) << 46u;
        inst |= (rpt_count & 0b11ull) << 44u;
        inst |= (sign_test & 0b11ull) << 42u;
        inst |= (zero_test & 0b11ull) << 40u;
        inst |= (test_crcomb_and & 0b1ull) << 39u;
        inst |= (tst_mask_type & 0b11ull) << 36u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (test_wben & 0b1ull) << 20u;
        inst |= (alu_sel & 0b11ull) << 18u;
        inst |= (alu_op & 0b1111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (nosched & 0b1ull) << 54u;
        inst |= (unknown & 0b1ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (repeat_count & 0b111ull) << 44u;
        inst |= (src_fmt & 0b111ull) << 41u;
        inst |= (dest_fmt & 0b111ull) << 38u;
        inst |= (dest_mask & 0b1111ull) << 34u;
        inst |= (dest_bank_sel & 0b11ull) << 32u;
        inst |= (src1_bank_sel & 0b11ull) << 30u;
        inst |= (src2_bank_sel & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (comp_sel_3 & 0b11ull) << 19u;
        inst |= (scale & 0b1ull) << 18u;
        inst |= (comp_sel_1 & 0b11ull) << 16u;
        inst |= (comp_sel_2 & 0b11ull) << 14u;
        inst |= (src1_n & 0b111111ull) << 8u;
        inst |= (comp0_sel_bit1 & 0b1ull) << 7u;
        inst |= (src2_n & 0b111111ull) << 1u;
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
        inst |= (op1 & 0b111ull) << 59u;
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (nosched & 0b1ull) << 54u;
        inst |= (repeat_count & 0b1ull) << 53u;
        inst |= (sync_start & 0b1ull) << 52u;
        inst |= (dest_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_ext & 0b1ull) << 49u;
        inst |= (src2_ext & 0b1ull) << 48u;
        inst |= (mask_count & 0b1111ull) << 44u;
        inst |= (src2_invert & 0b1ull) << 43u;
        inst |= (src2_rot & 0b11111ull) << 38u;
        inst |= (src2_exth & 0b11ull) << 36u;
        inst |= (op2 & 0b1ull) << 35u;
        inst |= (bitwise_partial & 0b1ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (src2_sel & 0b1111111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (nosched & 0b1ull) << 54u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (minpack & 0b1ull) << 51u;
        inst |= (src0_ext & 0b1ull) << 50u;
        inst |= (src1_ext & 0b1ull) << 49u;
        inst |= (src2_ext & 0b1ull) << 48u;
        inst |= (fconv_type & 0b11ull) << 46u;
        inst |= (mask_count & 0b11ull) << 44u;
        inst |= (dim & 0b11ull) << 42u;
        inst |= (lod_mode & 0b11ull) << 40u;
        inst |= (dest_use_pa & 0b1ull) << 39u;
        inst |= (sb_mode & 0b11ull) << 37u;
        inst |= (src0_type & 0b11ull) << 35u;
        inst |= (src0_bank & 0b1ull) << 34u;
        inst |= (drc_sel & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (src0_n & 0b1111111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (dest_type & 0b11ull) << 53u;
        inst |= (syncstart & 0b1ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (repeat_count & 0b1111ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (op2 & 0b11ull) << 41u;
        inst |= (src_type & 0b11ull) << 39u;
        inst |= (src1_mod & 0b11ull) << 37u;
        inst |= (src_comp & 0b11ull) << 35u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b11ull) << 57u;
        inst |= (cmod1 & 0b1ull) << 56u;
        inst |= (skipinv & 0b1ull) << 55u;
        inst |= (nosched & 0b1ull) << 54u;
        inst |= (asel1 & 0b11ull) << 52u;
        inst |= (dest_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (cmod2 & 0b1ull) << 47u;
        inst |= (count & 0b111ull) << 44u;
        inst |= (amod1 & 0b1ull) << 43u;
        inst |= (asel2 & 0b11ull) << 41u;
        inst |= (csel1 & 0b111ull) << 38u;
        inst |= (csel2 & 0b111ull) << 35u;
        inst |= (amod2 & 0b1ull) << 34u;
        inst |= (dest_bank & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (dest_n & 0b1111111ull) << 21u;
        inst |= (src1_mod & 0b1ull) << 20u;
        inst |= (cop & 0b11ull) << 18u;
        inst |= (aop & 0b11ull) << 16u;
        inst |= (asrc1_mod & 0b1ull) << 15u;
        inst |= (dest_mod & 0b1ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
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
        inst |= (pred & 0b111ull) << 56u;
        inst |= (syncend & 0b1ull) << 55u;
        inst |= 0b0ull << 54u;
        inst |= 0b00ull << 52u;
        inst |= (exception & 0b1ull) << 51u;
        inst |= (pwait & 0b1ull) << 45u;
        inst |= (sync_ext & 0b1ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (br_monitor & 0b1ull) << 42u;
        inst |= (save_link & 0b1ull) << 41u;
        inst |= 0b00ull << 39u;
        inst |= (br_type & 0b1ull) << 38u;
        inst |= (any_inst & 0b1ull) << 21u;
        inst |= (all_inst & 0b1ull) << 20u;
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
        inst |= (sprvv & 0b1ull) << 55u;
        inst |= 0b100ull << 52u;
        inst |= (end & 0b1ull) << 51u;
        inst |= (imm & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (mode & 0b1ull) << 45u;
        inst |= (rate_hi & 0b1ull) << 44u;
        inst |= (rate_lo_or_nosched & 0b1ull) << 43u;
        inst |= (wait_cond & 0b111ull) << 40u;
        inst |= (temp_count & 0b11111111ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (exe_addr_high & 0b111111ull) << 14u;
        inst |= (src1_n_or_exe_addr_mid & 0b1111111ull) << 7u;
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
        inst |= (nosched & 0b1ull) << 50u;
        inst |= (temp_limit & 0b1111ull) << 44u;
        inst |= (pa_limit & 0b1111ull) << 40u;
        inst |= (sa_limit & 0b1111ull) << 36u;
        inst |= (dest_inc_mode & 0b1ull) << 35u;
        inst |= (src0_inc_mode & 0b1ull) << 34u;
        inst |= (src1_inc_mode & 0b1ull) << 33u;
        inst |= (src2_inc_mode & 0b1ull) << 32u;
        inst |= (dest_inc & 0b11111111ull) << 24u;
        inst |= (src0_inc & 0b11111111ull) << 16u;
        inst |= (src1_inc & 0b11111111ull) << 8u;
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
        inst |= (sideband_high & 0b11ull) << 54u;
        inst |= 0b10ull << 52u;
        inst |= (src0_bank_ext & 0b1ull) << 51u;
        inst |= (end & 0b1ull) << 50u;
        inst |= (src1_bank_ext & 0b1ull) << 49u;
        inst |= (src2_bank_ext & 0b1ull) << 48u;
        inst |= (target & 0b11ull) << 46u;
        inst |= (task_start_or_mte_hi & 0b1ull) << 45u;
        inst |= (task_end_or_mte_lo & 0b1ull) << 44u;
        inst |= (nosched & 0b1ull) << 43u;
        inst |= (sideband_mid & 0b111111ull) << 35u;
        inst |= (src0_bank & 0b1ull) << 34u;
        inst |= (incp & 0b11ull) << 32u;
        inst |= (src1_bank & 0b11ull) << 30u;
        inst |= (src2_bank & 0b11ull) << 28u;
        inst |= (sideband_low & 0b111111ull) << 22u;
        inst |= (freep & 0b1ull) << 21u;
        inst |= (src0_n & 0b1111111ull) << 14u;
        inst |= (src1_n & 0b1111111ull) << 7u;
        inst |= (src2_n & 0b1111111ull) << 0u;
        return inst;
    }

    Instruction makeSPEC(
        Param/*1*/ special,
        Param/*2*/ category) {
        Instruction inst = 0;
        inst |= 0b11111ull << 59u;
        inst |= (special & 0b1ull) << 54u;
        inst |= (category & 0b11ull) << 52u;
        return inst;
    }

}
