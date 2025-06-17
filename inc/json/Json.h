#pragma once

#include "PTools.h"

namespace ptools::json
{

    struct Handler
    {
        virtual ~Handler () = default;

        virtual void start_object ()
        {
        }

        virtual void end_object ()
        {
        }

        virtual void start_array ()
        {
        }

        virtual void end_array ()
        {
        }

        virtual void key (const char * data, size_t len)
        {
        }

        virtual void string_value (const char * data, size_t len)
        {
        }

        virtual void number_value (const char * data, size_t len)
        {
        }

        virtual void boolean_value (bool value)
        {
        }

        virtual void null_value ()
        {
        }

        virtual void error (const char * message, size_t position)
        {
        }
    };




    // JSON-Scanner (similar to XML's SAX)
    struct Scanner
    {
        Scanner (const char * data, size_t size, Handler & handler);

        bool scan_json_data ();

        void debug_run();

    private:
        const char * data;
        size_t size;
        size_t pos;

        Handler & handler;

        void skip_whitespace ();

        char peek () const;
        char next ();
        bool match (char expected);

        bool parse_value ();
        bool parse_object ();
        bool parse_array ();
        bool parse_string (bool isKey = false);
        bool parse_number ();
        bool parse_literal (const char * literal, size_t len, void (Handler::*) (bool));
        bool parse_null ();

        void report_error (const char * message);
    };





} // namespace ptools::json
