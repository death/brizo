#include <cassert>

#include "brizo/types.h"
#include "brizo/operand.h"
#include "brizo/detail/modrm.h"

namespace Brizo_DE050324 { namespace detail {
    reg::type get_segment_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::es, reg::cs, reg::ss, reg::ds, 
            reg::fs, reg::gs, reg::none, reg::none
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_control_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::cr0, reg::cr1, reg::cr2, reg::cr3,
            reg::cr4, reg::cr5, reg::cr6, reg::cr7
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_debug_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::dr0, reg::dr1, reg::dr2, reg::dr3,
            reg::dr4, reg::dr5, reg::dr6, reg::dr7
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_test_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::tr0, reg::tr1, reg::tr2, reg::tr3,
            reg::tr4, reg::tr5, reg::tr6, reg::tr7
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_mmx_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::mm0, reg::mm1, reg::mm2, reg::mm3,
            reg::mm4, reg::mm5, reg::mm6, reg::mm7
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_xmm_register(byte_type index)
    {
        static const reg::type regs[] = {
            reg::xmm0, reg::xmm1, reg::xmm2, reg::xmm3,
            reg::xmm4, reg::xmm5, reg::xmm6, reg::xmm7
        };

        assert(index < 8);
        return regs[index];
    }

    reg::type get_general_register(byte_type index, 
                                   const operand_entry & oper)
    {
        static const reg::type v_regs[] = {
            reg::e_ax, reg::e_cx, reg::e_dx, reg::e_bx,
            reg::e_sp, reg::e_bp, reg::e_si, reg::e_di
        };

        static const reg::type d_regs[] = {
            reg::eax, reg::ecx, reg::edx, reg::ebx,
            reg::esp, reg::ebp, reg::esi, reg::edi
        };

        static const reg::type w_regs[] = {
            reg::ax, reg::cx, reg::dx, reg::bx,
            reg::sp, reg::bp, reg::si, reg::di
        };

        static const reg::type b_regs[] = {
            reg::al, reg::cl, reg::dl, reg::bl,
            reg::ah, reg::ch, reg::dh, reg::bh
        };

        assert(index < 8);

        switch (oper.o) {
            case operand::word_dword:
                return v_regs[index];
            case operand::abs_dword:
                return oper.a == amode::modrm_mmx_memory
                    ? get_mmx_register(index)
                    : d_regs[index];
            case operand::abs_word:
                return w_regs[index];
            case operand::abs_byte:
                return b_regs[index];
            case operand::abs_qword:
            case operand::abs_dqword:
                return oper.a == amode::modrm_mmx_memory
                    ? get_mmx_register(index)
                    : get_xmm_register(index);
            case operand::mmx_reg:
                return get_mmx_register(index);
            case operand::scalar_128:
            case operand::double_scalar_128:
            case operand::double_128:
            case operand::float_128:
                return get_xmm_register(index);
            default:
                assert(!"unexpected operand type");
                return reg::none;
        }
    }

    dword_type to_dword_type(const byte_type *bytes)
    {
        return bytes[0]
            | (bytes[1] << 8)
            | (bytes[2] << 16)
            | (bytes[3] << 24);
    }

    word_type to_word_type(const byte_type *bytes)
    {
        return bytes[0]
            | (bytes[1] << 8);
    }
}}