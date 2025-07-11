// PWriter.h
//
// okl, 2025 June 24
//
// PWriter is an abstract base class which writes characters and character arrays
//  via multiple overloaded write(..) and powerful sprintF(..)
//  sprintF differs from C's sprintf - it has ONE type of format placeholder $ instead of %.... and is type safe
//
//
// PWriter
// |-PString
// |-PStringFixedSize
// |-PWriterConsole
// |-PWriterGeneric
//   |-PWriterSocket
//   |-PWriterUArt
//
// The pp(...) logging make use of PWriterUArt in embedded context and PWriterConsole within linux
//
// User defined structures which has a to_writer method can be easily written
//
// struct A { ....void to_writer (PWriter & writer) const; ... }
// A a;
// pp("our a object:$", a);
//
// concept: "TO_WRITER"
// This WORKS with C++ "concepts". // see concept TO_WRITER
// The idea is to extend user defined structs with a function which is than detected during compile time for efficient code!
//
//
// concept: "TO_STRING"
// concept: "TO_PSTRING"
// ----------------------------------------------------


#pragma once


#include <cstring>
#include <cstdio>
#include <stdint.h>
#include <concepts>
#include <cctype>

#include "PBasics.h"
#include "PFunction.h"
#include "PFormat.h"
#include "PResult.h"
#include "PMutex.h"

#ifdef PC_VERSION
#include <string> // linux testing only
#endif


namespace ptools
{
    struct PWriterStringBase;
    struct PWriter;
    struct IObjectMemPool;


    template<typename T> concept TO_PSTRING =
    requires (const T & t)
    {
        { t.to_pw_string() } -> std::derived_from<PWriterStringBase>;
    };

    template<typename T> concept TO_WRITER =
    requires (T t, PWriter & writer)
    {
        { t.to_writer(writer) };
    };


    template<typename T> concept SET_OBJ_MEM_POOL =
    requires (T t, IObjectMemPool * pool)
    {
        { t.set_obj_mem_pool(pool) };
    };


    struct PWriter
    {
        virtual PResult write_char (char ch) = 0;
        virtual PResult write_mem (const char * pChars, uint32_t sz) = 0;
        virtual PResult write_cstring (const char * pChars);

        PResult write_spaces (uint32_t space);
        PResult write_LF ();
        PResult write_CRLF ();

        virtual ~PWriter ()
        {
        }

        template<typename T>
        PResult write (const T & t);

        template<TOSTRING T>
        PResult write (const T & t);

        template<TO_PSTRING T>
        PResult write (const T & t);

        template<TO_WRITER WRITER>
        PResult write (const WRITER & t)
        {
            t.to_writer(*this);
            return PResult{};
        }


        PResult write (char * pStr);
        PResult write (const char * pStr);
        PResult write (const char ch);
        PResult write (const bool flag);
        PResult write (const float f, uint32_t decimals = 2);
        PResult write (const double f, uint32_t decimals = 2);

#ifdef PC_VERSION
        PResult write (const std::string & s);
#endif


        template<std::integral T>
        PResult write (const T & t);

        PResult write (const void * p);


        // sprintF*** family. type safe. "format" consists of what you want to be printed and $ placeholder for the objects.
        // ----------
        void sprintF (const char * pStr); // appends formatted string like - s.sprintf("value:$, height:$", 10, 4.567);
        void sprintLF (const char * format = ""); // appends formatted string + LF  ( ideal for html )
        void sprintCRLF (const char * format = ""); // appends formatted string + CR + LF ( ideal for http header)

        template<typename T, typename... ARGS>
        inline void sprintF (const char * format, const T & value, const ARGS... args);

        template<typename T, typename... ARGS>
        inline void sprintF (uint32_t space, const char * format, const T & value, const ARGS... args);

        template<typename T, typename... ARGS>
        inline void sprintLF (const char * format, const T & value, const ARGS... args);

        template<typename T, typename... ARGS>
        inline void sprintCRLF (const char * format, const T & value, const ARGS... args);
    };


    struct PWriterGeneric : PWriter
    {
        PResult write_char (char ch) override;
        PResult write_mem (const char * pChars, uint32_t sz) override;

        using TxFunc = void (*) (void * conn, const uint8_t * data, uint16_t len);

        void * conn = nullptr;
        TxFunc txFunc = nullptr;

        void set_context_and_func (void * conn, TxFunc userFunc);

    protected:

        PMutex mutexWriter;
    };

    struct PWriterSocket : PWriterGeneric
    {
        PWriterSocket (void * conn, TxFunc userFunc)
        {
            set_context_and_func(conn, userFunc);
        }
    };


    struct PTrailer
    {
        static const int SZ_TRAILLER = 60;
        char buf_trailler[SZ_TRAILLER];
        int counterLogMessage = 0;
        void updateTrailer ();

        virtual void write_trailer ()
        {
        }

        bool flagWriteTrailer = true;
        bool flagWriteTime = true;
    };

    struct PWriterUArt : PWriterGeneric, PTrailer
    {
        void write_trailer () override
        {
            if (flagWriteTrailer)
            {
                updateTrailer();
                write_cstring(buf_trailler);
            }
        }
    };


    struct PWriterConsole : PWriter, PTrailer
    {
        static void set_write_header(bool flagHeader=true);

        PWriterConsole ();

        PResult write_char (char ch) override;
        PResult write_mem (const char * pChars, uint32_t sz) override;

        void write_trailer () override;

    protected:
        void write_header();

        PMutex mutexWriter;

    };


    // useful to count bytes - before you allocate mem for an object
    struct PWriterNULL : PWriter
    {
        uint32_t countBytesWritten = 0;
        PResult write_char (char ch) override;
        PResult write_mem (const char * pChars, uint32_t sz) override;
    };


#ifdef PC_VERSION
    extern PWriterConsole logWriter;
#else
    extern PWriterUArt logWriter;
#endif

    void pp ();
    void pp (const char * pStr);
    void pp (int space, const char * pStr);


    template<typename First, typename... Rest>
    inline void pp (const char * pFormat, const First & first, const Rest & ... rest)
    {
        logWriter.write_trailer();
        logWriter.sprintF(pFormat, first, rest ...);
        logWriter.write_LF();
    }

    template<typename First, typename... Rest>
    inline void pp (int space, const char * pFormat, const First & first, const Rest & ... rest)
    {
        logWriter.write_trailer();
        logWriter.write_spaces(space);
        logWriter.sprintF(pFormat, first, rest ...);
        logWriter.write_LF();
    }

    //-----------------------------------------------------------------------------

}

#include "inline/PWriter_inline.h"


