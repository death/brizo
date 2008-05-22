// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: decoder.h $ $Author: Death $
// $Revision: 38 $ $Date: 9/13/05 21:18 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_DECODER_H
#define BRIZO_INC_DECODER_H

#include "instruction.h"

#include "exhaustion/no_more_data.h"
#include "exhaustion/throw_exception.h"

#include "detail/modrm.h"
#include "detail/decoder_state.h"
#include "detail/prefix.h"
#include "detail/operand_value_decoder.h"

namespace Brizo_DE050324
{
    template<
        typename InputIterator,
        typename ExhaustionPolicy = throw_exception<no_more_data>,
        typename Instruction = instruction
    >
    class decoder
    {
    public:
        typedef InputIterator iterator_type;
        typedef ExhaustionPolicy exhaustion_policy_type;
        typedef Instruction instruction_type;

    private:
        typedef detail::decoder_state<
            iterator_type, 
            exhaustion_policy_type,
            instruction_type
        > decoder_state_type;

    public:
        decoder(iterator_type first, 
                iterator_type last, 
                ExhaustionPolicy exhaustion_policy = ExhaustionPolicy())
        : m_state(first, last, exhaustion_policy)
        , m_opv_decoder(m_state)
        {
        }

        iterator_type operator() (instruction_type & inst)
        {
            m_state.set_instruction(&inst);
            struct guard
            {
                decoder_state_type & state;
                ~guard() { state.set_instruction(0); }
            } g = { m_state };

            const opcode_entry & op = decode_opcode();
            m_opv_decoder(op);

            return m_state.current();
        }

    private:
        const opcode_entry & decode_opcode()
        {
            const byte_type mod11 = 0x03;

            byte_type c = byte();
            const opcode_entry *table = detail::get_opcodes();
            bool prefixtable = false;
            instruction_type & instruction = m_state.get_instruction();

            while (1) {
                const opcode_entry & entry = table[c];
                switch (entry.op) {
                    case opcode::prefix:
                        instruction.get_prefixes().push_back(c);
                        c = byte();
                        break;
                    case opcode::ptable:
                        table = entry.table;
                        c = detail::prefix_index(instruction);
                        prefixtable = true;
                        break;
                    case opcode::mrmbothtbl:
                        table = entry.table;
                        c = detail::modrm_op(modrm());
                        break;
                    case opcode::mrmfulltbl:
                        table = entry.table;
                        c = detail::modrm_op(modrm());
                        if (detail::modrm_mod(modrm()) == mod11)
                            c += 8;
                        break;
                    case opcode::sse3special:
                        table = entry.table;
                        c = detail::modrm_rm(modrm());
                        break;
                    case opcode::fputable:
                        table = entry.table;
                        c = modrm() >= 0xC0 ? modrm() - 0xC0 + 8
                                            : detail::modrm_op(modrm());
                        break;
                    case opcode::otable:
                        table = entry.table;
                        c = byte();
                        break;
                    case opcode::mrmbftbl:
                        table = entry.table;
                        c = modrm() >= 0xC0;
                        break;
                    case opcode::mrmmemtbl:
                        table = entry.table;
                        if (detail::modrm_mod(modrm()) == mod11)
                            return get_unknown_opcode();
                        c = detail::modrm_op(modrm());
                        break;
                    case opcode::mrm11tbl:
                        table = entry.table;
                        if (detail::modrm_mod(modrm()) != mod11)
                            return get_unknown_opcode();
                        c = detail::modrm_op(modrm());
                        break;
                    case opcode::unknown:
                        if (prefixtable) {
                            opcode::type op = table[detail::PTABLE_INDEX_NOPREFIX].op;
                            if (op != opcode::unknown) {
                                c = detail::PTABLE_INDEX_NOPREFIX;
                                break;
                            }
                        }
                        // Fallthrough
                    default:
                        instruction.set_opcode_entry(entry);
                        return entry;
                }
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
        decoder(const decoder &);
        decoder & operator= (const decoder &);

    private:
        decoder_state_type m_state;
        detail::operand_value_decoder<decoder_state_type> m_opv_decoder;
    };
}

#endif // BRIZO_INC_DECODER_H
