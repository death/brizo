// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: modrm.h $ $Author: Death $
// $Revision: 7 $ $Date: 9/13/05 21:11 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_MODRM_H
#define BRIZO_INC_MODRM_H

#include "brizo/types.h"

namespace Brizo_DE050324 { namespace detail {
    template<byte_type Mask, byte_type Shift>
    byte_type bits(byte_type value)
    {
        return (value & Mask) >> Shift;
    }

    inline byte_type modrm_rm(byte_type value)
    {
        return bits<0x07, 0>(value);
    }

    inline byte_type modrm_op(byte_type value)
    {
        return bits<0x38, 3>(value);
    }

    inline byte_type modrm_mod(byte_type value)
    {
        return bits<0xC0, 6>(value);
    }
}}

#endif // BRIZO_INC_MODRM_H
