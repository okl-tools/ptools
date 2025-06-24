#pragma once

namespace ptools
{
    template<typename... Rest>
    inline bool AbstractString::is_file_extension (const char * test, Rest... rest)
    {
        if (!test)
        {
            return false;
        }

        return string_compare(get_data(), test) == 0 || is_file_extension(get_data(), rest...);
//        if (string_compare(get_data(), test) == 0)
//        {
//            return true;
//        }
//
//        return is_file_extension(get_data(), rest...);
    }



    template<typename T>
    inline T PString::create_view (uint32_t posFrom, uint32_t countChars) const
    requires DERIVED<T, PStringView>
    {
        if (posFrom >= size())
        {
            return T(nullptr, 0);
        }

        const char * p = data + posFrom;

        uint32_t partLen = len - posFrom;
        if (countChars < partLen)
        {
            partLen = countChars;
        }

        return T(p, partLen);
    }

    template<typename T>
    inline PString & PString::operator<< (const T & val)
    {
        write(val);
        return *this;
    }

    inline PString::operator const char * () const
    {
        return to_string();
    }


    inline PString * IObjectMemPool::create_PString (const char * pStr)
    {
        PString * s = create_object<PString>();
        if (s)
        {
            s->set_obj_mem_pool(this);
            if (pStr)
            {
                s->write_cstring(pStr);
            }
        }

        return s;
    }


    template<typename T, typename... ARGS>
    inline PString * IObjectMemPool::create_PString (const char * format, const T & value, const ARGS... args)
    {
        PString * s = create_object<PString>();
        if (s)
        {
            s->set_obj_mem_pool(this);
            s->sprintF(format, value, args ...);
        }

        return s;
    }

}