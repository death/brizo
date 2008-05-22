#include "brizo/decoder.h"

#include "Helper.h"

using namespace brizo;

instruction GetInstruction(const byte_type *data, std::size_t size)
{
    decoder<const byte_type *> decode(data, data + size);
    instruction inst;
    decode(inst);
    return inst;
}

#define BRIZO_TESTS_Decode(z, n, unused)                                    \
    instruction Decode(BOOST_PP_ENUM_PARAMS(                                \
        BOOST_PP_INC(n), byte_type x))                                      \
    {                                                                       \
        const byte_type x[] = {BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n), x)};   \
        return GetInstruction(x, BOOST_PP_INC(n));                          \
    }

BOOST_PP_REPEAT(BRIZO_TESTS_MAX_DECODE_ARGS, BRIZO_TESTS_Decode, ~)

#undef BRIZO_TESTS_Decode
