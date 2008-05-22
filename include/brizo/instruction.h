// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: instruction.h $ $Author: Death $
// $Revision: 11 $ $Date: 9/15/05 2:05 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_INSTRUCTION_H
#define BRIZO_INC_INSTRUCTION_H

#include <cassert>

#include <boost/shared_ptr.hpp>

#include "tables.h"
#include "opcode.h"
#include "types.h"
#include "operand_values.h"

#include "detail/fixed_capacity_array.h"

namespace Brizo_DE050324
{
    class instruction
    {
        static const word_type invalid_modrm = -1;
        static const std::size_t max_prefixes = 4;
        static const std::size_t max_bytes = 17;
        static const std::size_t max_operands = 3;

    public:
        typedef detail::fixed_capacity_array<byte_type, max_prefixes> prefixes_type;
        typedef detail::fixed_capacity_array<byte_type, max_bytes> bytes_type;
        typedef boost::shared_ptr<operand::value> operand_value_type;
        typedef detail::fixed_capacity_array<operand_value_type, max_operands> operand_values_type;

    public:
        instruction()
        : m_entry(&get_unknown_opcode())
        , m_modrm(invalid_modrm)
        {
        }

        opcode::type get_opcode() const
        {
            return m_entry->op;
        }

        bool has_modrm() const
        {
            return m_modrm != invalid_modrm;
        }

        void set_modrm(byte_type c)
        {
            m_modrm = c;
        }

        byte_type get_modrm() const
        {
            assert(has_modrm());
            return static_cast<byte_type>(m_modrm);
        }

        prefixes_type & get_prefixes()
        {
            return m_prefixes;
        }

        const prefixes_type & get_prefixes() const
        {
            return m_prefixes;
        }

        void set_opcode_entry(const opcode_entry & entry)
        {
            m_entry = &entry;
        }

        bytes_type & get_bytes()
        {
            return m_bytes;
        }

        const bytes_type & get_bytes() const
        {
            return m_bytes;
        }

        operand_values_type & get_operand_values()
        {
            return m_oper_values;
        }

        const operand_values_type & get_operand_values() const
        {
            return m_oper_values;
        }

        void add_register_v(reg::type v)
        {
            operand_value_type value(new operand::register_v(v));
            m_oper_values.push_back(value);
        }

        void add_direct_address_48_v(word_type segment, dword_type address)
        {
            operand_value_type value(
                new operand::direct_address_48_v(segment, address));
            m_oper_values.push_back(value);
        }

        void add_direct_address_32_v(word_type segment, word_type address)
        {
            operand_value_type value(
                new operand::direct_address_32_v(segment, address));
            m_oper_values.push_back(value);
        }

        void add_immediate_8_v(byte_type v)
        {
            operand_value_type value(new operand::immediate_8_v(v));
            m_oper_values.push_back(value);
        }

        void add_immediate_16_v(word_type v)
        {
            operand_value_type value(new operand::immediate_16_v(v));
            m_oper_values.push_back(value);
        }

        void add_immediate_32_v(dword_type v)
        {
            operand_value_type value(new operand::immediate_32_v(v));
            m_oper_values.push_back(value);
        }

    private:
        const opcode_entry *m_entry;
        prefixes_type m_prefixes;
        bytes_type m_bytes;
        word_type m_modrm;
        operand_values_type m_oper_values;
    };
}

#endif // BRIZO_INC_INSTRUCTION_H
