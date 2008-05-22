// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: tables.h $ $Author: Death $
// $Revision: 5 $ $Date: 9/09/05 19:00 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_TABLES_H
#define BRIZO_INC_TABLES_H

#include "opcode.h"
#include "operand.h"

namespace Brizo_DE050324
{
    struct opcode_entry
    {
        opcode::type op;
        operand_entry operands[3];
        const opcode_entry *table;
    };

    const opcode_entry & get_unknown_opcode();
}

namespace Brizo_DE050324 { namespace detail {
    const opcode_entry *get_opcodes();
    const char *get_opcode_text(int idx);
}}

#endif // BRIZO_INC_TABLES_H
