#pragma once

#include <stdint.h>
#include <stddef.h>

#include "PFormat.h"
#include "PWriter.h"
#include "PBits.h"
#include "IObjectMemPool.h"

namespace ptools
{
    struct PString;
    struct PStringView;

    struct AbstractString
    {
        virtual const char * get_data () const = 0;
        virtual uint32_t size () const = 0;
        virtual uint32_t get_pos () const = 0;
        virtual void set_pos (uint32_t pos)  = 0;

        bool starts_with (const char * pPart) const;
        bool contains (const char * pPart) const;


        // find ....
        int32_t find (const char * pSearch) const;
        int32_t find (char ch, uint32_t pos = MAX_UINT32) const;

        int32_t find_backwards (const char * pSearch) const;
        int32_t find_backwards (char ch) const;

        int32_t find_first_of (const char * pSearch, uint32_t pos = 0) const;
        int32_t find_first_not_of (const char * pSearch, uint32_t pos = 0) const;

        bool is_valid () const;
        bool empty () const; // ??

        PStringView find_view (const char * pSearch) const;


        // convert
        bool to_bool (uint32_t pos = 0, uint32_t len = MAX_UINT32) const;
        int32_t to_int (uint32_t pos = 0, uint32_t len = MAX_UINT32) const;
        float to_float (uint32_t pos = 0, uint32_t len = MAX_UINT32) const;

        PStringView get_file_extension () const;  // "txt" , "bin", ... or NOT-VALID

        PStringView get_string_view (uint32_t posFrom = 0, uint32_t countChars = MAX_UINT32) const;


        bool is_file_extension (const char * c_str);

        // if (is_string(pStr, "com", "bat", "exe"))
        template<typename... Rest>
        inline bool is_file_extension (const char * test, Rest... rest);


        bool equals (const char * pStr, uint32_t lenStr = MAX_UINT32) const;
        bool equals_ignore_case (const char * pStr, uint32_t lenStr = MAX_UINT32) const;

        bool equals (const PStringView & view) const;
        bool equals_ignore_case (const PStringView & view) const;

    };

    struct PStringView : AbstractString
    {
        const char * mem = nullptr;
        uint32_t sz = 0;

        enum class BORDER
        {
            CRLF // used to build the view  ( exclude CR LF)
        };

        PStringView ();
        PStringView (const char * mem, uint32_t sz = MAX_UINT32); // to \0 terminated string or sz if sz < MAX_UINT32
        PStringView (const char * mem, BORDER border);
        PStringView (const PString & s, BORDER border);

        uint32_t get_pos () const override{return 0;}
        void set_pos (uint32_t pos) override {}


        operator bool () const;

        PString to_PString (IObjectMemPool & pool) const;
        void to_PString (PString & ps) const;


        bool operator== (const char * pStr) const;
        bool operator== (const PStringView & view) const;

        void to_writer (PWriter & writer) const;

        int32_t to_int (uint32_t pos = 0, uint32_t len = MAX_UINT32) const;

        uint32_t size () const override;

        const char * get_data () const override;
    };

    struct AbstractModifyableString : AbstractString
    {
        virtual bool insert_at(uint32_t posAt, const char *pPart, uint32_t partLen) ;
        virtual bool reserve (uint32_t newCap){return true;}

        bool remove_chars(uint32_t pos, uint32_t countChars=1);
        bool remove_first_found_char(uint32_t pos, char ch);
        void remove_from_end_CRLF(); // ?? replace by zeros ?

    };


    //-------------------------------------------------------
    struct PString : PWriter, AbstractModifyableString
    {
        friend IObjectMemPool;

        PString (IObjectMemPool * pool = nullptr, const char * pStr = nullptr);
        PString (IObjectMemPool & pool, const char * pStr = nullptr);
        PString (PString && s);

        PString & operator= (PString && s);
        PString (const PString & s);
        PString & operator= (const PString & s);

        ~PString () override;

        void set_obj_mem_pool (IObjectMemPool * thePool);

        char at (uint32_t index) const;
        char operator[] (uint32_t index) const;

        // like std::ostream
        PString & operator<< (const char * pStr);

        // like std::ostream
        template<typename T>
        PString & operator<< (const T & val);

        operator bool () const;

        operator const char * () const;

        static PString * create (IObjectMemPool * f);

        PResult write_char (char ch);

        const char * to_string () const; // if null return ""
        const char * c_str () const; // calls to_string()
        const char * get_data () const override; // can return null !

        PResult write_mem (const char * pChars, uint32_t countChars);
        bool insert_at(uint32_t posAt, const char *pPart, uint32_t partLen) override;


        // ideal for tokenizer, token should be derived from PStringView .. see usage in Node.cpp / internal struct "JToken"
        template<typename T>
        T create_view (uint32_t posFrom = 0, uint32_t countChars = MAX_UINT32) const
        requires DERIVED<T, PStringView>;


        void cleanup ();

        void fill_with_rand_LETTERS (uint32_t count);

        PString & operator= (const char * str);
        PString & operator+= (const char * str);

        bool assign (const char * str);
        bool append (const char * str);

        bool operator== (const PString & s) const;
        bool operator== (const char * pStr) const;


        uint32_t size () const override;
        uint32_t capacity () const;

        bool reserve (uint32_t newCap) override;


        IObjectMemPool * get_pool ();

        uint32_t get_pos () const override;
        void set_pos (uint32_t pos) override;


    protected:
        char * data = nullptr;
        uint32_t len = 0;
        uint32_t cap = 0;
        IObjectMemPool * pool = nullptr;
    };

}


#include "inline/PString_inline.h"