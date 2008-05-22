// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: prefix.h $ $Author: Death $
// $Revision: 2 $ $Date: 9/13/05 21:11 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_PREFIX_H
#define BRIZO_INC_PREFIX_H

#include "brizo/types.h"

namespace Brizo_DE050324 { namespace detail {
    enum
    {
        PTABLE_INDEX_NOPREFIX,
        PTABLE_INDEX_PREFIX66,
        PTABLE_INDEX_PREFIXF2,
        PTABLE_INDEX_PREFIXF3,
        PTABLE_SIZE
    };

    // Choose a prefix and return a suitable index to a PTABLE
    // entry. We choose the first good prefix, although the choice
    // is arbitrary (I think, TODO: verify this).
    template<typename Instruction>
    byte_type prefix_index(const Instruction & inst)
    {
        typedef typename Instruction::prefixes_type P;
        typedef typename P::const_iterator I;

        for (I i = inst.get_prefixes().begin(),
                e = inst.get_prefixes().end(); i != e; ++i) {
            switch (*i) {
                case 0x66:
                    return PTABLE_INDEX_PREFIX66;
                case 0xF2:
                    return PTABLE_INDEX_PREFIXF2;
                case 0xF3:
                    return PTABLE_INDEX_PREFIXF3;
                default:
                    break;
            }
        }

        return PTABLE_INDEX_NOPREFIX;
    }
}}


#endif // BRIZO_INC_PREFIX_H
