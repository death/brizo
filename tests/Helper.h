#ifndef BRIZO_TESTS_INC_HELPER_H
#define BRIZO_TESTS_INC_HELPER_H

#include <cstddef>

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include "brizo/brizo.h"
#include "brizo/instruction.h"
#include "brizo/types.h"
#include "brizo/operand.h"
#include "brizo/operand_values.h"

brizo::instruction GetInstruction(const brizo::byte_type *data, 
                                  std::size_t size);

#ifndef BRIZO_TESTS_MAX_DECODE_ARGS
    #define BRIZO_TESTS_MAX_DECODE_ARGS 7
#endif

#define BRIZO_TESTS_Decode(z, n, unused)                                    \
    brizo::instruction Decode(BOOST_PP_ENUM_PARAMS(                         \
            BOOST_PP_INC(n), brizo::byte_type x));

BOOST_PP_REPEAT(BRIZO_TESTS_MAX_DECODE_ARGS, BRIZO_TESTS_Decode, ~)

#undef BRIZO_TESTS_Decode

template<int N>
void CheckOperandRegisterValue(const brizo::instruction & inst, 
                               brizo::reg::type value)
{
    brizo::instruction::operand_value_type v = 
        inst.get_operand_values().at(N);
    typedef brizo::operand::register_v register_v;
    register_v & r = dynamic_cast<register_v &>(*v);
    CPPUNIT_ASSERT(r.get_value() == value);
}

#endif // BRIZO_TESTS_INC_HELPER_H
