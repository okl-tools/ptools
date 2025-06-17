// PArrayFixed.h

#pragma once

#include "PFormat.h"
#include "PFunction.h"


namespace ptools
{
    template<typename T>
    struct PArrayFixed
    {
        T * arElements = nullptr;
        uint32_t CAPACITY;
        uint32_t countElement = 0;


        using FPRINT = PFunction<void(uint32_t space, int32_t index, const T & t)>;

        PArrayFixed (void * pMem, uint32_t CAPACITY) : CAPACITY(CAPACITY)
        {
            arElements = static_cast<T *> (pMem);
        }

        bool push_back (const T & t)
        {
            if (countElement >= CAPACITY)
            {
                pp("PArray::push_back, out of space, countElement >= SIZE, $ >= $", countElement, CAPACITY);
                return false;
            }

            arElements[countElement++] = t;
            return true;
        }

        template<typename... Args>
        bool emplace_back(Args&&... args)
        {
            if (countElement >= CAPACITY)
            {
                pp("PArray::emplace_back, out of space, countElement >= SIZE, $ >= $", countElement, CAPACITY);
                return false;
            }

            new (&arElements[countElement]) T(std::forward<Args>(args)...);
            ++countElement;
//            pp("*** size:$", size());
            return true;
        }

        bool insert_at (uint32_t pos, const T & t)
        {
            if (countElement >= CAPACITY || pos > countElement)
            {
                pp("PArray::insert_at, pos:$, out of space, countElement >= SIZE, $ >= $", pos, countElement, CAPACITY);
                return false;
            }

            for (uint32_t i = countElement; i > pos; --i)
            {
                arElements[i] = arElements[i - 1];
            }

            arElements[pos] = t;
            ++countElement;
//            pp("*** size:$", size());

            return true;
        }

        bool delete_at (uint32_t pos)
        {
            if (pos >= countElement)
            {
                return false;
            }

            for (uint32_t i = pos; i < countElement - 1; ++i)
            {
                arElements[i] = arElements[i + 1];
            }

            --countElement;
//            pp("*** size:$", size());

            return true;
        }

        int32_t find (const T & tSearch)
        {
            for (uint32_t i = 0; i < countElement; ++i)
            {
                if (arElements[i] == tSearch)
                {
                    return static_cast<int32_t>(i);
                }
            }

            return -1;
        }

        void clear ()
        {
            countElement = 0;
        }

        uint32_t size () const
        {
            return countElement;
        }

        uint32_t capacity () const
        {
            return CAPACITY;
        }

        T & operator[] (uint32_t index)
        {
            return arElements[index];
        }

        const T & operator[] (uint32_t index) const
        {
            return arElements[index];
        }

        T & at (uint32_t index)
        {
            return arElements[index];
        }

        const T & at (uint32_t index) const
        {
            return arElements[index];
        }

        T * begin ()
        {
            return arElements;
        }

        T * end ()
        {
            return arElements + countElement;
        }

        const T * begin () const
        {
            return arElements;
        }

        const T * end () const
        {
            return arElements + countElement;
        }

        bool operator== (const PArrayFixed & other) const
        {
            if (countElement != other.countElement)
            {
                return false;
            }

            for (uint32_t i = 0; i < countElement; ++i)
            {
                if (!(arElements[i] == other.arElements[i]))
                {
                    return false;
                }
            }

            return true;
        }

        virtual void print_line(uint32_t space, int32_t index, const T & t) const
        {
            if constexpr (Copyable<T>)
            {
                pp(space, "[$]=$", index, t);
            }
        }

        virtual void show (uint32_t space = 0, int32_t countStart = 10, int32_t countEnd = 3) const
        {
            pp(space, "Array, count:$", countElement);

            if ((countStart + countEnd) < countElement)
            {
                for (int i = 0; i < countElement; i++)
                {
                    if (i < countStart || i >= (countElement - countEnd))
                    {
                        print_line(space+4, i, arElements[i]);
                        //pp(space + 4, "[$]=$", i, arElements[i]);
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
                for (int i = 0; i < countElement; i++)
                {
                    //pp(space + 4, "[$]=$", i, arElements[i]);
                    print_line(space+4, i, arElements[i]);
                }

            }

        }

        virtual void show_objects (FPRINT fp, uint32_t space = 0, int32_t countStart = 10, int32_t countEnd = 3) const
        {
            pp(space, "Array, count:$", countElement);


            if ((countStart + countEnd) < countElement)
            {
                for (int i = 0; i < countElement; i++)
                {
                    if (i < countStart || i >= (countElement - countEnd))
                    {
                        fp(space, i, arElements[i]);
                    }
                    else if (i == countStart)
                    {
                        pp(space + 4, "...");
                    }
                }
            }
            else
            {
                for (int i = 0; i < countElement; i++)
                {
                    fp(space, i, arElements[i]);
                }

            }

        }


        template<typename CompareEquals>
        int find_specific (CompareEquals cmpEquals, const T & t)
        {
            for (int i = 0; i < size(); i++)
            {
                if (cmpEquals(arElements[i], t))
                {
                    return i;
                }
            }

            return -1;
        }


        template<typename Compare>
        void sort (Compare cmp)
        {
            if (countElement < 2)
            {
                return;
            }
            struct StackFrame
            {
                int32_t left;
                int32_t right;
            };

            StackFrame arStack[CAPACITY];
            int32_t top = -1;

            arStack[++top] = {0, static_cast<int32_t>(countElement - 1)};

            while (top >= 0)
            {
                int32_t left = arStack[top].left;
                int32_t right = arStack[top--].right;

                if (left >= right)
                {
                    continue;
                }

                T pivot = arElements[right];
                int32_t i = left - 1;

                for (int32_t j = left; j < right; ++j)
                {
                    if (cmp(arElements[j], pivot))
                    {
                        ++i;
                        std::swap(arElements[i], arElements[j]);
                    }
                }

                ++i;
                std::swap(arElements[i], arElements[right]);

                arStack[++top] = {left, i - 1};
                arStack[++top] = {i + 1, right};
            }
        }

        void sort ()
        {
            if (countElement < 2)
            {
                return;
            }

            struct StackFrame
            {
                int32_t left;
                int32_t right;
            };
            StackFrame arStack[CAPACITY];  // worst case: log2(N) frames, aber wir reservieren max.
            int32_t top = -1;

            arStack[++top] = {0, static_cast<int32_t>(countElement - 1)};

            while (top >= 0)
            {
                int32_t left = arStack[top].left;
                int32_t right = arStack[top--].right;

                if (left >= right)
                {
                    continue;
                }

                T pivot = arElements[right];
                int32_t i = left - 1;

                for (int32_t j = left; j < right; ++j)
                {
                    if (arElements[j] < pivot)
                    {
                        ++i;
                        std::swap(arElements[i], arElements[j]);
                    }
                }

                ++i;
                std::swap(arElements[i], arElements[right]);

                arStack[++top] = {left, i - 1};
                arStack[++top] = {i + 1, right};
            }
        }


    };
}
