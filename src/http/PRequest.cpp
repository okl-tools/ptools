#include "PRequest.h"
#include <cstring>

namespace ptools
{

    static PStringView trim_left (PStringView v)
    {
        const char * p = v.get_data();
        uint32_t len = v.size();

        while (len > 0 && (*p == ' ' || *p == '\t'))
        {
            ++p;
            --len;
        }

        return PStringView(p, len);
    }

    static PStringView extract_token (PStringView & input)
    {
        const char * p = input.get_data();
        uint32_t len = input.size();
        uint32_t i = 0;

        while (i < len && p[i] != ' ' && p[i] != '\r' && p[i] != '\n')
        {
            ++i;
        }

        PStringView token(p, i);

        // skip token + optional whitespace
        uint32_t skip = i;
        while ((skip < len) && (p[skip] == ' ' || p[skip] == '\t'))
        {
            ++skip;
        }

        input = PStringView(p + skip, len - skip);
        return token;
    }

    PRequest PRequest::create_request(const PStringView & view)
    {
        return create_request(view.get_data(), view.size());
    }


    PRequest PRequest::create_request (const char * buf, uint32_t lenBuf)
    {
        PRequest r;

        if (!buf || lenBuf == 0)
        {
            return r;
        }

        r.requestAll = PStringView(buf, lenBuf);
        PStringView rest = r.requestAll;

        // Erste Zeile parsen: METHOD PATH VERSION\r\n
        PStringView methodStr = extract_token(rest);
        PStringView pathStr = extract_token(rest);
        PStringView versionStr = extract_token(rest);

        // Methode erkennen
        if (methodStr == "GET")
        {
            r.method = METHOD::GET;
        }
        else if (methodStr == "POST")
        {
            r.method = METHOD::POST;
        }
        else if (methodStr == "PUT")
        {
            r.method = METHOD::PUT;
        }
        else if (methodStr == "DELETE")
        {
            r.method = METHOD::DEL;
        }
        else if (methodStr == "HEAD")
        {
            r.method = METHOD::HEAD;
        }
        else if (methodStr == "OPTIONS")
        {
            r.method = METHOD::OPTIONS;
        }
        else
        {
            return r;
        } // ungültig

        // Version erkennen
        if (versionStr == "HTTP/0.9")
            r.version = HTTP_VERSION::VER_0_9;
        else if (versionStr == "HTTP/1.0")
            r.version = HTTP_VERSION::VER_1_0;
        else if (versionStr == "HTTP/1.1")
            r.version = HTTP_VERSION::VER_1_1;
        else if (versionStr == "HTTP/2.0")
            r.version = HTTP_VERSION::VER_2_0;
        else if (versionStr == "HTTP/3.0")
            r.version = HTTP_VERSION::VER_3_0;
        else
            return r; // ungültig

        r.requestTarget = pathStr;

        return r;
    }

    void PRequest::show (uint32_t space) const
    {
        pp(space, "PRequest all: '$'", requestAll);
        pp(space, "      method: '$'", method_to_string(method));
        pp(space, "      target: '$'", requestTarget);
        pp(space, "     version: '$'", version_to_string(version));


    }

    const char * PRequest::method_to_string (PRequest::METHOD method)
    {
        switch (method)
        {
            case METHOD::GET: return "GET";
            case METHOD::POST: return "POST";
            case METHOD::DEL: return "DELETE";
            case METHOD::HEAD: return "HEAD";
            case METHOD::OPTIONS: return "OPTIONS";
            case METHOD::PUT: return "PUT";
        }

        return "unknown-method";
    }

    const char * PRequest::version_to_string (PRequest::HTTP_VERSION version)
    {
        switch (version)
        {
            case HTTP_VERSION::VER_0_9: return "0.9";
            case HTTP_VERSION::VER_1_0: return "1.0";
            case HTTP_VERSION::VER_1_1: return "1.1";
            case HTTP_VERSION::VER_2_0: return "2.0";
            case HTTP_VERSION::VER_3_0: return "2.3";
        }
        return "unknown-version";
    }

    bool PRequest::is_GET () const
    {
        return method == METHOD::GET;
    }

    bool PRequest::is_POST () const
    {
        return method == METHOD::POST;
    }

    bool PRequest::is_http_1_0 () const
    {
        return version == HTTP_VERSION::VER_1_0;
    }

    bool PRequest::is_http_1_1 () const
    {
        return version == HTTP_VERSION::VER_1_1;
    }

    bool PRequest::is_http_2_0 () const
    {
        return version == HTTP_VERSION::VER_2_0;
    }

} // namespace ptools
