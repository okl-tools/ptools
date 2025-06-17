//

#pragma once

#include <cstddef>
#include <stdint.h>
#include <utility>

#include "PArray.h"

namespace ptools
{

    template<typename T>
    struct PStack : public PArray<T>
    {
        using PArray<T>::PArray; // erbt Konstruktor von DynArray<T>

        bool push (const T & value)
        {
            return this->push_back(value);
        }

        bool pop ()
        {
            if (this->size() == 0)
            {
                return false;
            }
            return this->remove_at(this->size() - 1);
        }

        T *top_object ()
        {
            if (this->size() == 0)
            {
                pp("ERROR, stack already empty");
                return nullptr;
            }
            return &(*this)[this->size() - 1];
        }
        T top ()
        {
            if (this->size() == 0)
            {
                pp("ERROR, stack already empty");
                static T t;
                return t;
            }
            return (*this)[this->size() - 1];
        }

        const T top () const
        {
            if (this->size() == 0)
            {
                pp("ERROR, stack already empty");
                static T t;
                return t;
            }
            return (*this)[this->size() - 1];
        }

        bool is_empty () const
        {
            return this->size() == 0;
        }


        void show (uint16_t space, int32_t countStart = 3, int32_t countEnd = 3) override
        {
            pp(space, "Stack, count:$", this->size());

            for (size_t i = 0; i < this->size(); ++i)
            {
                pp(space + 4, "[$]=$", i, (*this)[i]);
            }
        }

    };
} // namespace ptools
