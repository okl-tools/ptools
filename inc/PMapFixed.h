#pragma once

#include <stdint.h>
#include <stddef.h>
#include "PBasics.h"


namespace ptools
{

    template<typename K, typename V, size_t N>
    struct PMapFixed
    {
        struct Entry
        {
            K key;
            V val;
        };

        Entry entries[N]{};
        size_t count{0};

        bool insert (const K & key, const V & val);

        V * find (const K & key);
        const V * find (const K & key) const;
        size_t size () const;
        Entry * begin ();
        Entry * end ();
        const Entry * begin () const;
        const Entry * end () const;
    };


    struct CStrKey
    {
        const char * str;

        bool operator== (const CStrKey & other) const
        {
            return str_equal(str, other.str);
        }
    };



}

#include "inline/PMapFixed_inline.h"