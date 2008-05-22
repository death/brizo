// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: tables.cpp $ $Author: Death $
// $Revision: 27 $ $Date: 9/13/05 21:09 $
// ---------------------------------------------------------------------------
#include "brizo/tables.h"
#include "brizo/detail/prefix.h"

#define noop             reg::none, amode::none        , operand::none
#define mkop(a, o)       reg::none, amode::type(a)     , operand::type(o)
#define mkreg(r)         reg::##r , amode::none        , operand::none

#define mkprefix         prefix , {noop, noop, noop}
#define unkop            unknown, {noop, noop, noop}

#define mktable(o, p)    (o)    , {noop, noop, noop} , (p)
#define mkotable(p)      mktable(otable              , (p))
#define mkptable(p)      mktable(ptable              , (p))
#define mkmrmbothtbl(p)  mktable(mrmbothtbl          , (p))
#define mkmrmmemtbl(p)   mktable(mrmmemtbl           , (p))
#define mkmrm11tbl(p)    mktable(mrm11tbl            , (p))
#define mkmrmfulltbl(p)  mktable(mrmfulltbl          , (p))
#define mkfputable(p)    mktable(fputable            , (p))
#define mksse3special(p) mktable(sse3special         , (p))
#define mkmrmbftbl(p)    mktable(mrmbftbl            , (p))

// Note that if we want, we can drop last entries in an opcodes table
// if they are UNKOPs. Currently, I choose not to do that (it makes table
// appearance inconsistent).

using namespace Brizo_DE050324;
using namespace opcode;
using namespace detail;

namespace
{
    const opcode_entry unknown_opcode = {unkop};

    typedef const opcode_entry table[256];
    typedef const opcode_entry mrm_both_table[8];
    typedef const opcode_entry mrm_full_table[8 * 2];
    typedef const opcode_entry prefix_table[PTABLE_SIZE];
    typedef const opcode_entry fpu_table[8 + 0x100 - 0xC0];
    typedef const opcode_entry sse3_table[8];
    typedef const opcode_entry mrm_bf_table[2];
    typedef const opcode_entry mrm_mem_table[8];
    typedef const opcode_entry mrm_11_table[8];

    mrm_both_table opcodes_0F00 = {

    // NOTE: Older IA32 specification says SLDT takes Ev, new IA32
    // specification says SLDT takes Ew.
    {sldt       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 00-000
    {str        ,{mkop('E','v') ,noop          ,noop          }}, // 0F 00-001
    {lldt       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 00-010
    {ltr        ,{mkop('E','w') ,noop          ,noop          }}, // 0F 00-011
    {verr       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 00-100
    {verw       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 00-101
    {unkop                                                     }, // 0F 00-110
    {unkop                                                     }  // 0F 00-111

    };

    sse3_table opcodes_sse3_table =
    {

    {monitor    ,{noop          ,noop          ,noop          }}, // 0F 01-001 [11] (RM=000)
    {mwait      ,{noop          ,noop          ,noop          }}, // 0F 01-001 [11] (RM=001)
    {unkop                                                     }, // 0F 01-001 [11] (RM=010)
    {unkop                                                     }, // 0F 01-001 [11] (RM=011)
    {unkop                                                     }, // 0F 01-001 [11] (RM=100)
    {unkop                                                     }, // 0F 01-001 [11] (RM=101)
    {unkop                                                     }, // 0F 01-001 [11] (RM=110)
    {unkop                                                     }  // 0F 01-001 [11] (RM=111)

    };

    mrm_full_table opcodes_0F01 = {

    {sgdt       ,{mkop('M','s') ,noop          ,noop          }}, // 0F 01-000 [mem]
    {sidt       ,{mkop('M','s') ,noop          ,noop          }}, // 0F 01-001 [mem]
    {lgdt       ,{mkop('M','s') ,noop          ,noop          }}, // 0F 01-010 [mem]
    {lidt       ,{mkop('M','s') ,noop          ,noop          }}, // 0F 01-011 [mem]
    {smsw       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 01-100 [mem]
    {unkop                                                     }, // 0F 01-101 [mem]
    {lmsw       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 01-110 [mem]
    {invlpg     ,{mkop('M','b') ,noop          ,noop          }}, // 0F 01-111 [mem]

    {unkop                                                     }, // 0F 01-000 [11]
    {mksse3special(opcodes_sse3_table)                         }, // 0F 01-001 [11]
    {unkop                                                     }, // 0F 01-010 [11]
    {unkop                                                     }, // 0F 01-011 [11]
    {smsw       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 01-100 [11]
    {unkop                                                     }, // 0F 01-101 [11]
    {lmsw       ,{mkop('E','w') ,noop          ,noop          }}, // 0F 01-110 [11]
    {unkop                                                     }  // 0F 01-111 [11]

    };

    prefix_table opcodes_0F10 = {

    {movups     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 10
    {movupd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 10
    {movsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 10
    {movss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 10

    };

    prefix_table opcodes_0F11 = {

    {movups     ,{mkop('W','ps'),mkop('V','ps'),noop          }}, // 0F 11
    {movupd     ,{mkop('W','pd'),mkop('V','pd'),noop          }}, // (66) 0F 11
    // Intel IA32 Specification Volume II had a mistake concering the
    // following instruction (operands were Vsd, Wsd -> same as (F2) 0F 10)
    {movsd      ,{mkop('W','sd'),mkop('V','sd'),noop          }}, // (F2) 0F 11
    {movss      ,{mkop('W','ss'),mkop('V','ss'),noop          }}  // (F3) 0F 11

    };

    mrm_bf_table opcodes_0F12_NoPrefix = {

    // Intel IA32 specification defines the next instruction's operands as
    // 'Wq', 'Vq' but they are supposed to be 'Vq', 'Wq'
    {movlps     ,{mkop('V','q') ,mkop('M','q') ,noop          }}, // 0F 12 <= BF
    {movhlps    ,{mkop('V','ps'),mkop('V','ps'),noop          }}  // 0F 12 > BF

    };

    prefix_table opcodes_0F12 = {

    {mkmrmbftbl(opcodes_0F12_NoPrefix)                         }, // 0F 12
    {movlpd     ,{mkop('V','q') ,mkop('M','q') ,noop          }}, // (66) 0F 12
    {movddup    ,{mkop('V','q') ,mkop('W','q') ,noop          }}, // (F2) 0F 12
    {movsldup   ,{mkop('V','ps'),mkop('W','ps'),noop          }}  // (F3) 0F 12

    };

    prefix_table opcodes_0F13 = {

    {movlps     ,{mkop('M','q') ,mkop('V','q') ,noop          }}, // 0F 13
    {movlpd     ,{mkop('M','q') ,mkop('V','q') ,noop          }}, // (66) 0F 13
    // IDA interprets the following as if it were MOVDDUP?
    {unkop                                                     }, // (F2) 0F 13
    {unkop                                                     }  // (F3) 0F 13

    };

    prefix_table opcodes_0F14 = {

    {unpcklps   ,{mkop('V','ps'),mkop('W','q') ,noop          }}, // 0F 14
    {unpcklpd   ,{mkop('V','pd'),mkop('W','q') ,noop          }}, // (66) 0F 14
    {unkop                                                     }, // (F2) 0F 14
    {unkop                                                     }  // (F3) 0F 14

    };

    prefix_table opcodes_0F15 = {

    {unpckhps   ,{mkop('V','ps'),mkop('W','q') ,noop          }}, // 0F 15
    {unpckhpd   ,{mkop('V','pd'),mkop('W','q') ,noop          }}, // (66) 0F 15
    {unkop                                                     }, // (F2) 0F 15
    {unkop                                                     }  // (F3) 0F 15

    };

    mrm_bf_table opcodes_0F16_NoPrefix = {

    {movhps     ,{mkop('V','q') ,mkop('W','q') ,noop          }}, // 0F 16 <= BF
    {movlhps    ,{mkop('V','q') ,mkop('V','q') ,noop          }}  // 0F 16 > BF

    };

    mrm_bf_table opcodes_0F16_66 = {

    {movhpd     ,{mkop('V','q') ,mkop('W','q') ,noop          }}, // (66) 0F 16 <= BF
    {movlhps    ,{mkop('V','q') ,mkop('V','q') ,noop          }}  // (66) 0F 16 > BF

    };

    prefix_table opcodes_0F16 = {

    {mkmrmbftbl(opcodes_0F16_NoPrefix)                         }, // 0F 16
    {mkmrmbftbl(opcodes_0F16_66)                               }, // (66) 0F 16
    {movshdup   ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (F2) 0F 16
    {unkop                                                     }  // (F3) 0F 16

    };

    prefix_table opcodes_0F17 = {

    {movhps     ,{mkop('W','q') ,mkop('V','q') ,noop          }}, // 0F 17
    {movhpd     ,{mkop('W','q') ,mkop('V','q') ,noop          }}, // (66) 0F 17
    {unkop                                                     }, // (F2) 0F 16
    {unkop                                                     }  // (F3) 0F 16

    };

    mrm_mem_table opcodes_0F18 = {

    // NOTE: Instruction groups that have the MOD bits to set to memory addressing
    // have 'Ev' operands
    {prefetchnta,{mkop('E','v') ,noop          ,noop          }}, // 0F 18-000 [mem]
    {prefetcht0 ,{mkop('E','v') ,noop          ,noop          }}, // 0F 18-001 [mem]
    {prefetcht1 ,{mkop('E','v') ,noop          ,noop          }}, // 0F 18-010 [mem]
    {prefetcht2 ,{mkop('E','v') ,noop          ,noop          }}, // 0F 18-011 [mem]
    {unkop                                                     }, // 0F 18-100 [mem]
    {unkop                                                     }, // 0F 18-101 [mem]
    {unkop                                                     }, // 0F 18-110 [mem]
    {unkop                                                     }  // 0F 18-111 [mem]

    };

    prefix_table opcodes_0F28 = {

    {movaps     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 28
    {movapd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 28
    {unkop                                                     }, // (F2) 0F 28
    {unkop                                                     }  // (F3) 0F 28

    };

    prefix_table opcodes_0F29 = {

    {movaps     ,{mkop('W','ps'),mkop('V','ps'),noop          }}, // 0F 29
    {movapd     ,{mkop('W','pd'),mkop('V','pd'),noop          }}, // (66) 0F 29
    {unkop                                                     }, // (F2) 0F 29
    {unkop                                                     }  // (F3) 0F 29

    };

    prefix_table opcodes_0F2A = {

    {cvtpi2ps   ,{mkop('V','ps'),mkop('Q','q') ,noop          }}, // 0F 2A
    {cvtpi2pd   ,{mkop('V','pd'),mkop('Q','dq'),noop          }}, // (66) 0F 2A
    {cvtsi2sd   ,{mkop('V','sd'),mkop('E','d') ,noop          }}, // (F2) 0F 2A
    {cvtsi2ss   ,{mkop('V','ss'),mkop('E','d') ,noop          }}  // (F3) 0F 2A

    };

    prefix_table opcodes_0F2B = {

    // TODO: New specs got different operand specifications.
    {movntps    ,{mkop('W','ps'),mkop('V','ps'),noop          }}, // 0F 2B
    {movntpd    ,{mkop('W','pd'),mkop('V','pd'),noop          }}, // (66) 0F 2B
    {unkop                                                     }, // (F2) 0F 2B
    {unkop                                                     }  // (F3) 0F 2B

    };

    prefix_table opcodes_0F2C = {

    // TODO: Operand specifications grabbed from new specs. Test.
    {cvttps2pi  ,{mkop('P','q') ,mkop('W','q') ,noop          }}, // 0F 2C
    {cvttpd2pi  ,{mkop('P','q') ,mkop('W','pd'),noop          }}, // (66) 0F 2C
    {cvttsd2si  ,{mkop('G','d') ,mkop('W','sd'),noop          }}, // (F2) 0F 2C
    {cvttss2si  ,{mkop('G','d') ,mkop('W','ss'),noop          }}  // (F3) 0F 2C

    };

    prefix_table opcodes_0F2D = {

    // TODO: Operand specifications grabbed from new specs. Test.
    {cvtps2pi   ,{mkop('P','q') ,mkop('W','q') ,noop          }}, // 0F 2D
    {cvtpd2pi   ,{mkop('P','q') ,mkop('W','pd'),noop          }}, // (66) 0F 2D
    {cvtsd2si   ,{mkop('G','d') ,mkop('W','sd'),noop          }}, // (F2) 0F 2D
    {cvtss2si   ,{mkop('G','d') ,mkop('W','ss'),noop          }}  // (F3) 0F 2D

    };

    prefix_table opcodes_0F2E = {

    {ucomiss    ,{mkop('V','ss'),mkop('W','ss'),noop          }}, // 0F 2E
    {ucomisd    ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (66) 0F 2E
    {unkop                                                     }, // (F2) 0F 2E
    {unkop                                                     }  // (F3) 0F 2E

    };

    prefix_table opcodes_0F2F = {

    {comiss     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 2F
    {comisd     ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (66) 0F 2F
    {unkop                                                     }, // (F2) 0F 2F
    {unkop                                                     }  // (F3) 0F 2F

    };

    prefix_table opcodes_0F50 = {

    {movmskps   ,{mkop('G','d') ,mkop('V','ps'),noop          }}, // 0F 50
    {movmskpd   ,{mkop('G','d') ,mkop('V','pd'),noop          }}, // (66) 0F 50
    {unkop                                                     }, // (F2) 0F 50
    {unkop                                                     }  // (F3) 0F 50

    };

    prefix_table opcodes_0F51 = {

    {sqrtps     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 51
    {sqrtpd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 51
    {sqrtsd     ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 51
    {sqrtss     ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 51

    };

    prefix_table opcodes_0F52 = {

    {rsqrtps    ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 52
    {unkop                                                     }, // (66) 0F 52
    {unkop                                                     }, // (F2) 0F 52
    {rsqrtss    ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 52

    };

    prefix_table opcodes_0F53 = {

    {rcpps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 53
    {unkop                                                     }, // (66) 0F 53
    {unkop                                                     }, // (F2) 0F 53
    {rcpss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 53

    };

    prefix_table opcodes_0F54 = {

    {andps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 54
    {andpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 54
    {unkop                                                     }, // (F2) 0F 54
    {unkop                                                     }  // (F3) 0F 54

    };

    prefix_table opcodes_0F55 = {

    {andnps     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 55
    {andnpd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 55
    {unkop                                                     }, // (F2) 0F 55
    {unkop                                                     }  // (F3) 0F 55

    };

    prefix_table opcodes_0F56 = {

    {orps       ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 56
    {orpd       ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 56
    {unkop                                                     }, // (F2) 0F 56
    {unkop                                                     }  // (F3) 0F 56

    };

    prefix_table opcodes_0F57 = {

    {xorps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 57
    {xorpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 57
    {unkop                                                     }, // (F2) 0F 57
    {unkop                                                     }  // (F3) 0F 57

    };

    prefix_table opcodes_0F58 = {

    {addps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 58
    {addpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 58
    {addsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 58
    {addss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 58

    };

    prefix_table opcodes_0F59 = {

    {mulps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 59
    {mulpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 59
    {mulsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 59
    {mulss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 59

    };

    prefix_table opcodes_0F5A = {

    {cvtps2pd   ,{mkop('V','pd'),mkop('W','ps'),noop          }}, // 0F 5A
    {cvtpd2ps   ,{mkop('V','ps'),mkop('W','pd'),noop          }}, // (66) 0F 5A
    {cvtsd2ss   ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 5A
    {cvtss2sd   ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 5A

    };

    prefix_table opcodes_0F5B = {

    {cvtdq2ps   ,{mkop('V','ps'),mkop('W','dq'),noop          }}, // 0F 5B
    {cvtps2dq   ,{mkop('V','dq'),mkop('W','ps'),noop          }}, // (66) 0F 5B
    {unkop                                                     }, // (F2) 0F 5B
    {cvttps2dq  ,{mkop('V','dq'),mkop('W','ps'),noop          }}  // (F3) 0F 5B

    };

    prefix_table opcodes_0F5C = {

    {subps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 5C
    {subpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 5C
    {subsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 5C
    {subss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 5C

    };

    prefix_table opcodes_0F5D = {

    {minps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 5D
    {minpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 5D
    {minsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 5D
    {minss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 5D

    };

    prefix_table opcodes_0F5E = {

    {divps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 5E
    {divpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 5E
    {divsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 5E
    {divss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 5E

    };

    prefix_table opcodes_0F5F = {

    {maxps      ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // 0F 5F
    {maxpd      ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 5F
    {maxsd      ,{mkop('V','sd'),mkop('W','sd'),noop          }}, // (F2) 0F 5F
    {maxss      ,{mkop('V','ss'),mkop('W','ss'),noop          }}  // (F3) 0F 5F

    };

    prefix_table opcodes_0F60 = {

    {punpcklbw  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 60
    {punpcklbw  ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 60
    {unkop                                                     }, // (F2) 0F 60
    {unkop                                                     }  // (F3) 0F 60

    };

    prefix_table opcodes_0F61 = {

    {punpcklwd  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 61
    {punpcklwd  ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 61
    {unkop                                                     }, // (F2) 0F 61
    {unkop                                                     }  // (F3) 0F 61

    };

    prefix_table opcodes_0F62 = {

    {punpckldq  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 62
    {punpckldq  ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 62
    {unkop                                                     }, // (F2) 0F 62
    {unkop                                                     }  // (F3) 0F 62

    };

    prefix_table opcodes_0F63 = {

    {packsswb   ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 63
    {packsswb   ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 63
    {unkop                                                     }, // (F2) 0F 63
    {unkop                                                     }  // (F3) 0F 63

    };

    prefix_table opcodes_0F64 = {

    {pcmpgtb    ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 64
    {pcmpgtb    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 64
    {unkop                                                     }, // (F2) 0F 64
    {unkop                                                     }  // (F3) 0F 64

    };

    prefix_table opcodes_0F65 = {

    {pcmpgtw    ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 65
    {pcmpgtw    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 65
    {unkop                                                     }, // (F2) 0F 65
    {unkop                                                     }  // (F3) 0F 65

    };

    prefix_table opcodes_0F66 = {

    {pcmpgtd    ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 66
    {pcmpgtd    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 66
    {unkop                                                     }, // (F2) 0F 66
    {unkop                                                     }  // (F3) 0F 66

    };

    prefix_table opcodes_0F67 = {

    {packuswb   ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 67
    {packuswb   ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 67
    {unkop                                                     }, // (F2) 0F 67
    {unkop                                                     }  // (F3) 0F 67

    };

    prefix_table opcodes_0F68 = {

    {punpckhbw  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 68
    {punpckhbw  ,{mkop('P','dq'),mkop('Q','dq'),noop          }}, // (66) 0F 68
    {unkop                                                     }, // (F2) 0F 68
    {unkop                                                     }  // (F3) 0F 68

    };

    prefix_table opcodes_0F69 = {

    {punpckhwd  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 69
    {punpckhwd  ,{mkop('P','dq'),mkop('Q','dq'),noop          }}, // (66) 0F 69
    {unkop                                                     }, // (F2) 0F 69
    {unkop                                                     }  // (F3) 0F 69

    };

    prefix_table opcodes_0F6A = {

    {punpckhdq  ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 6A
    {punpckhdq  ,{mkop('P','dq'),mkop('Q','dq'),noop          }}, // (66) 0F 6A
    {unkop                                                     }, // (F2) 0F 6A
    {unkop                                                     }  // (F3) 0F 6A

    };

    prefix_table opcodes_0F6B = {

    {packssdw   ,{mkop('P','q') ,mkop('Q','d') ,noop          }}, // 0F 6B
    {packssdw   ,{mkop('P','dq'),mkop('Q','dq'),noop          }}, // (66) 0F 6B
    {unkop                                                     }, // (F2) 0F 6B
    {unkop                                                     }  // (F3) 0F 6B

    };

    prefix_table opcodes_0F6C = {

    {unkop                                                     }, // 0F 6C
    {punpcklqdq ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 6C
    {unkop                                                     }, // (F2) 0F 6C
    {unkop                                                     }  // (F3) 0F 6C

    };

    prefix_table opcodes_0F6D = {

    {unkop                                                     }, // 0F 6D
    {punpckhqdq ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 6D
    {unkop                                                     }, // (F2) 0F 6D
    {unkop                                                     }  // (F3) 0F 6D

    };

    prefix_table opcodes_0F6E = {

    {movd       ,{mkop('P','d') ,mkop('E','d') ,noop          }}, // 0F 6E
    {movd       ,{mkop('V','dq'),mkop('E','d') ,noop          }}, // (66) 0F 6E
    {unkop                                                     }, // (F2) 0F 6E
    {unkop                                                     }  // (F3) 0F 6E

    };

    prefix_table opcodes_0F6F = {

    {movq       ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F 6F
    {movdqa     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 6F
    {unkop                                                     }, // (F2) 0F 6F
    {movdqu     ,{mkop('V','dq'),mkop('W','dq'),noop          }}  // (F3) 0F 6F

    };

    prefix_table opcodes_0F70 = {

    {pshufw     ,{mkop('P','q') ,mkop('Q','q') ,mkop('I','b') }}, // 0F 70
    {pshufd     ,{mkop('V','dq'),mkop('W','dq'),mkop('I','b') }}, // (66) 0F 70
    {pshuflw    ,{mkop('V','dq'),mkop('W','dq'),mkop('I','b') }}, // (72) 0F 70
    {pshufhw    ,{mkop('V','dq'),mkop('W','dq'),mkop('I','b') }}  // (73) 0F 70

    };

    mrm_11_table opcodes_0F71_Mod_11 = {

    {unkop                                                     }, // 0F 71-000 [11]
    {unkop                                                     }, // 0F 71-001 [11]
    {psrlw      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 71-010 [11]
    {unkop                                                     }, // 0F 71-011 [11]
    {psraw      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 71-100 [11]
    {unkop                                                     }, // 0F 71-101 [11]
    {psllw      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 71-110 [11]
    {unkop                                                     }  // 0F 71-111 [11]

    };

    mrm_11_table opcodes_0F71_66_Mod_11 = {

    {unkop                                                     }, // (66) 0F 71-000 [11]
    {unkop                                                     }, // (66) 0F 71-001 [11]
    {psrlw      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 71-010 [11]
    {unkop                                                     }, // (66) 0F 71-011 [11]
    {psraw      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 71-100 [11]
    {unkop                                                     }, // (66) 0F 71-101 [11]
    {psllw      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 71-110 [11]
    {unkop                                                     }  // (66) 0F 71-111 [11]

    };

    prefix_table opcodes_0F71 = {

    {mkmrm11tbl(opcodes_0F71_Mod_11)                           }, // 0F 71
    {mkmrm11tbl(opcodes_0F71_66_Mod_11)                        }, // (66) 0F 71
    {unkop                                                     }, // (F2) 0F 71
    {unkop                                                     }  // (F3) 0F 71

    };

    mrm_11_table opcodes_0F72_Mod_11 = {

    {unkop                                                     }, // 0F 72-000 [11]
    {unkop                                                     }, // 0F 72-001 [11]
    {psrld      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 72-010 [11]
    {unkop                                                     }, // 0F 72-011 [11]
    {psrad      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 72-100 [11]
    {unkop                                                     }, // 0F 72-101 [11]
    {pslld      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 72-110 [11]
    {unkop                                                     }  // 0F 72-111 [11]

    };

    // NOTE: Intel specification says the next several instructions have
    // 'Wdq' as first operand, however HIEW/IDA confirm the usage of 'Pdq'
    // operand type (like (66) 0F 71)

    mrm_11_table opcodes_0F72_66_Mod_11 = {

    {unkop                                                     }, // (66) 0F 72-000 [11]
    {unkop                                                     }, // (66) 0F 72-001 [11]
    {psrld      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 72-010 [11]
    {unkop                                                     }, // (66) 0F 72-011 [11]
    {psrad      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 72-100 [11]
    {unkop                                                     }, // (66) 0F 72-101 [11]
    {pslld      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 72-110 [11]
    {unkop                                                     }  // (66) 0F 72-111 [11]

    };

    prefix_table opcodes_0F72 = {

    {mkmrm11tbl(opcodes_0F72_Mod_11)                           }, // 0F 72
    {mkmrm11tbl(opcodes_0F72_66_Mod_11)                        }, // (66) 0F 72
    {unkop                                                     }, // (F2) 0F 72
    {unkop                                                     }  // (F3) 0F 72

    };

    mrm_11_table opcodes_0F73_Mod_11 = {

    {unkop                                                     }, // 0F 73-000 [11]
    {unkop                                                     }, // 0F 73-001 [11]
    {psrlq      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 73-010 [11]
    {unkop                                                     }, // 0F 73-011 [11]
    {unkop                                                     }, // 0F 73-100 [11]
    {unkop                                                     }, // 0F 73-101 [11]
    {psllq      ,{mkop('P','q') ,mkop('I','b') ,noop          }}, // 0F 73-110 [11]
    {unkop                                                     }  // 0F 73-111 [11]

    };

    mrm_11_table opcodes_0F73_66_Mod_11 = {

    {unkop                                                     }, // (66) 0F 73-000 [11]
    {unkop                                                     }, // (66) 0F 73-001 [11]
    {psrlq      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 73-010 [11]
    {psrldq     ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 73-011 [11]
    {unkop                                                     }, // (66) 0F 73-100 [11]
    {unkop                                                     }, // (66) 0F 73-101 [11]
    {psllq      ,{mkop('P','dq'),mkop('I','b') ,noop          }}, // (66) 0F 73-110 [11]
    {pslldq     ,{mkop('P','dq'),mkop('I','b') ,noop          }}  // (66) 0F 73-111 [11]

    };

    prefix_table opcodes_0F73 = {

    {mkmrm11tbl(opcodes_0F73_Mod_11)                           }, // 0F 73
    {mkmrm11tbl(opcodes_0F73_66_Mod_11)                        }, // (66) 0F 73
    {unkop                                                     }, // (F2) 0F 73
    {unkop                                                     }  // (F3) 0F 73

    };

    prefix_table opcodes_0F74 = {

    {pcmpeqb    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F 74
    {pcmpeqb    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 74
    {unkop                                                     }, // (F2) 0F 74
    {unkop                                                     }  // (F3) 0F 74

    };

    prefix_table opcodes_0F75 = {

    {pcmpeqw    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F 75
    {pcmpeqw    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 75
    {unkop                                                     }, // (F2) 0F 75
    {unkop                                                     }  // (F3) 0F 75

    };

    prefix_table opcodes_0F76 = {

    {pcmpeqd    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F 76
    {pcmpeqd    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F 76
    {unkop                                                     }, // (F2) 0F 76
    {unkop                                                     }  // (F3) 0F 76

    };

    prefix_table opcodes_0F7C = {

    {unkop                                                     }, // 0F 7C
    {haddpd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 7C
    {haddps     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // (F2) 0F 7C
    {unkop                                                     }  // (F3) 0F 7C

    };

    prefix_table opcodes_0F7D = {

    {unkop                                                     }, // 0F 7D
    {hsubpd     ,{mkop('V','pd'),mkop('W','pd'),noop          }}, // (66) 0F 7D
    {hsubps     ,{mkop('V','ps'),mkop('W','ps'),noop          }}, // (F2) 0F 7D
    {unkop                                                     }  // (F3) 0F 7D

    };

    prefix_table opcodes_0F7E = {

    {movd       ,{mkop('E','d') ,mkop('P','d') ,noop          }}, // 0F 7E
    {movd       ,{mkop('E','d') ,mkop('V','d') ,noop          }}, // (66) 0F 7E
    {unkop                                                     }, // (F2) 0F 7E
    {movq       ,{mkop('V','q') ,mkop('W','q') ,noop          }}  // (F3) 0F 7E

    };

    prefix_table opcodes_0F7F = {

    {movq       ,{mkop('Q','q') ,mkop('P','q') ,noop          }}, // 0F 7F
    {movdqa     ,{mkop('W','dq'),mkop('V','dq'),noop          }}, // (66) 0F 7F
    {unkop                                                     }, // (F2) 0F 7F
    {movdqu     ,{mkop('W','dq'),mkop('V','dq'),noop          }}  // (F3) 0F 7F

    };

    mrm_full_table opcodes_0FAE = {

    {fxsave     ,{mkop('E','v') ,noop          ,noop          }}, // 0F AE-000 [mem]
    {fxrstor    ,{mkop('E','v') ,noop          ,noop          }}, // 0F AE-001 [mem]
    {ldmxcsr    ,{mkop('E','v') ,noop          ,noop          }}, // 0F AE-010 [mem]
    {stmxcsr    ,{mkop('E','v') ,noop          ,noop          }}, // 0F AE-011 [mem]
    {unkop                                                     }, // 0F AE-100 [mem]
    {unkop                                                     }, // 0F AE-101 [mem]
    {unkop                                                     }, // 0F AE-110 [mem]
    {clflush    ,{mkop('E','v') ,noop          ,noop          }}, // 0F AE-111 [mem]
    {unkop                                                     }, // 0F AE-000 [11]
    {unkop                                                     }, // 0F AE-001 [11]
    {unkop                                                     }, // 0F AE-010 [11]
    {unkop                                                     }, // 0F AE-011 [11]
    {unkop                                                     }, // 0F AE-100 [11]
    {lfence     ,{noop          ,noop          ,noop          }}, // 0F AE-101 [11]
    {mfence     ,{noop          ,noop          ,noop          }}, // 0F AE-110 [11]
    {sfence     ,{noop          ,noop          ,noop          }}  // 0F AE-111 [11]

    };

    mrm_both_table opcodes_0FBA = {

    {unkop                                                     }, // 0F BA-000
    {unkop                                                     }, // 0F BA-001
    {unkop                                                     }, // 0F BA-010
    {unkop                                                     }, // 0F BA-011
    {bt         ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 0F BA-100
    {bts        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 0F BA-101
    {btr        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 0F BA-110
    {btc        ,{mkop('E','v') ,mkop('I','b') ,noop          }}  // 0F BA-111

    };

    prefix_table opcodes_0FC2 =  {

    // NOTE: HIEW doesn't correctly disassemble the following instructions;
    // IDA does.
    {cmpps      ,{mkop('V','ps'),mkop('W','ps'),mkop('I','b') }}, // 0F C2
    {cmppd      ,{mkop('V','pd'),mkop('W','pd'),mkop('I','b') }}, // (66) 0F C2
    {cmpsd      ,{mkop('V','sd'),mkop('W','sd'),mkop('I','b') }}, // (F2) 0F C2
    {cmpss      ,{mkop('V','ss'),mkop('W','ss'),mkop('I','b') }}  // (F3) 0F C2

        };

    prefix_table opcodes_0FC4 = {

    {pinsrw     ,{mkop('P','q') ,mkop('E','d') ,mkop('I','b') }}, // 0F C4
    {pinsrw     ,{mkop('V','dq'),mkop('E','d') ,mkop('I','b') }}, // (66) 0F C4
    {unkop                                                     }, // (F2) 0F C4
    {unkop                                                     }  // (F3) 0F C4

    };

    prefix_table opcodes_0FC5 = {

    {pextrw     ,{mkop('G','d') ,mkop('P','q') ,mkop('I','b') }}, // 0F C5
    {pextrw     ,{mkop('G','d') ,mkop('V','dq'),mkop('I','b') }}, // (66) 0F C5
    {unkop                                                     }, // (F2) 0F C5
    {unkop                                                     }  // (F3) 0F C5

    };

    prefix_table opcodes_0FC6 = {

    {shufps     ,{mkop('V','ps'),mkop('W','ps'),mkop('I','b') }}, // 0F C6
    {shufpd     ,{mkop('V','pd'),mkop('W','pd'),mkop('I','b') }}, // (66) 0F C6
    {unkop                                                     }, // (F2) 0F C6
    {unkop                                                     }  // (F3) 0F C6

    };

    mrm_mem_table opcodes_0FC7_Mem = {

    {unkop                                                     }, // 0F C7-000 [mem]
    {cmpxchg8b  ,{mkop('M','q') ,noop          ,noop          }}, // 0F C7-001 [mem]
    {unkop                                                     }, // 0F C7-010 [mem]
    {unkop                                                     }, // 0F C7-011 [mem]
    {unkop                                                     }, // 0F C7-100 [mem]
    {unkop                                                     }, // 0F C7-101 [mem]
    {unkop                                                     }, // 0F C7-110 [mem]
    {unkop                                                     }, // 0F C7-111 [mem]

    };

    prefix_table opcodes_0FD0 = {

    {unkop                                                     }, // 0F D0
    {addsubpd   ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D0
    {addsubps   ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (F2) 0F D0
    {unkop                                                     }  // (F3) 0F D0

    };

    prefix_table opcodes_0FD1 = {

    {psrlw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D1
    {psrlw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D1
    {unkop                                                     }, // (F2) 0F D1
    {unkop                                                     }  // (F3) 0F D1

    };

    prefix_table opcodes_0FD2 = {

    {psrld      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D2
    {psrld      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D2
    {unkop                                                     }, // (F2) 0F D2
    {unkop                                                     }  // (F3) 0F D2

    };

    prefix_table opcodes_0FD3 = {

    {psrlq      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D3
    {psrlq      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D3
    {unkop                                                     }, // (F2) 0F D3
    {unkop                                                     }  // (F3) 0F D3

    };

    prefix_table opcodes_0FD4 = {

    {paddq      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D4
    {paddq      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D4
    {unkop                                                     }, // (F2) 0F D4
    {unkop                                                     }  // (F3) 0F D4

    };

    prefix_table opcodes_0FD5 = {

    {pmullw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D5
    {pmullw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D5
    {unkop                                                     }, // (F2) 0F D5
    {unkop                                                     }  // (F3) 0F D5

    };

    prefix_table opcodes_0FD6 = {

    {unkop                                                     }, // 0F D6
    {movq       ,{mkop('W','q') ,mkop('V','q') ,noop          }}, // (66) 0F D6
    // NOTE: Both IDA and HIEW don't disassemble these instructions.
    // TODO: Intel says the second operand is 'Vq'
    {movdq2q    ,{mkop('P','q') ,mkop('W','q') ,noop          }}, // (F2) 0F D6
    {movq2dq    ,{mkop('V','dq'),mkop('Q','q') ,noop          }}  // (F3) 0F D6

    };

    prefix_table opcodes_0FD7 = {

    // NOTE: The following instruction's second operand is stated 'Vdq' in Intel's
    // specification, but is considered 'Wdq' by HIEW and IDA.
    {pmovmskb   ,{mkop('G','d') ,mkop('P','q') ,noop          }}, // 0F D7
    {pmovmskb   ,{mkop('G','d') ,mkop('W','dq'),noop          }}, // (66) 0F D7
    {unkop                                                     }, // (F2) 0F D7
    {unkop                                                     }  // (F3) 0F D7

    };

    prefix_table opcodes_0FD8 = {

    {psubusb    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D8
    {psubusb    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D8
    {unkop                                                     }, // (F2) 0F D8
    {unkop                                                     }  // (F3) 0F D8

    };

    prefix_table opcodes_0FD9 = {

    {psubusw    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F D9
    {psubusw    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F D9
    {unkop                                                     }, // (F2) 0F D9
    {unkop                                                     }  // (F3) 0F D9

    };

    prefix_table opcodes_0FDA = {

    {pminub     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DA
    {pminub     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DA
    {unkop                                                     }, // (F2) 0F DA
    {unkop                                                     }  // (F3) 0F DA

    };

    prefix_table opcodes_0FDB = {

    {pand       ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DB
    {pand       ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DB
    {unkop                                                     }, // (F2) 0F DB
    {unkop                                                     }  // (F3) 0F DB

    };

    prefix_table opcodes_0FDC = {

    {paddusb    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DC
    {paddusb    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DC
    {unkop                                                     }, // (F2) 0F DC
    {unkop                                                     }  // (F3) 0F DC

    };

    prefix_table opcodes_0FDD = {

    {paddusw    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DD
    {paddusw    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DD
    {unkop                                                     }, // (F2) 0F DD
    {unkop                                                     }  // (F3) 0F DD

    };

    prefix_table opcodes_0FDE = {

    {pmaxub     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DE
    {pmaxub     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DE
    {unkop                                                     }, // (F2) 0F DE
    {unkop                                                     }  // (F3) 0F DE

    };

    prefix_table opcodes_0FDF = {

    {pandn      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F DF
    {pandn      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F DF
    {unkop                                                     }, // (F2) 0F DF
    {unkop                                                     }  // (F3) 0F DF

    };

    prefix_table opcodes_0FE0 = {

    {pavgb      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E0
    {pavgb      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E0
    {unkop                                                     }, // (F2) 0F E0
    {unkop                                                     }  // (F3) 0F E0

    };

    prefix_table opcodes_0FE1 = {

    {psraw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E1
    {psraw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E1
    {unkop                                                     }, // (F2) 0F E1
    {unkop                                                     }  // (F3) 0F E1

    };

    prefix_table opcodes_0FE2 = {

    {psrad      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E2
    {psrad      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E2
    {unkop                                                     }, // (F2) 0F E2
    {unkop                                                     }  // (F3) 0F E2

    };

    prefix_table opcodes_0FE3 = {

    {pavgw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E3
    {pavgw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E3
    {unkop                                                     }, // (F2) 0F E3
    {unkop                                                     }  // (F3) 0F E3

    };

    prefix_table opcodes_0FE4 = {

    {pmulhuw    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E4
    {pmulhuw    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E4
    {unkop                                                     }, // (F2) 0F E4
    {unkop                                                     }  // (F3) 0F E4

    };

    prefix_table opcodes_0FE5 = {

    {pmulhw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E5
    {pmulhw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E5
    {unkop                                                     }, // (F2) 0F E5
    {unkop                                                     }  // (F3) 0F E5

    };

    prefix_table opcodes_0FE6 = {

    {unkop                                                     }, // 0F E6
    {cvttpd2dq  ,{mkop('V','dq'),mkop('W','pd'),noop          }}, // (66) 0F E6
    {cvtpd2dq   ,{mkop('V','dq'),mkop('W','pd'),noop          }}, // (F2) 0F E6
    {cvtdq2pd   ,{mkop('V','pd'),mkop('W','q') ,noop          }}  // (F3) 0F E6

    };

    prefix_table opcodes_0FE7 = {

    // TODO: Intel now uses 'Mq', 'Vq' and 'Mdq', 'Vdq' for these two
    // instructions.
    // Intel's specification defines the following instruction's operands as
    // 'Wq', 'Vq' but according to HIEW/IDA they are 'Qq', 'Pq'
    {movntq     ,{mkop('Q','q') ,mkop('P','q') ,noop          }}, // 0F E7
    {movntdq    ,{mkop('W','dq'),mkop('V','dq'),noop          }}, // (66) 0F E7
    {unkop                                                     }, // (F2) 0F E7
    {unkop                                                     }  // (F3) 0F E7

    };

    prefix_table opcodes_0FE8 = {

    {psubsb     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E8
    {psubsb     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E8
    {unkop                                                     }, // (F2) 0F E8
    {unkop                                                     }  // (F3) 0F E8

    };

    prefix_table opcodes_0FE9 = {

    {psubsw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F E9
    {psubsw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F E9
    {unkop                                                     }, // (F2) 0F E9
    {unkop                                                     }  // (F3) 0F E9

    };

    prefix_table opcodes_0FEA = {

    {pminsw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F EA
    {pminsw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F EA
    {unkop                                                     }, // (F2) 0F EA
    {unkop                                                     }  // (F3) 0F EA

    };

    prefix_table opcodes_0FEB = {

    {por        ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F EB
    {por        ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F EB
    {unkop                                                     }, // (F2) 0F EB
    {unkop                                                     }  // (F3) 0F EB

    };

    prefix_table opcodes_0FEC = {

    {paddsb     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F EC
    {paddsb     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F EC
    {unkop                                                     }, // (F2) 0F EC
    {unkop                                                     }  // (F3) 0F EC

    };

    prefix_table opcodes_0FED = {

    {paddsw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F ED
    {paddsw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F ED
    {unkop                                                     }, // (F2) 0F ED
    {unkop                                                     }  // (F3) 0F ED

    };

    prefix_table opcodes_0FEE = {

    {pmaxsw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F EE
    {pmaxsw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F EE
    {unkop                                                     }, // (F2) 0F EE
    {unkop                                                     }  // (F3) 0F EE

    };

    prefix_table opcodes_0FEF = {

    {pxor       ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F EF
    {pxor       ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F EF
    {unkop                                                     }, // (F2) 0F EF
    {unkop                                                     }  // (F3) 0F EF

    };

    prefix_table opcodes_0FF0 = {

    {unkop                                                     }, // 0F F0
    {unkop                                                     }, // (66) 0F F0
    {lddqu      ,{mkop('V','dq'),mkop('M','dq'),noop          }}, // (F2) 0F F0
    {unkop                                                     }  // (F3) 0F F0

    };

    prefix_table opcodes_0FF1 = {

    {psllw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F1
    {psllw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F1
    {unkop                                                     }, // (F2) 0F F1
    {unkop                                                     }  // (F3) 0F F1

    };

    prefix_table opcodes_0FF2 = {

    {pslld      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F2
    {pslld      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F2
    {unkop                                                     }, // (F2) 0F F2
    {unkop                                                     }  // (F3) 0F F2

    };

    prefix_table opcodes_0FF3 = {

    {psllq      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F3
    {psllq      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F3
    {unkop                                                     }, // (F2) 0F F3
    {unkop                                                     }  // (F3) 0F F3

    };

    prefix_table opcodes_0FF4 = {

    {pmuludq    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F4
    {pmuludq    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F4
    {unkop                                                     }, // (F2) 0F F4
    {unkop                                                     }  // (F3) 0F F4

    };

    prefix_table opcodes_0FF5 = {

    {pmaddwd    ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F5
    {pmaddwd    ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F5
    {unkop                                                     }, // (F2) 0F F5
    {unkop                                                     }  // (F3) 0F F5

    };

    prefix_table opcodes_0FF6 = {

    {psadbw     ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F6
    {psadbw     ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F6
    {unkop                                                     }, // (F2) 0F F6
    {unkop                                                     }  // (F3) 0F F6

    };

    prefix_table opcodes_0FF7 = {

    {maskmovq   ,{mkop('P','pi'),mkop('Q','pi'),noop          }}, // 0F F7
    {maskmovdqu ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F7
    {unkop                                                     }, // (F2) 0F F7
    {unkop                                                     }  // (F3) 0F F7

    };

    prefix_table opcodes_0FF8 = {

    {psubb      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F8
    {psubb      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F8
    {unkop                                                     }, // (F2) 0F F8
    {unkop                                                     }  // (F3) 0F F8

    };

    prefix_table opcodes_0FF9 = {

    {psubw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F F9
    {psubw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F F9
    {unkop                                                     }, // (F2) 0F F9
    {unkop                                                     }  // (F3) 0F F9

    };

    prefix_table opcodes_0FFA = {

    {psubd      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F FA
    {psubd      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F FA
    {unkop                                                     }, // (F2) 0F FA
    {unkop                                                     }  // (F3) 0F FA

    };

    prefix_table opcodes_0FFB = {

    {psubq      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F FB
    {psubq      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F FB
    {unkop                                                     }, // (F2) 0F FB
    {unkop                                                     }  // (F3) 0F FB

    };

    prefix_table opcodes_0FFC = {

    {paddb      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F FC
    {paddb      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F FC
    {unkop                                                     }, // (F2) 0F FC
    {unkop                                                     }  // (F3) 0F FC

    };

    prefix_table opcodes_0FFD = {

    {paddw      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F FD
    {paddw      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F FD
    {unkop                                                     }, // (F2) 0F FD
    {unkop                                                     }  // (F3) 0F FD

    };

    prefix_table opcodes_0FFE = {

    {paddd      ,{mkop('P','q') ,mkop('Q','q') ,noop          }}, // 0F FE
    {paddd      ,{mkop('V','dq'),mkop('W','dq'),noop          }}, // (66) 0F FE
    {unkop                                                     }, // (F2) 0F FE
    {unkop                                                     }  // (F3) 0F FE

    };

    table opcodes_0F = {

    {mkmrmbothtbl(opcodes_0F00)                                }, // 0F 00
    {mkmrmfulltbl(opcodes_0F01)                                }, // 0F 01
    {lar        ,{mkop('G','v') ,mkop('E','w') ,noop          }}, // 0F 02
    {lsl        ,{mkop('G','v') ,mkop('E','w') ,noop          }}, // 0F 03
    {unkop                                                     }, // 0F 04
    {unkop                                                     }, // 0F 05
    {clts       ,{noop          ,noop          ,noop          }}, // 0F 06
    {unkop                                                     }, // 0F 07
    {invd       ,{noop          ,noop          ,noop          }}, // 0F 08
    {wbinvd     ,{noop          ,noop          ,noop          }}, // 0F 09
    {unkop                                                     }, // 0F 0A
    {ud2        ,{noop          ,noop          ,noop          }}, // 0F 0B
    {unkop                                                     }, // 0F 0C
    {unkop                                                     }, // 0F 0D
    {unkop                                                     }, // 0F 0E
    {unkop                                                     }, // 0F 0F
    {mkptable(opcodes_0F10)                                    }, // 0F 10
    {mkptable(opcodes_0F11)                                    }, // 0F 11
    {mkptable(opcodes_0F12)                                    }, // 0F 12
    {mkptable(opcodes_0F13)                                    }, // 0F 13
    {mkptable(opcodes_0F14)                                    }, // 0F 14
    {mkptable(opcodes_0F15)                                    }, // 0F 15
    {mkptable(opcodes_0F16)                                    }, // 0F 16
    {mkptable(opcodes_0F17)                                    }, // 0F 17
    {mkmrmmemtbl(opcodes_0F18)                                 }, // 0F 18
    {unkop                                                     }, // 0F 19
    {unkop                                                     }, // 0F 1A
    {unkop                                                     }, // 0F 1B
    {unkop                                                     }, // 0F 1C
    {unkop                                                     }, // 0F 1D
    {unkop                                                     }, // 0F 1E
    {unkop                                                     }, // 0F 1F
    {mov        ,{mkop('R','d') ,mkop('C','d') ,noop          }}, // 0F 20
    {mov        ,{mkop('R','d') ,mkop('D','d') ,noop          }}, // 0F 21
    {mov        ,{mkop('C','d') ,mkop('R','d') ,noop          }}, // 0F 22
    {mov        ,{mkop('D','d') ,mkop('R','d') ,noop          }}, // 0F 23
    {mov        ,{mkop('R','d') ,mkop('T','d') ,noop          }}, // 0F 24
    {unkop                                                     }, // 0F 25
    {mov        ,{mkop('T','d') ,mkop('R','d') ,noop          }}, // 0F 26
    {unkop                                                     }, // 0F 27
    {mkptable(opcodes_0F28)                                    }, // 0F 28
    {mkptable(opcodes_0F29)                                    }, // 0F 29
    {mkptable(opcodes_0F2A)                                    }, // 0F 2A
    {mkptable(opcodes_0F2B)                                    }, // 0F 2B
    {mkptable(opcodes_0F2C)                                    }, // 0F 2C
    {mkptable(opcodes_0F2D)                                    }, // 0F 2D
    {mkptable(opcodes_0F2E)                                    }, // 0F 2E
    {mkptable(opcodes_0F2F)                                    }, // 0F 2F
    {wrmsr      ,{noop          ,noop          ,noop          }}, // 0F 30
    {rdtsc      ,{noop          ,noop          ,noop          }}, // 0F 31
    {rdmsr      ,{noop          ,noop          ,noop          }}, // 0F 32
    {rdpmc      ,{noop          ,noop          ,noop          }}, // 0F 33
    {sysenter   ,{noop          ,noop          ,noop          }}, // 0F 34
    {sysexit    ,{noop          ,noop          ,noop          }}, // 0F 35
    {unkop                                                     }, // 0F 36
    {unkop                                                     }, // 0F 37
    {unkop                                                     }, // 0F 38
    {unkop                                                     }, // 0F 39
    {unkop                                                     }, // 0F 3A
    {unkop                                                     }, // 0F 3B
    {unkop                                                     }, // 0F 3C
    {unkop                                                     }, // 0F 3D
    {unkop                                                     }, // 0F 3E
    {unkop                                                     }, // 0F 3F
    {cmovo      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 40
    {cmovno     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 41
    {cmovb      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 42
    {cmovae     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 43
    {cmove      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 44
    {cmovne     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 45
    {cmovbe     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 46
    {cmova      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 47
    {cmovs      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 48
    {cmovns     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 49
    {cmovp      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4A
    {cmovnp     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4B
    {cmovl      ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4C
    {cmovnl     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4D
    {cmovle     ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4E
    {cmovnle    ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F 4F
    {mkptable(opcodes_0F50)                                    }, // 0F 50
    {mkptable(opcodes_0F51)                                    }, // 0F 51
    {mkptable(opcodes_0F52)                                    }, // 0F 52
    {mkptable(opcodes_0F53)                                    }, // 0F 53
    {mkptable(opcodes_0F54)                                    }, // 0F 54
    {mkptable(opcodes_0F55)                                    }, // 0F 55
    {mkptable(opcodes_0F56)                                    }, // 0F 56
    {mkptable(opcodes_0F57)                                    }, // 0F 57
    {mkptable(opcodes_0F58)                                    }, // 0F 58
    {mkptable(opcodes_0F59)                                    }, // 0F 59
    {mkptable(opcodes_0F5A)                                    }, // 0F 5A
    {mkptable(opcodes_0F5B)                                    }, // 0F 5B
    {mkptable(opcodes_0F5C)                                    }, // 0F 5C
    {mkptable(opcodes_0F5D)                                    }, // 0F 5D
    {mkptable(opcodes_0F5E)                                    }, // 0F 5E
    {mkptable(opcodes_0F5F)                                    }, // 0F 5F
    {mkptable(opcodes_0F60)                                    }, // 0F 60
    {mkptable(opcodes_0F61)                                    }, // 0F 61
    {mkptable(opcodes_0F62)                                    }, // 0F 62
    {mkptable(opcodes_0F63)                                    }, // 0F 63
    {mkptable(opcodes_0F64)                                    }, // 0F 64
    {mkptable(opcodes_0F65)                                    }, // 0F 65
    {mkptable(opcodes_0F66)                                    }, // 0F 66
    {mkptable(opcodes_0F67)                                    }, // 0F 67
    {mkptable(opcodes_0F68)                                    }, // 0F 68
    {mkptable(opcodes_0F69)                                    }, // 0F 69
    {mkptable(opcodes_0F6A)                                    }, // 0F 6A
    {mkptable(opcodes_0F6B)                                    }, // 0F 6B
    {mkptable(opcodes_0F6C)                                    }, // 0F 6C
    {mkptable(opcodes_0F6D)                                    }, // 0F 6D
    {mkptable(opcodes_0F6E)                                    }, // 0F 6E
    {mkptable(opcodes_0F6F)                                    }, // 0F 6F
    {mkptable(opcodes_0F70)                                    }, // 0F 70
    {mkptable(opcodes_0F71)                                    }, // 0F 71
    {mkptable(opcodes_0F72)                                    }, // 0F 72
    {mkptable(opcodes_0F73)                                    }, // 0F 73
    {mkptable(opcodes_0F74)                                    }, // 0F 74
    {mkptable(opcodes_0F75)                                    }, // 0F 75
    {mkptable(opcodes_0F76)                                    }, // 0F 76
    {emms       ,{noop          ,noop          ,noop          }}, // 0F 77
    {unkop                                                     }, // 0F 78
    {unkop                                                     }, // 0F 79
    {unkop                                                     }, // 0F 7A
    {unkop                                                     }, // 0F 7B
    {mkptable(opcodes_0F7C)                                    }, // 0F 7C
    {mkptable(opcodes_0F7D)                                    }, // 0F 7D
    {mkptable(opcodes_0F7E)                                    }, // 0F 7E
    {mkptable(opcodes_0F7F)                                    }, // 0F 7F
    {jo         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 80
    {jno        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 81
    {jb         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 82
    {jnb        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 83
    {jz         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 84
    {jnz        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 85
    {jbe        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 86
    {jnbe       ,{mkop('J','v') ,noop          ,noop          }}, // 0F 87
    {js         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 88
    {jns        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 89
    {jp         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8A
    {jnp        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8B
    {jl         ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8C
    {jnl        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8D
    {jle        ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8E
    {jnle       ,{mkop('J','v') ,noop          ,noop          }}, // 0F 8F
    {seto       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 90
    {setno      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 91
    {setb       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 92
    {setnb      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 93
    {setz       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 94
    {setnz      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 95
    {setbe      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 96
    {setnbe     ,{mkop('E','b') ,noop          ,noop          }}, // 0F 97
    {sets       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 98
    {setns      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 99
    {setp       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9A
    {setnp      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9B
    {setl       ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9C
    {setnl      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9D
    {setle      ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9E
    {setnle     ,{mkop('E','b') ,noop          ,noop          }}, // 0F 9F
    {push       ,{mkreg(fs)     ,noop          ,noop          }}, // 0F A0
    {pop        ,{mkreg(fs)     ,noop          ,noop          }}, // 0F A1
    {cpuid      ,{noop          ,noop          ,noop          }}, // 0F A2
    {bt         ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F A3
    {shld       ,{mkop('E','v') ,mkop('G','v') ,mkop('I','b') }}, // 0F A4
    {shld       ,{mkop('E','v') ,mkop('G','v') ,mkreg(cl)     }}, // 0F A5
    {unkop                                                     }, // 0F A6
    {unkop                                                     }, // 0F A7
    {push       ,{mkreg(gs)     ,noop          ,noop          }}, // 0F A8
    {pop        ,{mkreg(gs)     ,noop          ,noop          }}, // 0F A9
    {rsm        ,{noop          ,noop          ,noop          }}, // 0F AA
    {bts        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F AB
    {shrd       ,{mkop('E','v') ,mkop('G','v') ,mkop('I','b') }}, // 0F AC
    {shrd       ,{mkop('E','v') ,mkop('G','v') ,mkreg(cl)     }}, // 0F AD
    {mkmrmfulltbl(opcodes_0FAE)                                }, // 0F AE
    {imul       ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F AF
    {cmpxchg    ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 0F B0
    {cmpxchg    ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F B1
    // NOTE: The Intel documentation 'forgot' the first operand of the
    // following opcodes, which is 'Gv'
    // TODO: Intel now amended this, but still has LSS/LFS/LGS taking one 'Mp'.
    {lss        ,{mkop('G','v') ,mkop('M','p') ,noop          }}, // 0F B2
    {btr        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F B3
    {lfs        ,{mkop('G','v') ,mkop('M','p') ,noop          }}, // 0F B4
    {lgs        ,{mkop('G','v') ,mkop('M','p') ,noop          }}, // 0F B5
    {movzx      ,{mkop('G','v') ,mkop('E','b') ,noop          }}, // 0F B6
    {movzx      ,{mkop('G','v') ,mkop('E','w') ,noop          }}, // 0F B7
    {unkop                                                     }, // 0F B8
    // NOTE: This opcode is an invalid opcode (not UD2).
    {unkop                                                     }, // 0F B9
    {mkmrmbothtbl(opcodes_0FBA)                                }, // 0F BA
    {btc        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F BB
    {bsf        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F BC
    {bsr        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0F BD
    {movsx      ,{mkop('G','v') ,mkop('E','b') ,noop          }}, // 0F BE
    {movsx      ,{mkop('G','v') ,mkop('E','w') ,noop          }}, // 0F BF
    {xadd       ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 0F C0
    {xadd       ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 0F C1
    {mkptable(opcodes_0FC2)                                    }, // 0F C2
    {movnti     ,{mkop('M','d') ,mkop('G','d') ,noop          }}, // 0F C3
    {mkptable(opcodes_0FC4)                                    }, // 0F C4
    {mkptable(opcodes_0FC5)                                    }, // 0F C5
    {mkptable(opcodes_0FC6)                                    }, // 0F C6
    {mkmrmmemtbl(opcodes_0FC7_Mem)                             }, // 0F C7
    {bswap      ,{mkreg(eax)     ,noop          ,noop         }}, // 0F C8
    {bswap      ,{mkreg(ecx)     ,noop          ,noop         }}, // 0F C9
    {bswap      ,{mkreg(edx)     ,noop          ,noop         }}, // 0F CA
    {bswap      ,{mkreg(ebx)     ,noop          ,noop         }}, // 0F CB
    {bswap      ,{mkreg(esp)     ,noop          ,noop         }}, // 0F CC
    {bswap      ,{mkreg(ebp)     ,noop          ,noop         }}, // 0F CD
    {bswap      ,{mkreg(esi)     ,noop          ,noop         }}, // 0F CE
    {bswap      ,{mkreg(edi)     ,noop          ,noop         }}, // 0F CF
    {mkptable(opcodes_0FD0)                                    }, // 0F D0
    {mkptable(opcodes_0FD1)                                    }, // 0F D1
    {mkptable(opcodes_0FD2)                                    }, // 0F D2
    {mkptable(opcodes_0FD3)                                    }, // 0F D3
    {mkptable(opcodes_0FD4)                                    }, // 0F D4
    {mkptable(opcodes_0FD5)                                    }, // 0F D5
    {mkptable(opcodes_0FD6)                                    }, // 0F D6
    {mkptable(opcodes_0FD7)                                    }, // 0F D7
    {mkptable(opcodes_0FD8)                                    }, // 0F D8
    {mkptable(opcodes_0FD9)                                    }, // 0F D9
    {mkptable(opcodes_0FDA)                                    }, // 0F DA
    {mkptable(opcodes_0FDB)                                    }, // 0F DB
    {mkptable(opcodes_0FDC)                                    }, // 0F DC
    {mkptable(opcodes_0FDD)                                    }, // 0F DD
    {mkptable(opcodes_0FDE)                                    }, // 0F DE
    {mkptable(opcodes_0FDF)                                    }, // 0F DF
    {mkptable(opcodes_0FE0)                                    }, // 0F E0
    {mkptable(opcodes_0FE1)                                    }, // 0F E1
    {mkptable(opcodes_0FE2)                                    }, // 0F E2
    {mkptable(opcodes_0FE3)                                    }, // 0F E3
    {mkptable(opcodes_0FE4)                                    }, // 0F E4
    {mkptable(opcodes_0FE5)                                    }, // 0F E5
    {mkptable(opcodes_0FE6)                                    }, // 0F E6
    {mkptable(opcodes_0FE7)                                    }, // 0F E7
    {mkptable(opcodes_0FE8)                                    }, // 0F E8
    {mkptable(opcodes_0FE9)                                    }, // 0F E9
    {mkptable(opcodes_0FEA)                                    }, // 0F EA
    {mkptable(opcodes_0FEB)                                    }, // 0F EB
    {mkptable(opcodes_0FEC)                                    }, // 0F EC
    {mkptable(opcodes_0FED)                                    }, // 0F ED
    {mkptable(opcodes_0FEE)                                    }, // 0F EE
    {mkptable(opcodes_0FEF)                                    }, // 0F EF
    {mkptable(opcodes_0FF0)                                    }, // 0F F0
    {mkptable(opcodes_0FF1)                                    }, // 0F F1
    {mkptable(opcodes_0FF2)                                    }, // 0F F2
    {mkptable(opcodes_0FF3)                                    }, // 0F F3
    {mkptable(opcodes_0FF4)                                    }, // 0F F4
    {mkptable(opcodes_0FF5)                                    }, // 0F F5
    {mkptable(opcodes_0FF6)                                    }, // 0F F6
    {mkptable(opcodes_0FF7)                                    }, // 0F F7
    {mkptable(opcodes_0FF8)                                    }, // 0F F8
    {mkptable(opcodes_0FF9)                                    }, // 0F F9
    {mkptable(opcodes_0FFA)                                    }, // 0F FA
    {mkptable(opcodes_0FFB)                                    }, // 0F FB
    {mkptable(opcodes_0FFC)                                    }, // 0F FC
    {mkptable(opcodes_0FFD)                                    }, // 0F FD
    {mkptable(opcodes_0FFE)                                    }, // 0F FE
    {unkop                                                     }  // 0F FF

    };

    mrm_both_table opcodes_80 = {

    {add        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-000
    {or_        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-001
    {adc        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-010
    {sbb        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-011
    {and_       ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-100
    {sub        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-101
    {xor_       ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 80-110
    {cmp        ,{mkop('E','b') ,mkop('I','b') ,noop          }}  // 80-111

    };

    mrm_both_table opcodes_81 = {

    {add        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-000
    {or_        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-001
    {adc        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-010
    {sbb        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-011
    {and_       ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-100
    {sub        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-101
    {xor_       ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // 81-110
    {cmp        ,{mkop('E','v') ,mkop('I','v') ,noop          }}  // 81-111

    };

    mrm_both_table opcodes_82 = {

    {add        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-000
    {or_        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-001
    {adc        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-010
    {sbb        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-011
    {and_       ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-100
    {sub        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-101
    {xor_       ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // 82-110
    {cmp        ,{mkop('E','b') ,mkop('I','b') ,noop          }}  // 82-111

    };

    mrm_both_table opcodes_83 = {

    {add        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-000
    {or_        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-001
    {adc        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-010
    {sbb        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-011
    {and_       ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-100
    {sub        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-101
    {xor_       ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // 83-110
    {cmp        ,{mkop('E','v') ,mkop('I','b') ,noop          }}  // 83-111

    };

    prefix_table opcodes_90 = {

    {nop        ,{noop          ,noop          ,noop          }}, // 90
    {unkop                                                     }, // (66) 90
    {unkop                                                     }, // (F2) 90
    {pause      ,{noop          ,noop          ,noop          }}  // (F3) 90

    };

    mrm_both_table opcodes_C0 = {

    {rol        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-000
    {ror        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-001
    {rcl        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-010
    {rcr        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-011
    {shl        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-100
    {shr        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C0-101
    {unkop                                                     }, // C0-110
    {sar        ,{mkop('E','b') ,mkop('I','b') ,noop          }}  // C0-111

    };

    mrm_both_table opcodes_C1 = {

    {rol        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-000
    {ror        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-001
    {rcl        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-010
    {rcr        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-011
    {shl        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-100
    {shr        ,{mkop('E','v') ,mkop('I','b') ,noop          }}, // C1-101
    {unkop                                                     }, // C1-110
    {sar        ,{mkop('E','v') ,mkop('I','b') ,noop          }}  // C1-111

    };

    mrm_both_table opcodes_C6 = {

    {mov        ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // C6-000
    {unkop                                                     }, // C6-001
    {unkop                                                     }, // C6-010
    {unkop                                                     }, // C6-011
    {unkop                                                     }, // C6-100
    {unkop                                                     }, // C6-101
    {unkop                                                     }, // C6-110
    {unkop                                                     }  // C6-111

    };

    mrm_both_table opcodes_C7 = {

    {mov        ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // C7-000
    {unkop                                                     }, // C7-001
    {unkop                                                     }, // C7-010
    {unkop                                                     }, // C7-011
    {unkop                                                     }, // C7-100
    {unkop                                                     }, // C7-101
    {unkop                                                     }, // C7-110
    {unkop                                                     }  // C7-111

    };

    mrm_both_table opcodes_D0 = {

    {rol        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-000
    {ror        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-001
    {rcl        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-010
    {rcr        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-011
    {shl        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-100
    {shr        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}, // D0-101
    {unkop                                                     }, // D0-110
    {sar        ,{mkop('E','b') ,mkreg(val_1)  ,noop          }}  // D0-111

    };

    mrm_both_table opcodes_D1 = {

    {rol        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-000
    {ror        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-001
    {rcl        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-010
    {rcr        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-011
    {shl        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-100
    {shr        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}, // D1-101
    {unkop                                                     }, // D1-110
    {sar        ,{mkop('E','v') ,mkreg(val_1)  ,noop          }}  // D1-111

    };

    mrm_both_table opcodes_D2 = {

    {rol        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-000
    {ror        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-001
    {rcl        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-010
    {rcr        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-011
    {shl        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-100
    {shr        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}, // D2-101
    // According to HIEW, the following instruction is SAL. According to
    // Intel's specification, however, it is undefined.
    {unkop                                                     }, // D2-110
    {sar        ,{mkop('E','b') ,mkreg(cl)     ,noop          }}  // D2-111

    };

    mrm_both_table opcodes_D3 = {

    {rol        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-000
    {ror        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-001
    {rcl        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-010
    {rcr        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-011
    {shl        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-100
    {shr        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}, // D3-101
    {unkop                                                     }, // D3-110
    {sar        ,{mkop('E','v') ,mkreg(cl)     ,noop          }}  // D3-111

    };

    fpu_table opcodes_D8 = {

    {fadd       ,{mkop('E','fd'),noop          ,noop          }}, // D8-000
    {fmul       ,{mkop('E','fd'),noop          ,noop          }}, // D8-001
    {fcom       ,{mkop('E','fd'),noop          ,noop          }}, // D8-010
    {fcomp      ,{mkop('E','fd'),noop          ,noop          }}, // D8-011
    {fsub       ,{mkop('E','fd'),noop          ,noop          }}, // D8-100
    {fsubr      ,{mkop('E','fd'),noop          ,noop          }}, // D8-101
    {fdiv       ,{mkop('E','fd'),noop          ,noop          }}, // D8-110
    {fdivr      ,{mkop('E','fd'),noop          ,noop          }}, // D8-111

    {fadd       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 C0
    {fadd       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 C1
    {fadd       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 C2
    {fadd       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 C3
    {fadd       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 C4
    {fadd       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 C5
    {fadd       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 C6
    {fadd       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 C7

    {fmul       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 C8
    {fmul       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 C9
    {fmul       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 CA
    {fmul       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 CB
    {fmul       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 CC
    {fmul       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 CD
    {fmul       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 CE
    {fmul       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 CF

    {fcom       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 D0
    {fcom       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 D1
    {fcom       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 D2
    {fcom       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 D3
    {fcom       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 D4
    {fcom       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 D5
    {fcom       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 D6
    {fcom       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 D7

    {fcomp      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 D8
    {fcomp      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 D9
    {fcomp      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 DA
    {fcomp      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 DB
    {fcomp      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 DC
    {fcomp      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 DD
    {fcomp      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 DE
    {fcomp      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 DF

    {fsub       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 E0
    {fsub       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 E1
    {fsub       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 E2
    {fsub       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 E3
    {fsub       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 E4
    {fsub       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 E5
    {fsub       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 E6
    {fsub       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 E7

    {fsubr      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 E8
    {fsubr      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 E9
    {fsubr      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 EA
    {fsubr      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 EB
    {fsubr      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 EC
    {fsubr      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 ED
    {fsubr      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 EE
    {fsubr      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 EF

    {fdiv       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 F0
    {fdiv       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 F1
    {fdiv       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 F2
    {fdiv       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 F3
    {fdiv       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 F4
    {fdiv       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 F5
    {fdiv       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 F6
    {fdiv       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D8 F7

    {fdivr      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D8 F8
    {fdivr      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D8 F9
    {fdivr      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D8 FA
    {fdivr      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D8 FB
    {fdivr      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D8 FC
    {fdivr      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D8 FD
    {fdivr      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D8 FE
    {fdivr      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}  // D8 FF

    };

    fpu_table opcodes_D9 = {

    {fld        ,{mkop('E','fd'),noop          ,noop          }}, // D9-000
    {unkop                                                     }, // D9-001
    {fst        ,{mkop('E','fd'),noop          ,noop          }}, // D9-010
    {fstp       ,{mkop('E','fd'),noop          ,noop          }}, // D9-011
    {fldenv     ,{mkop('E','fe'),noop          ,noop          }}, // D9-100
    {fldcw      ,{mkop('E','w') ,noop          ,noop          }}, // D9-101
    {fstenv     ,{mkop('E','fe'),noop          ,noop          }}, // D9-110
    {fstcw      ,{mkop('E','w') ,noop          ,noop          }}, // D9-111

    {fld        ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D9 C0
    {fld        ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D9 C1
    {fld        ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D9 C2
    {fld        ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D9 C3
    {fld        ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D9 C4
    {fld        ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D9 C5
    {fld        ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D9 C6
    {fld        ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D9 C7

    {fxch       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // D9 C8
    {fxch       ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // D9 C9
    {fxch       ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // D9 CA
    {fxch       ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // D9 CB
    {fxch       ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // D9 CC
    {fxch       ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // D9 CD
    {fxch       ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // D9 CE
    {fxch       ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // D9 CF

    {fnop       ,{noop          ,noop          ,noop          }}, // D9 D0
    {unkop                                                     }, // D9 D1
    {unkop                                                     }, // D9 D2
    {unkop                                                     }, // D9 D3
    {unkop                                                     }, // D9 D4
    {unkop                                                     }, // D9 D5
    {unkop                                                     }, // D9 D6
    {unkop                                                     }, // D9 D7

    {unkop                                                     }, // D9 D8
    {unkop                                                     }, // D9 D9
    {unkop                                                     }, // D9 DA
    {unkop                                                     }, // D9 DB
    {unkop                                                     }, // D9 DC
    {unkop                                                     }, // D9 DD
    {unkop                                                     }, // D9 DE
    {unkop                                                     }, // D9 DF

    {fchs      ,{noop           ,noop          ,noop          }}, // D9 E0
    {fabs      ,{noop           ,noop          ,noop          }}, // D9 E1
    {unkop                                                     }, // D9 E2
    {unkop                                                     }, // D9 E3
    {ftst      ,{noop           ,noop          ,noop          }}, // D9 E4
    {fxam      ,{noop           ,noop          ,noop          }}, // D9 E5
    {unkop                                                     }, // D9 E6
    {unkop                                                     }, // D9 E7

    {fld1      ,{noop           ,noop          ,noop          }}, // D9 E8
    {fldl2t    ,{noop           ,noop          ,noop          }}, // D9 E9
    {fldl2e    ,{noop           ,noop          ,noop          }}, // D9 EA
    {fldpi     ,{noop           ,noop          ,noop          }}, // D9 EB
    {fldlg2    ,{noop           ,noop          ,noop          }}, // D9 EC
    {fldln2    ,{noop           ,noop          ,noop          }}, // D9 ED
    {fldz      ,{noop           ,noop          ,noop          }}, // D9 EE
    {unkop                                                     }, // D9 EF

    {f2xm1     ,{noop           ,noop          ,noop          }}, // D9 F0
    {fyl2x     ,{noop           ,noop          ,noop          }}, // D9 F1
    {fptan     ,{noop           ,noop          ,noop          }}, // D9 F2
    {fpatan    ,{noop           ,noop          ,noop          }}, // D9 F3
    {fxtract   ,{noop           ,noop          ,noop          }}, // D9 F4
    {fprem1    ,{noop           ,noop          ,noop          }}, // D9 F5
    {fdecstp   ,{noop           ,noop          ,noop          }}, // D9 F6
    {fincstp   ,{noop           ,noop          ,noop          }}, // D9 F7

    {fprem     ,{noop           ,noop          ,noop          }}, // D9 F8
    {fyl2xp1   ,{noop           ,noop          ,noop          }}, // D9 F9
    {fsqrt     ,{noop           ,noop          ,noop          }}, // D9 FA
    {fsincos   ,{noop           ,noop          ,noop          }}, // D9 FB
    {frndint   ,{noop           ,noop          ,noop          }}, // D9 FC
    {fscale    ,{noop           ,noop          ,noop          }}, // D9 FD
    {fsin      ,{noop           ,noop          ,noop          }}, // D9 FE
    {fcos      ,{noop           ,noop          ,noop          }}  // D9 FF

    };

    fpu_table opcodes_DA = {

    {fiadd      ,{mkop('E','v') ,noop          ,noop          }}, // DA-000
    {fimul      ,{mkop('E','v') ,noop          ,noop          }}, // DA-001
    {ficom      ,{mkop('E','v') ,noop          ,noop          }}, // DA-010
    {ficomp     ,{mkop('E','v') ,noop          ,noop          }}, // DA-011
    {fisub      ,{mkop('E','v') ,noop          ,noop          }}, // DA-100
    {fisubr     ,{mkop('E','v') ,noop          ,noop          }}, // DA-101
    {fidiv      ,{mkop('E','v') ,noop          ,noop          }}, // DA-110
    {fidivr     ,{mkop('E','v') ,noop          ,noop          }}, // DA-111

    {fcmovb     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DA C0
    {fcmovb     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DA C1
    {fcmovb     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DA C2
    {fcmovb     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DA C3
    {fcmovb     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DA C4
    {fcmovb     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DA C5
    {fcmovb     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DA C6
    {fcmovb     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DA C7

    {fcmove     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DA C8
    {fcmove     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DA C9
    {fcmove     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DA CA
    {fcmove     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DA CB
    {fcmove     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DA CC
    {fcmove     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DA CD
    {fcmove     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DA CE
    {fcmove     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DA CF

    {fcmovbe    ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DA D0
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DA D1
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DA D2
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DA D3
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DA D4
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DA D5
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DA D6
    {fcmovbe    ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DA D7

    {fcmovu     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DA D8
    {fcmovu     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DA D9
    {fcmovu     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DA DA
    {fcmovu     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DA DB
    {fcmovu     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DA DC
    {fcmovu     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DA DD
    {fcmovu     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DA DE
    {fcmovu     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DA DF

    {unkop                                                     }, // DA E0
    {unkop                                                     }, // DA E1
    {unkop                                                     }, // DA E2
    {unkop                                                     }, // DA E3
    {unkop                                                     }, // DA E4
    {unkop                                                     }, // DA E5
    {unkop                                                     }, // DA E6
    {unkop                                                     }, // DA E7

    {unkop                                                     }, // DA E8
    {fucompp    ,{noop          ,noop          ,noop          }}, // DA E9
    {unkop                                                     }, // DA EA
    {unkop                                                     }, // DA EB
    {unkop                                                     }, // DA EC
    {unkop                                                     }, // DA ED
    {unkop                                                     }, // DA EE
    {unkop                                                     }, // DA EF

    {unkop                                                     }, // DA F0
    {unkop                                                     }, // DA F1
    {unkop                                                     }, // DA F2
    {unkop                                                     }, // DA F3
    {unkop                                                     }, // DA F4
    {unkop                                                     }, // DA F5
    {unkop                                                     }, // DA F6
    {unkop                                                     }, // DA F7

    {unkop                                                     }, // DA F8
    {unkop                                                     }, // DA F9
    {unkop                                                     }, // DA FA
    {unkop                                                     }, // DA FB
    {unkop                                                     }, // DA FC
    {unkop                                                     }, // DA FD
    {unkop                                                     }, // DA FE
    {unkop                                                     }  // DA FF

    };

    fpu_table opcodes_DB = {

    {fild       ,{mkop('E','v') ,noop          ,noop          }}, // DB-000
    {fisttp     ,{mkop('E','v') ,noop          ,noop          }}, // DB-001
    {fist       ,{mkop('E','v') ,noop          ,noop          }}, // DB-010
    {fistp      ,{mkop('E','v') ,noop          ,noop          }}, // DB-011
    {unkop                                                     }, // DB-100
    {fld        ,{mkop('E','ft'),noop          ,noop          }}, // DB-101
    {unkop                                                     }, // DB-110
    {fstp       ,{mkop('E','ft'),noop          ,noop          }}, // DB-111

    {fcmovnb    ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB C0
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB C1
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB C2
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB C3
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB C4
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB C5
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB C6
    {fcmovnb    ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB C7

    {fcmovne    ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB C8
    {fcmovne    ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB C9
    {fcmovne    ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB CA
    {fcmovne    ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB CB
    {fcmovne    ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB CC
    {fcmovne    ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB CD
    {fcmovne    ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB CE
    {fcmovne    ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB CF

    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB D0
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB D1
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB D2
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB D3
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB D4
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB D5
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB D6
    {fcmovnbe   ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB D7

    {fcmovnu    ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB D8
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB D9
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB DA
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB DB
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB DC
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB DD
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB DE
    {fcmovnu    ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB DF

    {unkop                                                     }, // DB E0
    {unkop                                                     }, // DB E1
    {fclex      ,{noop          ,noop          ,noop          }}, // DB D2
    {finit      ,{noop          ,noop          ,noop          }}, // DB D3
    {unkop                                                     }, // DB E4
    {unkop                                                     }, // DB E5
    {unkop                                                     }, // DB E6
    {unkop                                                     }, // DB E7

    {fucomi     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB E8
    {fucomi     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB E9
    {fucomi     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB EA
    {fucomi     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB EB
    {fucomi     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB EC
    {fucomi     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB ED
    {fucomi     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB EE
    {fucomi     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB EF

    {fcomi      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DB F0
    {fcomi      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DB F1
    {fcomi      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DB F2
    {fcomi      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DB F3
    {fcomi      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DB F4
    {fcomi      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DB F5
    {fcomi      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DB F6
    {fcomi      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DB F7

    {unkop                                                     }, // DB F8
    {unkop                                                     }, // DB F9
    {unkop                                                     }, // DB FA
    {unkop                                                     }, // DB FB
    {unkop                                                     }, // DB FC
    {unkop                                                     }, // DB FD
    {unkop                                                     }, // DB FE
    {unkop                                                     }  // DB FF

    };

    fpu_table opcodes_DC = {

    {fadd       ,{mkop('E','fq'),noop          ,noop          }}, // DC-000
    {fmul       ,{mkop('E','fq'),noop          ,noop          }}, // DC-001
    {fcom       ,{mkop('E','fq'),noop          ,noop          }}, // DC-010
    {fcomp      ,{mkop('E','fq'),noop          ,noop          }}, // DC-011
    {fsub       ,{mkop('E','fq'),noop          ,noop          }}, // DC-100
    {fsubr      ,{mkop('E','fq'),noop          ,noop          }}, // DC-101
    {fdiv       ,{mkop('E','fq'),noop          ,noop          }}, // DC-110
    {fdivr      ,{mkop('E','fq'),noop          ,noop          }}, // DC-111

    {fadd       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC C0
    {fadd       ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC C1
    {fadd       ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC C2
    {fadd       ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC C3
    {fadd       ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC C4
    {fadd       ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC C5
    {fadd       ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC C6
    {fadd       ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DC C7

    {fmul       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC C8
    {fmul       ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC C9
    {fmul       ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC CA
    {fmul       ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC CB
    {fmul       ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC CC
    {fmul       ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC CD
    {fmul       ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC CE
    {fmul       ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DC CF

    {unkop                                                     }, // DC D0
    {unkop                                                     }, // DC D1
    {unkop                                                     }, // DC D2
    {unkop                                                     }, // DC D3
    {unkop                                                     }, // DC D4
    {unkop                                                     }, // DC D5
    {unkop                                                     }, // DC D6
    {unkop                                                     }, // DC D7

    {unkop                                                     }, // DC D8
    {unkop                                                     }, // DC D9
    {unkop                                                     }, // DC DA
    {unkop                                                     }, // DC DB
    {unkop                                                     }, // DC DC
    {unkop                                                     }, // DC DD
    {unkop                                                     }, // DC DE
    {unkop                                                     }, // DC DF

    {fsub       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC E0
    {fsub       ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC E1
    {fsub       ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC E2
    {fsub       ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC E3
    {fsub       ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC E4
    {fsub       ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC E5
    {fsub       ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC E6
    {fsub       ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DC E7

    {fsubr      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC E8
    {fsubr      ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC E9
    {fsubr      ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC EA
    {fsubr      ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC EB
    {fsubr      ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC EC
    {fsubr      ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC ED
    {fsubr      ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC EE
    {fsubr      ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DC EF

    {fdivr      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC F0
    {fdivr      ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC F1
    {fdivr      ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC F2
    {fdivr      ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC F3
    {fdivr      ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC F4
    {fdivr      ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC F5
    {fdivr      ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC F6
    {fdivr      ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DC F7

    {fdiv       ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DC F8
    {fdiv       ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DC F9
    {fdiv       ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DC FA
    {fdiv       ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DC FB
    {fdiv       ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DC FC
    {fdiv       ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DC FD
    {fdiv       ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DC FE
    {fdiv       ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}  // DC FF

    };

    fpu_table opcodes_DD = {

    {fld        ,{mkop('E','fq'),noop          ,noop          }}, // DD-000
    {fisttp     ,{mkop('E','q') ,noop          ,noop          }}, // DD-001
    {fst        ,{mkop('E','fq'),noop          ,noop          }}, // DD-010
    {fstp       ,{mkop('E','fq'),noop          ,noop          }}, // DD-011
    {frstor     ,{mkop('E','fs'),noop          ,noop          }}, // DD-100
    {unkop                                                     }, // DD-101
    {fsave      ,{mkop('E','fs'),noop          ,noop          }}, // DD-110
    {fstsw      ,{mkop('E','w') ,noop          ,noop          }}, // DD-111

    {ffree      ,{mkreg(st0)    ,noop          ,noop          }}, // DD C0
    {ffree      ,{mkreg(st1)    ,noop          ,noop          }}, // DD C1
    {ffree      ,{mkreg(st2)    ,noop          ,noop          }}, // DD C2
    {ffree      ,{mkreg(st3)    ,noop          ,noop          }}, // DD C3
    {ffree      ,{mkreg(st4)    ,noop          ,noop          }}, // DD C4
    {ffree      ,{mkreg(st5)    ,noop          ,noop          }}, // DD C5
    {ffree      ,{mkreg(st6)    ,noop          ,noop          }}, // DD C6
    {ffree      ,{mkreg(st7)    ,noop          ,noop          }}, // DD C7

    {unkop                                                     }, // DD C8
    {unkop                                                     }, // DD C9
    {unkop                                                     }, // DD CA
    {unkop                                                     }, // DD CB
    {unkop                                                     }, // DD CC
    {unkop                                                     }, // DD CD
    {unkop                                                     }, // DD CE
    {unkop                                                     }, // DD CF

    {fst        ,{mkreg(st0)    ,noop          ,noop          }}, // DD D0
    {fst        ,{mkreg(st1)    ,noop          ,noop          }}, // DD D1
    {fst        ,{mkreg(st2)    ,noop          ,noop          }}, // DD D2
    {fst        ,{mkreg(st3)    ,noop          ,noop          }}, // DD D3
    {fst        ,{mkreg(st4)    ,noop          ,noop          }}, // DD D4
    {fst        ,{mkreg(st5)    ,noop          ,noop          }}, // DD D5
    {fst        ,{mkreg(st6)    ,noop          ,noop          }}, // DD D6
    {fst        ,{mkreg(st7)    ,noop          ,noop          }}, // DD D7

    {fstp       ,{mkreg(st0)    ,noop          ,noop          }}, // DD D8
    {fstp       ,{mkreg(st1)    ,noop          ,noop          }}, // DD D9
    {fstp       ,{mkreg(st2)    ,noop          ,noop          }}, // DD DA
    {fstp       ,{mkreg(st3)    ,noop          ,noop          }}, // DD DB
    {fstp       ,{mkreg(st4)    ,noop          ,noop          }}, // DD DC
    {fstp       ,{mkreg(st5)    ,noop          ,noop          }}, // DD DD
    {fstp       ,{mkreg(st6)    ,noop          ,noop          }}, // DD DE
    {fstp       ,{mkreg(st7)    ,noop          ,noop          }}, // DD DF

    {fucom      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DD E0
    {fucom      ,{mkreg(st1)    ,mkreg(st0)    ,noop          }}, // DD E1
    {fucom      ,{mkreg(st2)    ,mkreg(st0)    ,noop          }}, // DD E2
    {fucom      ,{mkreg(st3)    ,mkreg(st0)    ,noop          }}, // DD E3
    {fucom      ,{mkreg(st4)    ,mkreg(st0)    ,noop          }}, // DD E4
    {fucom      ,{mkreg(st5)    ,mkreg(st0)    ,noop          }}, // DD E5
    {fucom      ,{mkreg(st6)    ,mkreg(st0)    ,noop          }}, // DD E6
    {fucom      ,{mkreg(st7)    ,mkreg(st0)    ,noop          }}, // DD E7

    {fucomp     ,{mkreg(st0)    ,noop          ,noop          }}, // DD E8
    {fucomp     ,{mkreg(st1)    ,noop          ,noop          }}, // DD E9
    {fucomp     ,{mkreg(st2)    ,noop          ,noop          }}, // DD EA
    {fucomp     ,{mkreg(st3)    ,noop          ,noop          }}, // DD EB
    {fucomp     ,{mkreg(st4)    ,noop          ,noop          }}, // DD EC
    {fucomp     ,{mkreg(st5)    ,noop          ,noop          }}, // DD ED
    {fucomp     ,{mkreg(st6)    ,noop          ,noop          }}, // DD EE
    {fucomp     ,{mkreg(st7)    ,noop          ,noop          }}, // DD EF

    {unkop                                                     }, // DD F8
    {unkop                                                     }, // DD F9
    {unkop                                                     }, // DD FA
    {unkop                                                     }, // DD FB
    {unkop                                                     }, // DD FC
    {unkop                                                     }, // DD FD
    {unkop                                                     }, // DD FE
    {unkop                                                     }, // DD FF

    {unkop                                                     }, // DD F8
    {unkop                                                     }, // DD F9
    {unkop                                                     }, // DD FA
    {unkop                                                     }, // DD FB
    {unkop                                                     }, // DD FC
    {unkop                                                     }, // DD FD
    {unkop                                                     }, // DD FE
    {unkop                                                     }  // DD FF

    };

    fpu_table opcodes_DE = {

    {fiadd      ,{mkop('E','w') ,noop          ,noop          }}, // DE-000
    {fimul      ,{mkop('E','w') ,noop          ,noop          }}, // DE-001
    {ficom      ,{mkop('E','w') ,noop          ,noop          }}, // DE-010
    {ficomp     ,{mkop('E','w') ,noop          ,noop          }}, // DE-011
    {fisub      ,{mkop('E','w') ,noop          ,noop          }}, // DE-100
    {fisubr     ,{mkop('E','w') ,noop          ,noop          }}, // DE-101
    {fidiv      ,{mkop('E','w') ,noop          ,noop          }}, // DE-110
    {fidivr     ,{mkop('E','w') ,noop          ,noop          }}, // DE-111

    {faddp      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE C0
    {faddp      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE C1
    {faddp      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE C2
    {faddp      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE C3
    {faddp      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE C4
    {faddp      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE C5
    {faddp      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE C6
    {faddp      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE C7

    {fmulp      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE C8
    {fmulp      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE C9
    {fmulp      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE CA
    {fmulp      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE CB
    {fmulp      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE CC
    {fmulp      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE CD
    {fmulp      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE CE
    {fmulp      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE CF

    {unkop                                                     }, // DE D0
    {unkop                                                     }, // DE D1
    {unkop                                                     }, // DE D2
    {unkop                                                     }, // DE D3
    {unkop                                                     }, // DE D4
    {unkop                                                     }, // DE D5
    {unkop                                                     }, // DE D6
    {unkop                                                     }, // DE D7

    {unkop                                                     }, // DE D8
    {fcompp     ,{noop          ,noop          ,noop          }}, // DE D9
    {unkop                                                     }, // DE DA
    {unkop                                                     }, // DE DB
    {unkop                                                     }, // DE DC
    {unkop                                                     }, // DE DD
    {unkop                                                     }, // DE DE
    {unkop                                                     }, // DE DF

    {fsubrp     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE E0
    {fsubrp     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE E1
    {fsubrp     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE E2
    {fsubrp     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE E3
    {fsubrp     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE E4
    {fsubrp     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE E5
    {fsubrp     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE E6
    {fsubrp     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE E7

    {fsubp      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE E8
    {fsubp      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE E9
    {fsubp      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE EA
    {fsubp      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE EB
    {fsubp      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE EC
    {fsubp      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE ED
    {fsubp      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE EE
    {fsubp      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE EF

    {fdivrp     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE F0
    {fdivrp     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE F1
    {fdivrp     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE F2
    {fdivrp     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE F3
    {fdivrp     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE F4
    {fdivrp     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE F5
    {fdivrp     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE F6
    {fdivrp     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE F7

    {fdivp      ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DE F8
    {fdivp      ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DE F9
    {fdivp      ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DE FA
    {fdivp      ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DE FB
    {fdivp      ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DE FC
    {fdivp      ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DE FD
    {fdivp      ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DE FE
    {fdivp      ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DE FF

    };

    fpu_table opcodes_DF = {

    {fild       ,{mkop('E','w') ,noop          ,noop          }}, // DF-000
    {fisttp     ,{mkop('E','w') ,noop          ,noop          }}, // DF-001
    {fist       ,{mkop('E','w') ,noop          ,noop          }}, // DF-010
    {fistp      ,{mkop('E','w') ,noop          ,noop          }}, // DF-011
    {fbld       ,{mkop('E','fb'),noop          ,noop          }}, // DF-100
    {fild       ,{mkop('E','q') ,noop          ,noop          }}, // DF-101
    {fbstp      ,{mkop('E','fb'),noop          ,noop          }}, // DF-110
    {fistp      ,{mkop('E','q') ,noop          ,noop          }}, // DF-111

    {unkop                                                     }, // DF C0
    {unkop                                                     }, // DF C1
    {unkop                                                     }, // DF C2
    {unkop                                                     }, // DF C3
    {unkop                                                     }, // DF C4
    {unkop                                                     }, // DF C5
    {unkop                                                     }, // DF C6
    {unkop                                                     }, // DF C7

    {unkop                                                     }, // DF C8
    {unkop                                                     }, // DF C9
    {unkop                                                     }, // DF CA
    {unkop                                                     }, // DF CB
    {unkop                                                     }, // DF CC
    {unkop                                                     }, // DF CD
    {unkop                                                     }, // DF CE
    {unkop                                                     }, // DF CF

    {unkop                                                     }, // DF D0
    {unkop                                                     }, // DF D1
    {unkop                                                     }, // DF D2
    {unkop                                                     }, // DF D3
    {unkop                                                     }, // DF D4
    {unkop                                                     }, // DF D5
    {unkop                                                     }, // DF D6
    {unkop                                                     }, // DF D7

    {unkop                                                     }, // DF D8
    {unkop                                                     }, // DF D9
    {unkop                                                     }, // DF DA
    {unkop                                                     }, // DF DB
    {unkop                                                     }, // DF DC
    {unkop                                                     }, // DF DD
    {unkop                                                     }, // DF DE
    {unkop                                                     }, // DF DF

    {fstsw      ,{mkreg(ax)     ,noop          ,noop          }}, // DF E0
    {unkop                                                     }, // DF E1
    {unkop                                                     }, // DF E2
    {unkop                                                     }, // DF E3
    {unkop                                                     }, // DF E4
    {unkop                                                     }, // DF E5
    {unkop                                                     }, // DF E6
    {unkop                                                     }, // DF E7

    {fucomip    ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DF E8
    {fucomip    ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DF E9
    {fucomip    ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DF EA
    {fucomip    ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DF EB
    {fucomip    ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DF EC
    {fucomip    ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DF ED
    {fucomip    ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DF EE
    {fucomip    ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DF EF

    {fcomip     ,{mkreg(st0)    ,mkreg(st0)    ,noop          }}, // DF F0
    {fcomip     ,{mkreg(st0)    ,mkreg(st1)    ,noop          }}, // DF F1
    {fcomip     ,{mkreg(st0)    ,mkreg(st2)    ,noop          }}, // DF F2
    {fcomip     ,{mkreg(st0)    ,mkreg(st3)    ,noop          }}, // DF F3
    {fcomip     ,{mkreg(st0)    ,mkreg(st4)    ,noop          }}, // DF F4
    {fcomip     ,{mkreg(st0)    ,mkreg(st5)    ,noop          }}, // DF F5
    {fcomip     ,{mkreg(st0)    ,mkreg(st6)    ,noop          }}, // DF F6
    {fcomip     ,{mkreg(st0)    ,mkreg(st7)    ,noop          }}, // DF F7

    {unkop                                                     }, // DF F8
    {unkop                                                     }, // DF F9
    {unkop                                                     }, // DF FA
    {unkop                                                     }, // DF FB
    {unkop                                                     }, // DF FC
    {unkop                                                     }, // DF FD
    {unkop                                                     }, // DF FE
    {unkop                                                     }  // DF FF

    };

    mrm_both_table opcodes_F6 = {

    {test       ,{mkop('E','b') ,mkop('I','b') ,noop          }}, // F6-000
    {unkop                                                     }, // F6-001
    {not_       ,{mkop('E','b') ,noop          ,noop          }}, // F6-010
    {neg        ,{mkop('E','b') ,noop          ,noop          }}, // F6-011
    {mul        ,{mkop('E','b') ,noop          ,noop          }}, // F6-100
    {imul       ,{mkop('E','b') ,noop          ,noop          }}, // F6-101
    {div        ,{mkop('E','b') ,noop          ,noop          }}, // F6-110
    {idiv       ,{mkop('E','b') ,noop          ,noop          }}  // F6-111

    };

    mrm_both_table opcodes_F7 = {

    {test       ,{mkop('E','v') ,mkop('I','v') ,noop          }}, // F7-000
    {unkop                                                     }, // F7-001
    {not_       ,{mkop('E','v') ,noop          ,noop          }}, // F7-010
    {neg        ,{mkop('E','v') ,noop          ,noop          }}, // F7-011
    {mul        ,{mkop('E','v') ,noop          ,noop          }}, // F7-100
    {imul       ,{mkop('E','v') ,noop          ,noop          }}, // F7-101
    {div        ,{mkop('E','v') ,noop          ,noop          }}, // F7-110
    {idiv       ,{mkop('E','v') ,noop          ,noop          }}  // F7-111

    };

    mrm_both_table opcodes_FE = {

    {inc        ,{mkop('E','b') ,noop          ,noop          }}, // FE-000
    {dec        ,{mkop('E','b') ,noop          ,noop          }}, // FE-001
    {unkop                                                     }, // FE-010
    {unkop                                                     }, // FE-011
    {unkop                                                     }, // FE-100
    {unkop                                                     }, // FE-101
    {unkop                                                     }, // FE-110
    {unkop                                                     }  // FE-111

    };

    mrm_both_table opcodes_FF = {

    {inc        ,{mkop('E','v') ,noop          ,noop          }}, // FF-000
    {dec        ,{mkop('E','v') ,noop          ,noop          }}, // FF-001
    {call       ,{mkop('E','v') ,noop          ,noop          }}, // FF-010
    {callf      ,{mkop('E','p') ,noop          ,noop          }}, // FF-011
    {jmp        ,{mkop('E','v') ,noop          ,noop          }}, // FF-100
    {jmp        ,{mkop('E','p') ,noop          ,noop          }}, // FF-101
    {push       ,{mkop('E','v') ,noop          ,noop          }}, // FF-110
    {unkop                                                     }  // FF-111

    };

    table opcodes = {

    {add        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 00
    {add        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 01
    {add        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 02
    {add        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 03
    {add        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 04
    {add        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 05
    {push       ,{mkreg(es)     ,noop          ,noop          }}, // 06
    {pop        ,{mkreg(es)     ,noop          ,noop          }}, // 07
    {or_        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 08
    {or_        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 09
    {or_        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 0A
    {or_        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 0B
    {or_        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 0C
    {or_        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 0D
    {push       ,{mkreg(cs)     ,noop          ,noop          }}, // 0E
    {mkotable(opcodes_0F)                                      }, // 0F
    {adc        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 10
    {adc        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 11
    {adc        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 12
    {adc        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 13
    {adc        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 14
    {adc        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 15
    {push       ,{mkreg(ss)     ,noop          ,noop          }}, // 16
    {pop        ,{mkreg(ss)     ,noop          ,noop          }}, // 17
    {sbb        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 18
    {sbb        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 19
    {sbb        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 1A
    {sbb        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 1B
    {sbb        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 1C
    {sbb        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 1D
    {push       ,{mkreg(ds)     ,noop          ,noop          }}, // 1E
    {pop        ,{mkreg(ds)     ,noop          ,noop          }}, // 1F
    {and_       ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 20
    {and_       ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 21
    {and_       ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 22
    {and_       ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 23
    {and_       ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 24
    {and_       ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 25
    {mkprefix                                                  }, // 26
    {daa        ,{noop          ,noop          ,noop          }}, // 27
    {sub        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 28
    {sub        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 29
    {sub        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 2A
    {sub        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 2B
    {sub        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 2C
    {sub        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 2D
    {mkprefix                                                  }, // 2E
    {das        ,{noop          ,noop          ,noop          }}, // 2F
    {xor_       ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 30
    {xor_       ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 31
    {xor_       ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 32
    {xor_       ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 33
    {xor_       ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 34
    {xor_       ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 35
    {mkprefix                                                  }, // 36
    {aaa        ,{noop          ,noop          ,noop          }}, // 37
    {cmp        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 38
    {cmp        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 39
    {cmp        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 3A
    {cmp        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 3B
    {cmp        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // 3C
    {cmp        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // 3D
    {mkprefix                                                  }, // 3E
    {aas        ,{noop          ,noop          ,noop          }}, // 3F
    {inc        ,{mkreg(e_ax)   ,noop          ,noop          }}, // 40
    {inc        ,{mkreg(e_cx)   ,noop          ,noop          }}, // 41
    {inc        ,{mkreg(e_dx)   ,noop          ,noop          }}, // 42
    {inc        ,{mkreg(e_bx)   ,noop          ,noop          }}, // 43
    {inc        ,{mkreg(e_sp)   ,noop          ,noop          }}, // 44
    {inc        ,{mkreg(e_bp)   ,noop          ,noop          }}, // 45
    {inc        ,{mkreg(e_si)   ,noop          ,noop          }}, // 46
    {inc        ,{mkreg(e_di)   ,noop          ,noop          }}, // 47
    {dec        ,{mkreg(e_ax)   ,noop          ,noop          }}, // 48
    {dec        ,{mkreg(e_cx)   ,noop          ,noop          }}, // 49
    {dec        ,{mkreg(e_dx)   ,noop          ,noop          }}, // 4A
    {dec        ,{mkreg(e_bx)   ,noop          ,noop          }}, // 4B
    {dec        ,{mkreg(e_sp)   ,noop          ,noop          }}, // 4C
    {dec        ,{mkreg(e_bp)   ,noop          ,noop          }}, // 4D
    {dec        ,{mkreg(e_si)   ,noop          ,noop          }}, // 4E
    {dec        ,{mkreg(e_di)   ,noop          ,noop          }}, // 4F
    {push       ,{mkreg(e_ax)   ,noop          ,noop          }}, // 50
    {push       ,{mkreg(e_cx)   ,noop          ,noop          }}, // 51
    {push       ,{mkreg(e_dx)   ,noop          ,noop          }}, // 52
    {push       ,{mkreg(e_bx)   ,noop          ,noop          }}, // 53
    {push       ,{mkreg(e_sp)   ,noop          ,noop          }}, // 54
    {push       ,{mkreg(e_bp)   ,noop          ,noop          }}, // 55
    {push       ,{mkreg(e_si)   ,noop          ,noop          }}, // 56
    {push       ,{mkreg(e_di)   ,noop          ,noop          }}, // 57
    {pop        ,{mkreg(e_ax)   ,noop          ,noop          }}, // 58
    {pop        ,{mkreg(e_cx)   ,noop          ,noop          }}, // 59
    {pop        ,{mkreg(e_dx)   ,noop          ,noop          }}, // 5A
    {pop        ,{mkreg(e_bx)   ,noop          ,noop          }}, // 5B
    {pop        ,{mkreg(e_sp)   ,noop          ,noop          }}, // 5C
    {pop        ,{mkreg(e_bp)   ,noop          ,noop          }}, // 5D
    {pop        ,{mkreg(e_si)   ,noop          ,noop          }}, // 5E
    {pop        ,{mkreg(e_di)   ,noop          ,noop          }}, // 5F
    {pusha      ,{noop          ,noop          ,noop          }}, // 60
    {popa       ,{noop          ,noop          ,noop          }}, // 61
    {bound      ,{mkop('G','v') ,mkop('M','a') ,noop          }}, // 62
    {arpl       ,{mkop('E','w') ,mkop('G','w') ,noop          }}, // 63
    {mkprefix                                                  }, // 64
    {mkprefix                                                  }, // 65
    {mkprefix                                                  }, // 66
    {mkprefix                                                  }, // 67
    {push       ,{mkop('I','v') ,noop          ,noop          }}, // 68
    {imul       ,{mkop('G','v') ,mkop('E','v') ,mkop('I','v') }}, // 69
    {push       ,{mkop('I','b') ,noop          ,noop          }}, // 6A
    {imul       ,{mkop('G','v') ,mkop('E','v') ,mkop('I','b') }}, // 6B
    {ins        ,{mkop('Y','b') ,mkreg(dx)     ,noop          }}, // 6C
    {ins        ,{mkop('Y','v') ,mkreg(dx)     ,noop          }}, // 6D
    {outs       ,{mkreg(dx)     ,mkop('X','b') ,noop          }}, // 6E
    {outs       ,{mkreg(dx)     ,mkop('X','v') ,noop          }}, // 6F
    {jo         ,{mkop('J','b') ,noop          ,noop          }}, // 70
    {jno        ,{mkop('J','b') ,noop          ,noop          }}, // 71
    {jb         ,{mkop('J','b') ,noop          ,noop          }}, // 72
    {jnb        ,{mkop('J','b') ,noop          ,noop          }}, // 73
    {jz         ,{mkop('J','b') ,noop          ,noop          }}, // 74
    {jnz        ,{mkop('J','b') ,noop          ,noop          }}, // 75
    {jbe        ,{mkop('J','b') ,noop          ,noop          }}, // 76
    {jnbe       ,{mkop('J','b') ,noop          ,noop          }}, // 77
    {js         ,{mkop('J','b') ,noop          ,noop          }}, // 78
    {jns        ,{mkop('J','b') ,noop          ,noop          }}, // 79
    {jp         ,{mkop('J','b') ,noop          ,noop          }}, // 7A
    {jnp        ,{mkop('J','b') ,noop          ,noop          }}, // 7B
    {jl         ,{mkop('J','b') ,noop          ,noop          }}, // 7C
    {jnl        ,{mkop('J','b') ,noop          ,noop          }}, // 7D
    {jle        ,{mkop('J','b') ,noop          ,noop          }}, // 7E
    {jnle       ,{mkop('J','b') ,noop          ,noop          }}, // 7F
    {mkmrmbothtbl(opcodes_80)                                  }, // 80
    {mkmrmbothtbl(opcodes_81)                                  }, // 81
    {mkmrmbothtbl(opcodes_82)                                  }, // 82
    {mkmrmbothtbl(opcodes_83)                                  }, // 83
    {test       ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 84
    {test       ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 85
    {xchg       ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 86
    {xchg       ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 87
    {mov        ,{mkop('E','b') ,mkop('G','b') ,noop          }}, // 88
    {mov        ,{mkop('E','v') ,mkop('G','v') ,noop          }}, // 89
    {mov        ,{mkop('G','b') ,mkop('E','b') ,noop          }}, // 8A
    {mov        ,{mkop('G','v') ,mkop('E','v') ,noop          }}, // 8B
    {mov        ,{mkop('E','w') ,mkop('S','w') ,noop          }}, // 8C
    {lea        ,{mkop('G','v') ,mkop('M', 0)  ,noop          }}, // 8D
    {mov        ,{mkop('S','w') ,mkop('E','w') ,noop          }}, // 8E
    {pop        ,{mkop('E','v') ,noop          ,noop          }}, // 8F
    {mkptable(opcodes_90)                                      }, // 90
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_cx)   ,noop          }}, // 91
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_dx)   ,noop          }}, // 92
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_bx)   ,noop          }}, // 93
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_sp)   ,noop          }}, // 94
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_bp)   ,noop          }}, // 95
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_si)   ,noop          }}, // 96
    {xchg       ,{mkreg(e_ax)   ,mkreg(e_di)   ,noop          }}, // 97
    {cbw        ,{noop          ,noop          ,noop          }}, // 98
    {cwd        ,{noop          ,noop          ,noop          }}, // 99
    {callf      ,{mkop('A','p') ,noop          ,noop          }}, // 9A
    {fwait      ,{noop          ,noop          ,noop          }}, // 9B
    {pushf      ,{noop          ,noop          ,noop          }}, // 9C
    {popf       ,{noop          ,noop          ,noop          }}, // 9D
    {sahf       ,{noop          ,noop          ,noop          }}, // 9E
    {lahf       ,{noop          ,noop          ,noop          }}, // 9F
    {mov        ,{mkreg(al)     ,mkop('O','b') ,noop          }}, // A0
    {mov        ,{mkreg(e_ax)   ,mkop('O','v') ,noop          }}, // A1
    {mov        ,{mkop('O','b') ,mkreg(al)     ,noop          }}, // A2
    {mov        ,{mkop('O','v') ,mkreg(e_ax)   ,noop          }}, // A3
    {movs       ,{mkop('X','b') ,mkop('Y','b') ,noop          }}, // A4
    {movs       ,{mkop('X','v') ,mkop('Y','v') ,noop          }}, // A5
    {cmps       ,{mkop('X','b') ,mkop('Y','b') ,noop          }}, // A6
    {cmps       ,{mkop('X','v') ,mkop('Y','v') ,noop          }}, // A7
    {test       ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // A8
    {test       ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // A9
    {stos       ,{mkop('Y','b') ,mkreg(al)     ,noop          }}, // AA
    {stos       ,{mkop('Y','v') ,mkreg(e_ax)   ,noop          }}, // AB
    {lods       ,{mkreg(al)     ,mkop('X','b') ,noop          }}, // AC
    {lods       ,{mkreg(e_ax)   ,mkop('X','v') ,noop          }}, // AD
    {scas       ,{mkreg(al)     ,mkop('Y','b') ,noop          }}, // AE
    // Intel IA32 Specification Volume II had a mistake concering the
    // following instruction (it used X addressing mode)
    {scas       ,{mkreg(e_ax)   ,mkop('Y','v') ,noop          }}, // AF
    {mov        ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // B0
    {mov        ,{mkreg(cl)     ,mkop('I','b') ,noop          }}, // B1
    {mov        ,{mkreg(dl)     ,mkop('I','b') ,noop          }}, // B2
    {mov        ,{mkreg(bl)     ,mkop('I','b') ,noop          }}, // B3
    {mov        ,{mkreg(ah)     ,mkop('I','b') ,noop          }}, // B4
    {mov        ,{mkreg(ch)     ,mkop('I','b') ,noop          }}, // B5
    {mov        ,{mkreg(dh)     ,mkop('I','b') ,noop          }}, // B6
    {mov        ,{mkreg(bh)     ,mkop('I','b') ,noop          }}, // B7
    {mov        ,{mkreg(e_ax)   ,mkop('I','v') ,noop          }}, // B8
    {mov        ,{mkreg(e_cx)   ,mkop('I','v') ,noop          }}, // B9
    {mov        ,{mkreg(e_dx)   ,mkop('I','v') ,noop          }}, // BA
    {mov        ,{mkreg(e_bx)   ,mkop('I','v') ,noop          }}, // BB
    {mov        ,{mkreg(e_sp)   ,mkop('I','v') ,noop          }}, // BC
    {mov        ,{mkreg(e_bp)   ,mkop('I','v') ,noop          }}, // BD
    {mov        ,{mkreg(e_si)   ,mkop('I','v') ,noop          }}, // BE
    {mov        ,{mkreg(e_di)   ,mkop('I','v') ,noop          }}, // BF
    {mkmrmbothtbl(opcodes_C0)                                  }, // C0
    {mkmrmbothtbl(opcodes_C1)                                  }, // C1
    {retn       ,{mkop('I','w') ,noop          ,noop          }}, // C2
    {retn       ,{noop          ,noop          ,noop          }}, // C3
    {les        ,{mkop('G','v') ,mkop('M','p') ,noop          }}, // C4
    {lds        ,{mkop('G','v') ,mkop('M','p') ,noop          }}, // C5
    {mkmrmbothtbl(opcodes_C6)                                  }, // C6
    {mkmrmbothtbl(opcodes_C7)                                  }, // C7
    {enter      ,{mkop('I','w') ,mkop('I','b') ,noop          }}, // C8
    {leave      ,{noop          ,noop          ,noop          }}, // C9
    {retf       ,{mkop('I','w') ,noop          ,noop          }}, // CA
    {retf       ,{noop          ,noop          ,noop          }}, // CB
    {int3       ,{noop          ,noop          ,noop          }}, // CC
    {int_       ,{mkop('I','b') ,noop          ,noop          }}, // CD
    {into       ,{noop          ,noop          ,noop          }}, // CE
    {iret       ,{noop          ,noop          ,noop          }}, // CF
    {mkmrmbothtbl(opcodes_D0)                                  }, // D0
    {mkmrmbothtbl(opcodes_D1)                                  }, // D1
    {mkmrmbothtbl(opcodes_D2)                                  }, // D2
    {mkmrmbothtbl(opcodes_D3)                                  }, // D3
    {aam        ,{mkop('I','b') ,noop          ,noop          }}, // D4
    {aad        ,{mkop('I','b') ,noop          ,noop          }}, // D5
    {unkop                                                     }, // D6
    {xlat       ,{noop          ,noop          ,noop          }}, // D7
    {mkfputable(opcodes_D8)                                    }, // D8
    {mkfputable(opcodes_D9)                                    }, // D9
    {mkfputable(opcodes_DA)                                    }, // DA
    {mkfputable(opcodes_DB)                                    }, // DB
    {mkfputable(opcodes_DC)                                    }, // DC
    {mkfputable(opcodes_DD)                                    }, // DD
    {mkfputable(opcodes_DE)                                    }, // DE
    {mkfputable(opcodes_DF)                                    }, // DF
    {loopne     ,{mkop('J','b') ,noop          ,noop          }}, // E0
    {loope      ,{mkop('J','b') ,noop          ,noop          }}, // E1
    {loop       ,{mkop('J','b') ,noop          ,noop          }}, // E2
    {jcxz       ,{mkop('J','b') ,noop          ,noop          }}, // E3
    {in         ,{mkreg(al)     ,mkop('I','b') ,noop          }}, // E4
    {in         ,{mkreg(e_ax)   ,mkop('I','b') ,noop          }}, // E5
    {out        ,{mkop('I','b') ,mkreg(al)     ,noop          }}, // E6
    {out        ,{mkop('I','b') ,mkreg(e_ax)   ,noop          }}, // E7
    {call       ,{mkop('J','v') ,noop          ,noop          }}, // E8
    {jmp        ,{mkop('J','v') ,noop          ,noop          }}, // E9
    {jmp        ,{mkop('A','p') ,noop          ,noop          }}, // EA
    {jmp        ,{mkop('J','b') ,noop          ,noop          }}, // EB
    {in         ,{mkreg(al)     ,mkreg(dx)     ,noop          }}, // EC
    {in         ,{mkreg(e_ax)   ,mkreg(dx)     ,noop          }}, // ED
    {out        ,{mkreg(dx)     ,mkreg(al)     ,noop          }}, // EE
    {out        ,{mkreg(dx)     ,mkreg(e_ax)   ,noop          }}, // EF
    {mkprefix                                                  }, // F0
    {unkop                                                     }, // F1
    {mkprefix                                                  }, // F2
    {mkprefix                                                  }, // F3
    {hlt        ,{noop          , noop         ,noop          }}, // F4
    {cmc        ,{noop          , noop         ,noop          }}, // F5
    {mkmrmbothtbl(opcodes_F6)                                  }, // F6
    {mkmrmbothtbl(opcodes_F7)                                  }, // F7
    {clc        ,{noop          ,noop          ,noop          }}, // F8
    {stc        ,{noop          ,noop          ,noop          }}, // F9
    {cli        ,{noop          ,noop          ,noop          }}, // FA
    {sti        ,{noop          ,noop          ,noop          }}, // FB
    {cld        ,{noop          ,noop          ,noop          }}, // FC
    {std        ,{noop          ,noop          ,noop          }}, // FD
    {mkmrmbothtbl(opcodes_FE)                                  }, // FE
    {mkmrmbothtbl(opcodes_FF)                                  }  // FF

    };

    // OpcodeText table

    #define BRIZO_GEN_OPCODE(o, t) (t),

    extern const char *opcodes_text[] =
    {
        #include <brizo/detail/opcodes.h>
        0
    };

    #undef BRIZO_GEN_OPCODE
}

namespace Brizo_DE050324
{
    const opcode_entry & get_unknown_opcode()
    {
        return unknown_opcode;
    }
}

namespace Brizo_DE050324 { namespace detail {
    const opcode_entry *get_opcodes()
    {
        return opcodes;
    }

    const char *get_opcode_text(int idx)
    {
        return opcodes_text[idx];
    }
}}
