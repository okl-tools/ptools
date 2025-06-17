// PArray.h
//
// okl, 2025 June 12
//
//
// PArray is a little like std::vector
// PArray is fully dynamic and managed by IMemoryPool.
// A PArray object can grow or shrink.
// For convenience, there are 2 show* methods to output objects.
// --------------------------------------------

#pragma once

#include <cstddef>
#include <stdint.h>
#include <utility>

#include "IObjectMemPool.h"

namespace ptools
{
    template<typename T>
    struct PArray
    {
        PArray (IObjectMemPool & pool, size_t initial_capacity = 4);
        PArray(const PArray & ar);
        ~PArray ();

        PArray & operator=(const PArray & ar);

        bool push_back (const T & value);
        bool emplace_back(T &&val);
        bool insert_at (size_t index, const T & value);

        bool remove_at (size_t index);

        void clear ();

        bool reserve (size_t newCapacity);

        T & at (uint32_t index);
        const T & at (uint32_t index) const;

        T & operator[] (size_t index);
        const T & operator[] (size_t index) const;


        T & back();
        T * last();

        T * begin ();
        T * end ();

        const T * begin () const;
        const T * end () const;

        size_t size () const;

        bool is_empty() const;

        size_t get_capacity () const;



        virtual void show (uint16_t space=0, int32_t countStart = 10, int32_t countEnd = 3);

        // output for special types - normally it's enough if user type implements "void to_writer (PWriter & writer) const;" - In this case "show" method is ok also
        using FPRINT = PFunction<void(uint32_t space, int32_t index, const T & t)>;
        virtual void show_objects (FPRINT fp, uint32_t space = 0, int32_t countStart = 10, int32_t countEnd = 3) const;


        IObjectMemPool & get_pool();

        void free_pointer_objects()
        {
            for (T p: *this)
            {
                pool.free_object(p);
            }
        }

    private:
        IObjectMemPool & pool;
        T * data = nullptr;
        size_t countItems = 0;
        size_t capacity = 0;
    };



}

#include "inline/PArray_inline.h"