#include "PString.h"
#include "PTools.h"

namespace ptools
{

//    PString * IObjectMemPool::create_PString (const char * pStr)
//    {
//        PString * s = create_object<PString>();
//        if (s)
//        {
//            s->set_obj_mem_pool(this);
//            if (pStr)
//            {
//                s->sprintF("$", pStr);
//            }
//        }
//        return s;
//    }


    PString * PString::create (IObjectMemPool * thePool)
    {
        PString * s = thePool->create_object<PString>();
        if (s)
        {
            s->pool = thePool;
        }
        return s;
    }


    PResult PString::write_char (char ch)
    {
        PResult res;

        if (!reserve(len + 1))
        {
            pp("PString::write_char - MEM PROBLEMS");
            res.err = 1;
            res.errClass = ERR_CLASS_MEMORY;
            return res;
        }

        data[len++] = ch;
        data[len] = 0;

        return res;
    }

    const char * PString::to_string () const
    {
        return data ?
               data :
               "";
    }

    const char * PString::c_str () const
    {
        return to_string();
    }

    PResult PString::write_mem (const char * pChars, uint32_t countChars)
    {
        if (pChars)
        {
            while (countChars > 0)
            {
                PResult res = write_char(*pChars++);
                if (res.isError())
                {
                    return res;
                }
                countChars--;
            }
        }

        return PResult();
    }


    void PString::cleanup ()
    {
        if (data)
        {
            pool->free_object(data);
            data = nullptr;
            len = 0;
            cap = 0;
        }
    }

    PString & PString::operator= (const char * str)
    {
        assign(str);
        return *this;
    }

    PString & PString::operator+= (const char * str)
    {
        append(str);
        return *this;
    }

    bool PString::assign (const char * str)
    {
        if (!str)
        {
            return false;
        }

        uint32_t l = static_cast<uint32_t>(strlen(str));
        if (!reserve(l))
        {
            return false;
        }

        std::memcpy(data, str, l);
        len = l;
        data[len] = 0;

        return true;
    }

    bool PString::append (const char * str)
    {
        if (!str)
        {
            return false;
        }

        uint32_t l = static_cast<uint32_t>(strlen(str));
        if (!reserve(len + l))
        {
            return false;
        }

        std::memcpy(data + len, str, l);
        len += l;
        data[len] = 0;

        return true;
    }

    uint32_t PString::size () const
    {
        return len;
    }

    uint32_t PString::capacity () const
    {
        return cap;
    }


    bool PString::reserve_exact (uint32_t newCap)
    {
//        pp("PString::reserve* $ bytes", newCap);

        if (pool->is_error_state())
        {
            return false;
        }

        if (newCap <= cap)
        {
            return true;
        }

        uint32_t newSize = newCap;

        int blocksNeed = newSize / pool->get_block_size();
        if ((newSize % pool->get_block_size()) > 0)
        {
            blocksNeed++;
        }

        newSize = blocksNeed * pool->get_block_size();


        char * newData = static_cast<char *>(pool->blocks_alloc(blocksNeed));
        if (!newData)
        {
            pp("PString::reserve_exact - newData == nullptr");
            return false;
        }

        memset(newData, 0, newSize);

        if (data)
        {
            memcpy(newData, data, cap);
            pool->free_object(data);
        }


        data = newData;
        cap = newSize;
        data[len] = 0;

        return true;
    }

    bool PString::reserve (uint32_t newCap)
    {

        if (pool->is_error_state())
        {
            return false;
        }

        if (newCap + 1 <= cap)
        {
            return true;
        }

//        pp("PString::reserve* $ bytes", newCap);


//        uint32_t newSize = (newCap + 15) & ~15; // nächstes 16er-Multiple
        uint32_t newSize = newCap + 1;
//        pp("PString::reserve, newSize:$", newSize);
//        pp(4, "PString::reserve, <<<$>>>", data);

//        newSize += 1; // to have a c_string '\0'
//        newSize += 4; // to have a c_string '\0'
//        newSize += 1; // to have a c_string '\0'

        int blocksNeed = newSize / pool->get_block_size();
        if ((newSize % pool->get_block_size()) > 0)
        {
            blocksNeed++;
        }

        newSize = blocksNeed * pool->get_block_size();


        char * newData = static_cast<char *>(pool->blocks_alloc(blocksNeed));
        if (!newData)
        {
            pp("PString::reserve - newData == nullptr");
            return false;
        }

        memset(newData, 0, newSize);

        if (data)
        {
            memcpy(newData, data, cap);
            pool->free_object(data);
        }


        data = newData;
        cap = newSize;
        data[len] = 0;

        return true;
    }

    void PString::fill_with_rand_LETTERS (uint32_t count)
    {
        for (uint32_t i = 0; i < count; i++)
        {
            PResult res = write_char((char) rand_next_int('A', 'Z'));
            if (res.isError())
            {
                pp("PString::fill_with_rand_LETTERS - ERROR");
                break;
            }
        }
    }


    PString::PString (IObjectMemPool * pool, const char * pStr) : pool(pool)
    {
        if (pStr)
        {
            write_mem(pStr, strlen(pStr));
        }
    }

    PString::PString (IObjectMemPool & pool, const char * pStr) : pool(&pool)
    {
        if (pStr)
        {
            write_mem(pStr, strlen(pStr));
        }
    }

    PString::PString (PString && s)
    {
        pool = s.pool;
        std::swap(cap, s.cap);
        std::swap(len, s.len);
        std::swap(data, s.data);
    }

    PString & PString::operator= (PString && s)
    {
        pool = s.pool;
        std::swap(cap, s.cap);
        std::swap(len, s.len);
        std::swap(data, s.data);

        return *this;
    }

    PString::PString (const PString & s)
    {
        pool = s.pool;
        if (s.data)
        {
            write_mem(s.data, s.len);
        }
    }

    PString & PString::operator= (const PString & s)
    {
        cleanup();
        pool = s.pool;
        if (s.data)
        {
            write_mem(s.data, s.len);
        }

        return *this;
    }


    PString::~PString ()
    {
        cleanup();
    }

    void PString::set_obj_mem_pool (IObjectMemPool * thePool)
    {
        pool = thePool;
    }


    PString::operator bool () const
    {
        return is_valid();
    }

    bool PString::operator== (const PString & s) const
    {
        if (data == nullptr || s.data == nullptr)
        {
            return false;
        }

        return string_compare(data, s.data) == 0;
    }

    bool PString::operator== (const char * pStr) const
    {
        return string_compare(data, pStr) == 0;
    }


    IObjectMemPool * PString::get_pool ()
    {
        return pool;
    }

    char PString::operator[] (uint32_t index) const
    {
        return at(index);
    }

    char PString::at (uint32_t index) const
    {
        if (!is_valid())
        {
            pp("ERROR, PString::at - pstring is not valid");
            return -1;
        }
        if (index >= size())
        {
            pp("ERROR, PString::at - index >= size(), $ >= $", index, size());
            return -1;
        }
        return data[index];
    }

    const char * PString::get_data () const
    {
        return data;
    }

    PString & PString::operator<< (const char * pStr)
    {
        write_cstring(pStr);
        return *this;
    }

//    PString::PString (const char * pStr)
//    {
//
//    }


}