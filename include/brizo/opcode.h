// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: opcode.h $ $Author: Death $
// $Revision: 5 $ $Date: 9/13/05 21:09 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_OPCODE_H
#define BRIZO_INC_OPCODE_H

namespace Brizo_DE050324
{
    #define BRIZO_GEN_OPCODE(o, t) o,

    namespace opcode
    {
        enum type
        {
            #include "detail/opcodes.h"
            count
        };
    }

    #undef BRIZO_GEN_OPCODE
}

#endif // BRIZO_INC_OPCODE_H
