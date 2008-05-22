// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: visitor.h $ $Author: Death $
// $Revision: 1 $ $Date: 7/02/05 20:01 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_VISITOR_H
#define BRIZO_INC_VISITOR_H

namespace Brizo_DE050324
{
    class visitor_base
    {
    public:
        virtual ~visitor_base() = 0;
    };

    inline visitor_base::~visitor_base()
    {
    }

    template<typename T>
    class visitor
    {
    public:
        virtual void visit(T & obj) = 0;
    };

    class visitable_base
    {
    public:
        virtual ~visitable_base() {}
        virtual void accept(visitor_base & guest) = 0;

    protected:
        template<typename T>
        static void accept_impl(T & obj, visitor_base & guest)
        {
            if (visitor<T> *v = dynamic_cast<visitor<T> *>(&guest))
                v->visit(obj);
            else
                ;
        }
    };

#define BRIZO_DEFINE_VISITABLE() \
    virtual void accept(visitor_base & guest) \
    { accept_impl(*this, guest); }
}

#endif // BRIZO_INC_VISITOR_H
