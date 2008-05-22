// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: throw_exception.h $ $Author: Death $
// $Revision: 2 $ $Date: 9/12/05 0:33 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_THROW_EXCEPTION_H
#define BRIZO_INC_THROW_EXCEPTION_H

#include "types.h"

namespace Brizo_DE050324
{
    // A model of InputExhaustedPolicy
    template<typename Exception>
    struct throw_exception
    {
        static byte_type exhaustion()
        {
            throw Exception();
        }
    };
}

#endif // BRIZO_INC_THROW_EXCEPTION_H
