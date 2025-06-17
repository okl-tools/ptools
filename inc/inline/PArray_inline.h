#pragma once

namespace ptools
{

    template<typename T>
    inline bool PArray<T>::push_back (const T & value)
    {
        if (countItems >= capacity)
        {
            if (!reserve(capacity * 2 + 1))
//                if (!reserve(capacity +1))
            {
                return false;
            }
        }

        new(&data[countItems]) T(value);
        ++countItems;
        return true;
    }


    template<typename T>
    inline void PArray<T>::show (uint16_t space, int32_t countStart, int32_t countEnd)
    {
        pp(space, "PArray, count:$", countItems);

        //if (partBegin >=0)

        if ((countStart + countEnd) < countItems)
        {
            for (int i = 0; i < countItems; i++)
            {
                if (i < countStart || i >= (countItems - countEnd))
                {
                    pp(space + 4, "[$]=$", i, data[i]);
                    //fp(i, data[i], false);
                }
                else if (i == countStart)
                {
                    pp(space + 4, "...");
                    //fp(i, data[i], true);
                }
            }
        }
        else
        {
            for (int i = 0; i < countItems; i++)
            {
                pp(space + 4, "[$]=$", i, data[i]);
            }

        }

    }

    template<typename T>
    inline void PArray<T>::show_objects (PArray::FPRINT fp, uint32_t space, int32_t countStart, int32_t countEnd) const
    {
        pp(space, "PArray, count:$", size());

        if ((countStart + countEnd) < size())
        {
            for (int i = 0; i < size(); i++)
            {
                if (i < countStart || i >= (size() - countEnd))
                {
                    fp(space+4, i, at(i));
                }
                else if (i == countStart)
                {
                    pp(space + 4, "...");
                }
            }
        }
        else
        {
            for (int i = 0; i < size(); i++)
            {
                fp(space+4, i, at(i));
            }

        }

    }


    template<typename T>
    inline size_t PArray<T>::get_capacity () const
    {
        return capacity;
    }

    template<typename T>
    inline bool PArray<T>::is_empty () const
    {
        return countItems == 0;
    }

    template<typename T>
    inline size_t PArray<T>::size () const
    {
        return countItems;
    }

    template<typename T>
    inline const T * PArray<T>::end () const
    {
        return data + countItems;
    }

    template<typename T>
    inline const T * PArray<T>::begin () const
    {
        return data;
    }

    template<typename T>
    inline T * PArray<T>::end ()
    {
        return data + countItems;
    }

    template<typename T>
    inline T * PArray<T>::begin ()
    {
        return data;
    }

    template<typename T>
    inline T & PArray<T>::back ()
    {
        if (is_empty())
        {
            pp("ERROR, PArray::back, is empty!");
            static T t;
            return t;
        }
        return data[countItems-1];
    }

    template<typename T>
    inline T * PArray<T>::last ()
    {
        if (is_empty())
        {
            pp("ERROR, PArray::back, is empty!");
            return nullptr;
        }
        return &data[countItems-1];
    }

    template<typename T>
    inline const T & PArray<T>::operator[] (size_t index) const
    {
        return data[index];
    }

    template<typename T>
    inline T & PArray<T>::operator[] (size_t index)
    {
        return data[index];
    }

    template<typename T>
    inline const T & PArray<T>::at (uint32_t index) const
    {
        return data[index];
    }

    template<typename T>
    inline T & PArray<T>::at (uint32_t index)
    {
        return data[index];
    }

    template<typename T>
    inline bool PArray<T>::reserve (size_t newCapacity)
    {
        if (newCapacity <= capacity)
        {
            return true;
        }

        T * dataTmp = static_cast<T *>(pool.alloc(sizeof(T) * newCapacity));
        if (!dataTmp)
        {
            return false;
        }


        if (data)
        {
            for (size_t i = 0; i < countItems; ++i)
            {
                new(&dataTmp[i]) T(std::move(data[i]));
                data[i].~T();
            }
            pool.free_object(data);
        }

        data = dataTmp;
        capacity = newCapacity;


        return true;
    }

    template<typename T>
    inline void PArray<T>::clear ()
    {
        for (size_t i = 0; i < countItems; ++i)
        {
            data[i].~T();
        }
        countItems = 0;
    }

    template<typename T>
    inline bool PArray<T>::remove_at (size_t index)
    {
        if (index >= countItems)
        {
            return false;
        }

        data[index].~T();
        for (size_t i = index + 1; i < countItems; ++i)
        {
            new(&data[i - 1]) T(std::move(data[i]));
            data[i].~T();
        }

        --countItems;
        return true;
    }

    template<typename T>
    inline bool PArray<T>::insert_at (size_t index, const T & value)
    {
        if (index > countItems)
        {
            return false;
        } // erlaubt auch insert am Ende

        if (countItems >= capacity)
        {
            if (!reserve(capacity * 2 + 1))
                return false;
        }

        // Nach rechts verschieben (von hinten nach vorne!)
        for (size_t i = countItems; i > index; --i)
        {
            new(&data[i]) T(std::move(data[i - 1]));
            data[i - 1].~T();
        }

        new(&data[index]) T(value);
        ++countItems;
        return true;
    }

    template<typename T>
    inline bool PArray<T>::emplace_back (T && val)
    {
        if (countItems >= capacity)
        {
            if (!reserve(capacity * 2 + 1))
            {
                return false;
            }
        }

        new(&data[countItems]) T(std::move(val));

        ++countItems;
        return true;
    }

    template<typename T>
    inline PArray<T>::~PArray ()
    {
        clear();
        if (data)
        {
            pool.free_object(data);
        }
    }

    template<typename T>
    inline PArray<T> & PArray<T>::operator= (const PArray & ar)
    {
        if (ar.data && ar.countItems)
        {
            for (const T & val: ar)
            {
                push_back(val);
            }
        }
        return *this;
    }

    template<typename T>
    inline PArray<T>::PArray (const PArray & ar): pool(ar.pool)
    {
        if (ar.data && ar.countItems)
        {
            for (const T & val: ar)
            {
                push_back(val);
            }
        }
    }

    template<typename T>
    inline PArray<T>::PArray (IObjectMemPool & pool, size_t initial_capacity) : pool(pool)
    {
        reserve(initial_capacity);
    }

    template<typename T>
    inline IObjectMemPool & PArray<T>::get_pool ()
    {
        return pool;
    }

}