#pragma once

#include <stdint.h>

#include "PString.h"

namespace ptools
{
    struct PRequest
    {
        enum class METHOD
        {
            GET,
            POST,
            PUT,
            DEL,
            HEAD,
            OPTIONS
        };

        enum class HTTP_VERSION
        {
            VER_0_9,
            VER_1_0,
            VER_1_1,
            VER_2_0,
            VER_3_0
        };

        static PRequest create_request(const char* buf, uint32_t lenBuf);
        static PRequest create_request(const PStringView & view);

        // original request like "GET /index.html HTTP/1.1" exclude CRLF
        PStringView requestAll;

        // extracted path, like "/api/led"
        PStringView requestTarget;

        METHOD method = METHOD::GET;
        HTTP_VERSION version = HTTP_VERSION::VER_1_0;

        bool is_GET() const;
        bool is_POST() const;

        bool is_http_1_0() const;
        bool is_http_1_1() const;
        bool is_http_2_0() const;

        // show on console............
        static const char * method_to_string(METHOD method);
        static const char * version_to_string(HTTP_VERSION version);

        void show(uint32_t space=0) const;


    };
}
