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
    inline T PString::get_view (uint32_t posFrom, uint32_t countChars) const
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

}