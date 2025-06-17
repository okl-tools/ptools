// PHttpHeader.h

#pragma once


#include <stdint.h>

#include "PString.h"
#include "PRequest.h"
#include "PMap.h"



namespace ptools
{
    struct PHttpHeader
    {
        PRequest request; // 1st Line like "GET /index.html HTTP/1.1" exclude CRLF
        PMap <PStringView, PStringView> hmap; // key-val pairs like "Content-Length: 512"

        static PHttpHeader create_header(IObjectMemPool & pool, const char* buf, uint32_t lenBuf);

        // show on console............
        void show(uint32_t space=0) const;

    private:

        PHttpHeader (IObjectMemPool & pool);
    };



}