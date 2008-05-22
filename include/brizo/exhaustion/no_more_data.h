// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: no_more_data.h $ $Author: Death $
// $Revision: 6 $ $Date: 9/07/05 2:48 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_NO_MORE_DATA_H
#define BRIZO_INC_NO_MORE_DATA_H

#include <stdexcept>

namespace Brizo_DE050324
{
    class no_more_data : public std::runtime_error
    {
    public:
        no_more_data() : std::runtime_error("no more data") {}
    };
}

#endif // BRIZO_INC_NO_MORE_DATA_H
