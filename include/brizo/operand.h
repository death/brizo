// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: operand.h $ $Author: Death $
// $Revision: 4 $ $Date: 6/01/05 0:24 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_OPERAND_H
#define BRIZO_INC_OPERAND_H

namespace Brizo_DE050324
{
    namespace operand
    {
        enum type
        {
            none = 0,
            two_operands = 'a',
            abs_byte = 'b',
            byte_word = 'c',
            abs_dword = 'd',
            abs_dqword = 'dq',
            ptr_32_48 = 'p',
            mmx_reg = 'pi',
            float_128 = 'ps',
            double_128 = 'pd',
            abs_qword = 'q',
            pdesc = 's',
            double_scalar_128 = 'sd',
            scalar_128 = 'ss',
            dword_reg = 'si',
            word_dword = 'v',
            abs_word = 'w',
            // Extended operand types not defined by Intel specification
            fpu_single = 'fd',
            fpu_double = 'fq',
            fpu_extended = 'ft',
            fpu_env = 'fe',
            fpu_state = 'fs',
            fpu_bcd = 'fb'
        };
    }

    namespace amode
    {
        enum type
        {
            none = 0,
            direct_address = 'A',
            control_register = 'C',
            debug_register = 'D',
            modrm = 'E',
            eflags = 'F',
            general_register = 'G',
            immediate_data = 'I',
            relative_offset = 'J',
            modrm_memory = 'M',
            no_modrm = 'O',
            modrm_mmx = 'P',
            modrm_mmx_memory = 'Q',
            modrm_general_register = 'R',
            modrm_segment = 'S',
            modrm_test = 'T',
            modrm_xmm = 'V',
            modrm_xmm_memory = 'W',
            ds_si = 'X',
            es_di = 'Y'
        };
    }

    namespace reg
    {
        enum type
        {
            none = 0,
            al,
            ah,
            ax,
            eax,
            e_ax,
            bl,
            bh,
            bx,
            ebx,
            e_bx,
            cl,
            ch,
            cx,
            ecx,
            e_cx,
            dl,
            dh,
            dx,
            edx,
            e_dx,
            si,
            esi,
            e_si,
            di,
            edi,
            e_di,
            sp,
            esp,
            e_sp,
            bp,
            ebp,
            e_bp,
            cs,
            ds,
            es,
            fs,
            gs,
            ss,
            // Pseudo-registers
            val_1,
            // XMM registers
            xmm0,
            xmm1,
            xmm2,
            xmm3,
            xmm4,
            xmm5,
            xmm6,
            xmm7,
            // Control registers
            cr0,
            cr1,
            cr2,
            cr3,
            cr4,
            cr5,
            cr6,
            cr7,
            // Debug registers
            dr0,
            dr1,
            dr2,
            dr3,
            dr4,
            dr5,
            dr6,
            dr7,
            // Test registers
            tr0,
            tr1,
            tr2,
            tr3,
            tr4,
            tr5,
            tr6,
            tr7,
            // MMX registers
            mm0,
            mm1,
            mm2,
            mm3,
            mm4,
            mm5,
            mm6,
            mm7,
            // FPU stack
            st0,
            st1,
            st2,
            st3,
            st4,
            st5,
            st6,
            st7
        };
    }

    struct operand_entry
    {
        unsigned char r; // register
        unsigned char a; // addressing mode
        unsigned short o; // operand type
    };
}

#endif // BRIZO_INC_OPERAND_H
