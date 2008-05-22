// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: operand_values.h $ $Author: Death $
// $Revision: 9 $ $Date: 9/15/05 2:05 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_OPERAND_VALUES_H
#define BRIZO_INC_OPERAND_VALUES_H

#include "types.h"
#include "operand.h"
#include "visitor.h"

namespace Brizo_DE050324
{
    namespace operand
    {
        class value : public visitable_base
        {
        public:
            virtual ~value() = 0;
        };

        inline value::~value()
        {
        }

        namespace detail
        {
            template<typename T>
            class basic_value : public value
            {
                BRIZO_DEFINE_VISITABLE()

            public:
                explicit basic_value(T value)
                : m_value(value)
                {
                }

                T get_value() const
                {
                    return m_value;
                }

            private:
                T m_value;
            };
        }

        class direct_address_48_v : public value
        {
            BRIZO_DEFINE_VISITABLE()

        public:
            explicit direct_address_48_v(word_type segment, dword_type address)
            : m_segment(segment)
            , m_address(address)
            {
            }

            word_type get_segment() const
            {
                return m_segment;
            }

            dword_type get_address() const
            {
                return m_address;
            }

        private:
            word_type m_segment;
            dword_type m_address;
        };

        class direct_address_32_v : public value
        {
            BRIZO_DEFINE_VISITABLE()

        public:
            explicit direct_address_32_v(word_type segment, word_type address)
            : m_segment(segment)
            , m_address(address)
            {
            }

            word_type get_segment() const
            {
                return m_segment;
            }

            word_type get_address() const
            {
                return m_address;
            }

        private:
            word_type m_segment;
            word_type m_address;
        };

        typedef detail::basic_value<reg::type> register_v;
        typedef detail::basic_value<byte_type> immediate_8_v;
        typedef detail::basic_value<word_type> immediate_16_v;
        typedef detail::basic_value<dword_type> immediate_32_v;
    }

    struct operand_entry; // forward decl. for get_general_register
}

#endif // BRIZO_INC_OPERAND_VALUES_H
