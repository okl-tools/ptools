#include "json/Json.h"

namespace ptools::json
{

    Scanner::Scanner (const char * data_, size_t size_, Handler & handler_) : data{data_}, size{size_}, pos{0}, handler{handler_}
    {
    }

    bool Scanner::scan_json_data ()
    {
//        pp("Scanner::scan_json_data");

        skip_whitespace();

        if (!parse_value())
        {
            return false;
        }

        skip_whitespace();
        if (pos != size)
        {
            report_error("Extra get_data after JSON value");
            return false;
        }

//        pp("Scanner::~scan_json_data - DONE");

        return true;
    }

    void Scanner::skip_whitespace ()
    {
        while (pos < size && (data[pos] == ' ' || data[pos] == '\n' || data[pos] == '\r' || data[pos] == '\t'))
        {
            ++pos;
        }
    }

    char Scanner::peek () const
    {
        return (pos < size) ? data[pos] : '\0';
    }

    char Scanner::next ()
    {
        return (pos < size) ? data[pos++] : '\0';
    }

    bool Scanner::match (char expected)
    {
        if (peek() == expected)
        {
            ++pos;
            return true;
        }
        return false;
    }

    void Scanner::report_error (const char * message)
    {
        handler.error(message, pos);
    }

    bool Scanner::parse_value ()
    {
        skip_whitespace();

        char ch = peek();
        if (ch == '{')
        {
            return parse_object();
        }
        if (ch == '[')
        {
            return parse_array();
        }
        if (ch == '"')
        {
            return parse_string();
        }
        if ((ch >= '0' && ch <= '9') || ch == '-')
        {
            return parse_number();
        }
        if (ch == 't')
        {
            return parse_literal("true", 4, &Handler::boolean_value);
        }
        if (ch == 'f')
        {
            return parse_literal("false", 5, &Handler::boolean_value);
        }
        if (ch == 'n')
        {
            return parse_null();
        }

        pp("Scanner::parse_value, ch-int:$,  char '$'", (int)ch, ch);
        pp("pos:$", pos);

        report_error("Unexpected character while parsing value");
        return false;
    }

    bool Scanner::parse_object ()
    {
        if (!match('{'))
        {
            return false;
        }
        handler.start_object();
        skip_whitespace();

        if (match('}'))
        {
            handler.end_object();
            return true;
        }

        while (true)
        {
            skip_whitespace();
            if (!parse_string(true))
            {
                return false;
            } // key
            skip_whitespace();

            if (!match(':'))
            {
                report_error("Expected ':' after key in object");
                return false;
            }

            if (!parse_value())
                return false;

            skip_whitespace();
            if (match('}'))
                break;
            if (!match(','))
            {
                report_error("Expected ',' or '}' in object");
                return false;
            }
        }

        handler.end_object();
        return true;
    }

    bool Scanner::parse_array ()
    {
        if (!match('['))
        {
            return false;
        }
        handler.start_array();
        skip_whitespace();

        if (match(']'))
        {
            handler.end_array();
            return true;
        }

        while (true)
        {
            if (!parse_value())
            {
                return false;
            }
            skip_whitespace();
            if (match(']'))
            {
                break;
            }
            if (!match(','))
            {
                report_error("Expected ',' or ']' in array");
                return false;
            }
        }

        handler.end_array();
        return true;
    }

    bool Scanner::parse_string (bool isKey)
    {
        if (!match('"'))
        {
            report_error("Expected '\"' at start of string");
            return false;
        }

        size_t start = pos;
        while (pos < size)
        {
            char ch = data[pos++];
            if (ch == '"')
            {
                size_t len = pos - start - 1;
                if (isKey)
                {
                    handler.key(data + start, len);
                }
                else
                {
                    handler.string_value(data + start, len);
                }
                return true;
            }
            if (ch == '\\')
            {
                pos++;
            } // Skip escaped character
        }

        report_error("Unterminated string");
        return false;
    }

    bool Scanner::parse_number ()
    {
        size_t start = pos;
        if (data[pos] == '-')
        {
            pos++;
        }
        while (pos < size && data[pos] >= '0' && data[pos] <= '9')
            pos++;
        if (pos < size && data[pos] == '.')
        {
            pos++;
            while (pos < size && data[pos] >= '0' && data[pos] <= '9')
                pos++;
        }
        size_t len = pos - start;
        if (len == 0)
        {
            report_error("Invalid number");
            return false;
        }
        handler.number_value(data + start, len);
        return true;
    }

    bool Scanner::parse_literal (const char * literal, size_t len, void (Handler::*callback) (bool))
    {
        if (pos + len > size || strncmp(data + pos, literal, len) != 0)
        {
            report_error("Invalid literal");
            return false;
        }
        pos += len;
        bool value = (literal[0] == 't');
        (handler.*callback)(value);
        return true;
    }

    bool Scanner::parse_null ()
    {
        if (pos + 4 > size || strncmp(data + pos, "null", 4) != 0)
        {
            report_error("Invalid null literal");
            return false;
        }
        pos += 4;
        handler.null_value();
        return true;
    }

    void Scanner::debug_run ()
    {
        pos = 0;
        while (pos < size)
        {
            char ch = peek();
            pp("Scanner::debug_run, ch-int:$,  char '$'", (int)ch, ch);
            ++pos;
        }
    }

} // namespace ptools::json
