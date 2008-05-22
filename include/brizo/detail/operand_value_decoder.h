// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: operand_value_decoder.h $ $Author: Death $
// $Revision: 8 $ $Date: 9/15/05 2:05 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_OPERAND_VALUE_DECODER_H
#define BRIZO_INC_OPERAND_VALUE_DECODER_H

#include <algorithm>

#include <boost/bind.hpp>

#include "tables.h"

namespace Brizo_DE050324 { namespace detail {
    reg::type get_segment_register(byte_type index);
    reg::type get_control_register(byte_type index);
    reg::type get_debug_register(byte_type index);
    reg::type get_test_register(byte_type index);
    reg::type get_mmx_register(byte_type index);
    reg::type get_xmm_register(byte_type index);
    reg::type get_general_register(byte_type index, 
                                   const operand_entry & oper);
    dword_type to_dword_type(const byte_type *bytes);
    word_type to_word_type(const byte_type *bytes);

    template<typename DecoderState>
    class operand_value_decoder
    {
        typedef typename DecoderState::instruction_type instruction_type;

    public:
        operand_value_decoder(DecoderState & state)
        : m_state(state)
        {
        }

        void operator() (const opcode_entry & op)
        {
            for (int i = 0; i < 3; i++)
                decode(op.operands[i]);
        }

    private:
        void decode(const operand_entry & oper)
        {
            instruction_type & instruction = m_state.get_instruction();

            if (oper.r != reg::none) {
                reg::type r = static_cast<reg::type>(oper.r);
                instruction.add_register_v(r);
                return;
            }

            if (oper.o == operand::none)
                return;

            switch (oper.a) {
                case amode::modrm_segment:
                    instruction.add_register_v(
                        get_segment_register(modrm_op(modrm())));
                    break;
                case amode::general_register:
                    instruction.add_register_v(
                        get_general_register(modrm_op(modrm()), oper));
                    break;
                case amode::relative_offset:
                    // todo
                    break;
                case amode::immediate_data:
                    decode_immediate_data(oper);
                    break;
                case amode::no_modrm:
                    // todo
                    break;
                case amode::ds_si:
                    // todo
                    break;
                case amode::es_di:
                    // todo
                    break;
                case amode::direct_address:
                    decode_direct_address();
                    break;
                case amode::modrm_xmm:
                    instruction.add_register_v(
                        get_xmm_register(modrm_op(modrm())));
                    break;
                case amode::modrm_mmx_memory: // test non-register
                case amode::modrm_xmm_memory: // test non-register
                case amode::modrm_memory:     // test
                case amode::modrm:            // test
                    decode_modrm(oper);
                    break;
                case amode::modrm_general_register:
                    instruction.add_register_v(
                        get_general_register(modrm_rm(modrm()), oper));
                    break;
                case amode::control_register:
                    instruction.add_register_v(
                        get_control_register(modrm_op(modrm())));
                    break;
                case amode::debug_register:
                    instruction.add_register_v(
                        get_debug_register(modrm_op(modrm())));
                    break;
                case amode::modrm_test:
                    instruction.add_register_v(
                        get_test_register(modrm_op(modrm())));
                    break;
                case amode::modrm_mmx:
                    instruction.add_register_v(
                        get_mmx_register(modrm_op(modrm())));
                    break;
                case amode::eflags:
                case amode::none:
                    break;
                default:
                    assert(!"Weird addressing mode");
                    break;
            }
        }

        // HACK: Remove magic constant and ugly implementation.
        void decode_direct_address()
        {
            instruction_type & instruction = m_state.get_instruction();

            typedef typename instruction_type::prefixes_type prefixes_type;
            const prefixes_type & prefixes = instruction.get_prefixes();
            if (std::find(prefixes.begin(), prefixes.end(),
                          0x66) == prefixes.end()) {
                byte_type bytes[6];
                std::generate_n(bytes, 6,
                    boost::bind(&operand_value_decoder::byte, this));
                dword_type address = to_dword_type(bytes);
                word_type segment = to_word_type(bytes + 4);
                instruction.add_direct_address_48_v(segment, address);
            } else {
                byte_type bytes[4];
                std::generate_n(bytes, 4,
                    boost::bind(&operand_value_decoder::byte, this));
                word_type address = to_word_type(bytes);
                word_type segment = to_word_type(bytes + 2);
                instruction.add_direct_address_32_v(segment, address);
            }
        }

        void decode_immediate_data(const operand_entry & oper)
        {
            switch (oper.o) {
                case operand::abs_byte:
                    m_state.get_instruction().add_immediate_8_v(byte());
                    return;
                case operand::abs_word:
                    {
                        byte_type bytes[2];
                        std::generate_n(bytes, 2,
                            boost::bind(&operand_value_decoder::byte, this));
                        word_type word = to_word_type(bytes);
                        m_state.get_instruction().add_immediate_16_v(word);
                    }
                    return;
                case operand::word_dword:
                    {
                        // TODO: adapt to 16-bit mode
                        byte_type bytes[4];
                        std::generate_n(bytes, 4,
                            boost::bind(&operand_value_decoder::byte, this));
                        dword_type dword = to_dword_type(bytes);
                        m_state.get_instruction().add_immediate_32_v(dword);
                    }
                    return;
                default:
                    assert(!"unexpected operand type in decode_immediate_data");
                    return;
            }
        }

        void decode_modrm(const operand_entry & oper)
        {
            instruction_type & instruction = m_state.get_instruction();

            switch (modrm_mod(modrm())) {
                case 0:
                    return;
                case 1:
                    return;
                case 2:
                    return;
                case 3:
                    instruction.add_register_v(
                        get_general_register(modrm_rm(modrm()), oper));
                    return;
                default:
                    assert(!"strange modrm:mod value!?");
                    return;
            }
        }

        byte_type byte()
        {
            return m_state.byte();
        }

        byte_type modrm()
        {
            return m_state.modrm();
        }

    private:
        operand_value_decoder(const operand_value_decoder &);
        operand_value_decoder & operator= (const operand_value_decoder &);

    private:
        DecoderState & m_state;
    };
}}

#endif // BRIZO_INC_OPERAND_VALUE_DECODER_H
