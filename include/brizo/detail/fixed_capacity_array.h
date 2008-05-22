// ---------------------------------------------------------------------------
// Brizo                                                              by DEATH
// ---------------------------------------------------------------------------
// $Workfile: fixed_capacity_array.h $ $Author: Death $
// $Revision: 7 $ $Date: 9/02/05 17:09 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_INC_FIXED_CAPACITY_ARRAY_H
#define BRIZO_INC_FIXED_CAPACITY_ARRAY_H

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstddef>
#include <cassert>

#include <boost/integer.hpp>

namespace Brizo_DE050324 { namespace detail {
    template<typename T, std::size_t N>
    class fixed_capacity_array
    {
    public:
        typedef T value_type;
        typedef T & reference;
        typedef const T & const_reference;
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef typename boost::int_max_value_t<N>::least difference_type;
        typedef typename boost::uint_value_t<N>::least size_type;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    public:
        fixed_capacity_array()
        : m_size(0)
        {
        }

        void push_back(value_type value)
        {
            if (m_size < N)
                m_array[m_size++] = value;
            else
                throw std::runtime_error("overflow");
        }

        iterator begin()
        {
            return m_array;
        }

        const_iterator begin() const
        {
            return m_array;
        }

        iterator end()
        {
            return m_array + m_size;
        }

        const_iterator end() const
        {
            return m_array + m_size;
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return *(end() - 1);
        }

        const_reference back() const
        {
            return *(end() - 1);
        }

        size_type size() const
        {
            return m_size;
        }

        size_type max_size() const
        {
            return N;
        }

        void swap(fixed_capacity_array & other)
        {
            using std::swap;
            size_type maximum = (std::max)(m_size, other.m_size);
            for (size_type i = 0; i < maximum; i++)
                swap(m_array[i], other.m_array[i]);
            swap(m_size, other.m_size);
        }

        bool empty() const
        {
            return m_size == 0;
        }

        const_reference at(size_type n) const
        {
            if (n < m_size)
                return m_array[n];
            else
                throw std::out_of_range("in at()");
        }

        reference at(size_type n)
        {
            const fixed_capacity_array & self = *this;
            return const_cast<reference>(self.at(n));
        }

        const_reference operator[] (size_type n) const
        {
            assert(n < N);
            return m_array[n];
        }

        reference operator[] (size_type n)
        {
            const fixed_capacity_array & self = *this;
            return const_cast<reference>(self[n]);
        }

    private:
        value_type m_array[N];
        size_type m_size;
    };

    template<typename T, std::size_t N>
    bool operator== (const fixed_capacity_array<T, N> & first, 
                     const fixed_capacity_array<T, N> & second)
    {
        return first.size() == second.size() &&
                std::equal(first.begin(), first.end(), second.begin());
    }

    template<typename T, std::size_t N>
    bool operator!= (const fixed_capacity_array<T, N> & first,
                     const fixed_capacity_array<T, N> & second)
    {
        return !(first == second);
    }

    template<typename T, std::size_t N>
    void swap(fixed_capacity_array<T, N> & first,
              fixed_capacity_array<T, N> & second)
    {
        first.swap(second);
    }

    template<typename T, std::size_t N>
    bool operator< (const fixed_capacity_array<T, N> & first,
                    const fixed_capacity_array<T, N> & second)
    {
        return std::lexicographical_compare(first.begin(), first.end(),
                                            second.begin(), second.end());
    }

    template<typename T, std::size_t N>
    bool operator> (const fixed_capacity_array<T, N> & first,
                    const fixed_capacity_array<T, N> & second)
    {
        return second < first;
    }

    template<typename T, std::size_t N>
    bool operator<= (const fixed_capacity_array<T, N> & first,
                     const fixed_capacity_array<T, N> & second)
    {
        return !(first > second);
    }

    template<typename T, std::size_t N>
    bool operator>= (const fixed_capacity_array<T, N> & first,
                     const fixed_capacity_array<T, N> & second)
    {
        return !(first < second);
    }
}}

#endif // BRIZO_INC_FIXED_CAPACITY_ARRAY_H
