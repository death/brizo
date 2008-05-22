// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: pad_with_zeros.h $ $Author: Death $
// $Revision: 5 $ $Date: 9/13/05 21:18 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_PAD_WITH_ZEROS_H
#define BRIZO_INC_PAD_WITH_ZEROS_H

#include "brizo/types.h"

namespace Brizo_DE050324
{
    // A model of InputExhaustedPolicy
    class pad_with_zeros
    {
    public:
        explicit pad_with_zeros(bool & flag)
        : m_flag(&flag)
        {
        }

        byte_type exhaustion()
        {
            *m_flag = true;
            return 0;
        }

    private:
        bool *m_flag;
    };
}

#endif // BRIZO_INC_PAD_WITH_ZEROS_H
