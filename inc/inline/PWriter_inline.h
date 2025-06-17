#pragma once

namespace ptools
{
    template<typename T, typename... ARGS>
    inline void PWriter::sprintLF (const char * format, const T & value, const ARGS... args)
    {
        sprintF(format, value, args ...);
        write_LF();
    }

    template<typename T, typename... ARGS>
    inline void PWriter::sprintCRLF (const char * format, const T & value, const ARGS... args)
    {
        sprintF(format, value, args ...);
        write_CRLF();
    }

    template<typename T, typename... ARGS>
    inline void PWriter::sprintF (const char * format, const T & value, const ARGS... args)
    {
        for (; *format; format++)
        {
            if (*format == '$')
            {
                write(value);

                sprintF(format + 1, args...); // recursive call

                return;
            }
            write(*format);
        }
    }

    template<typename T, typename... ARGS>
    inline void PWriter::sprintF (uint32_t space, const char * format, const T & value, const ARGS... args)
    {
        while (space > 0)
        {
            write_char(' ');
            space--;
        }

        for (; *format; format++)
        {
            if (*format == '$')
            {
                write(value);

                sprintF(format + 1, args...); // recursive call

                return;
            }
            write(*format);
        }
    }

    template<std::integral T>
    inline PResult PWriter::write (const T & t)
    {
        char bufConvert[64];
        number_to_ascii(t, bufConvert);
        return write_cstring(bufConvert);
    }

    inline PResult PWriter::write (const void *p)
    {
        char bufConvert[64];
        uintptr_t addr = reinterpret_cast<uintptr_t>(p);
        get_hex_trimmed(addr, bufConvert);
        return write_cstring(bufConvert);
    }



    template<TO_PSTRING T>
    inline PResult PWriter::write (const T & t)
    {
        auto pw = t.to_pw_string();
        return write_cstring(pw.to_string());
    }

    template<TOSTRING T>
    inline PResult PWriter::write (const T & t)
    {
        return write_cstring(t.to_string());
    }


    template<typename T>
    inline PResult PWriter::write (const T & t)
    {
        return write_cstring("[unsupported]");
    }


}