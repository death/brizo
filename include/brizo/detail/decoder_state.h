// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: decoder_state.h $ $Author: Death $
// $Revision: 4 $ $Date: 9/13/05 20:56 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_DECODER_STATE_H
#define BRIZO_INC_DECODER_STATE_H

#include <cassert>

#include "types.h"

namespace Brizo_DE050324 { namespace detail {
    template<
        typename InputIterator, 
        typename ExhaustionPolicy, 
        typename Instruction
    >
    class decoder_state
    {
    public:
        typedef Instruction instruction_type;
        typedef InputIterator iterator_type;
        typedef ExhaustionPolicy exhaustion_policy_type;

    public:
        decoder_state(iterator_type first,
                      iterator_type last,
                      exhaustion_policy_type exhaustion_policy)
        : m_current(first)
        , m_last(last)
        , m_exhaustion_policy(exhaustion_policy)
        , m_instruction(0)
        {
        }

        byte_type byte()
        {
            assert(m_instruction);
            byte_type result = 
                m_current == m_last ? m_exhaustion_policy.exhaustion()
                                    : *m_current++;
            m_instruction->get_bytes().push_back(result);
            return result;
        }

        byte_type modrm()
        {
            assert(m_instruction);
            if (!m_instruction->has_modrm())
                m_instruction->set_modrm(byte());
            return m_instruction->get_modrm();
        }

        void set_instruction(instruction_type *instruction)
        {
            m_instruction = instruction;
        }

        instruction_type & get_instruction()
        {
            assert(m_instruction);
            return *m_instruction;
        }

        iterator_type current() const
        {
            return m_current;
        }

    private:
        iterator_type m_current;
        iterator_type m_last;
        exhaustion_policy_type m_exhaustion_policy;
        instruction_type *m_instruction;
    };
}}

#endif // BRIZO_INC_DECODER_STATE_H
