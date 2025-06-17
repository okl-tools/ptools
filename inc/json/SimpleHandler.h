#pragma once

#include "PTools.h"


namespace ptools::json
{
    // Very simple Handler for a Scanner object - shows idea of scanning json structure
    // SimpleHandler logs entry/exist points of parsed sections
    // Derive from ptools::json::Handler or use ptools::json::Node ( DOM like )
    // -------------------------------

    struct SimpleHandler : public ptools::json::Handler
    {
        static void pp_info (int depth, const char * type, const char * data, size_t len)
        {
            for (int i = 0; i < depth * 4; i++)
            {
                pp_string(" ");
            }
            pp_string(type);
            pp_print_buf(data, len);
            pp("");
        }

        int depth = 0;

        void start_object () override
        {
            pp(depth * 4, "start_object");
            depth++;
        }

        void end_object () override
        {
            depth--;
            pp(depth * 4, "end_object");
        }

        void start_array () override
        {
            pp(depth * 4, "start_array");
            depth++;
        }

        void end_array () override
        {
            depth--;
            pp(depth * 4, "end_array");
        }

        void key (const char * data, size_t len) override
        {
            pp_info(depth, "key:", data, len);

        }

        void string_value (const char * data, size_t len) override
        {
            pp_info(depth, "val(string):", data, len);
        }

        void number_value (const char * data, size_t len) override
        {
            pp_info(depth, "val (number):", data, len);
        }

        void boolean_value (bool value) override
        {
            pp(depth * 4, "val (boolean): $", value);
        }

        void null_value () override
        {
            pp(depth * 4, "val (null_value)");
        }

        void error (const char * message, size_t position) override
        {
            pp(depth * 4, "ERROR at $: $", position, message);
        }
    };


}
