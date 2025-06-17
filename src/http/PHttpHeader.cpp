#include "PHttpHeader.h"

namespace ptools
{

    struct HeaderLineIterator
    {
        const char* pRequest;
        const char* pRequestLast;
        const char* p;
        uint32_t lenLine ;

        explicit HeaderLineIterator (const char * pRequest, uint32_t lenBuf) : pRequest(pRequest)
        {
            p = pRequest;
            lenLine = 0;
            pRequestLast = pRequest + lenBuf-1;
        }

        bool has_next() const
        {
            return p < pRequestLast;
        }

        bool is_next_char(uint32_t offset, char ch)
        {
            if ((p + offset) < pRequestLast)
            {
                return *(p + offset) == ch;
            }
            return false;
        }

        const char* next_line()
        {
            lenLine = 0;
            const char *pLine = p;

            while (p <= pRequestLast)
            {
                if (*p == '\r')
                {
                    if (is_next_char(1, '\n'))
                    {
                        p+=2;
                    }
                    break;
                }
                p++;
                lenLine++;
            }

            return pLine;
        }


        PStringView next_view()
        {
            const char * pLine = next_line();

            PStringView view (pLine, lenLine);
            if (view.size() == 0)
            {
                view.mem = nullptr;
            }
            return view;
        }

    };

    static PStringView trim_right(PStringView v)
    {
        const char* p = v.get_data();
        int32_t len = static_cast<int32_t>(v.size());

        while (len > 0 && (p[len - 1] == ' ' || p[len - 1] == '\t'))
        {
            --len;
        }

        return PStringView(p, static_cast<uint32_t>(len));
    }


    PHttpHeader::PHttpHeader (IObjectMemPool & pool): hmap(pool)
    {
    }

    PHttpHeader PHttpHeader::create_header(IObjectMemPool& pool, const char* buf, uint32_t lenBuf)
    {
        PHttpHeader hdr(pool);

        if (!buf || lenBuf == 0)
        {
            return hdr;
        }

        HeaderLineIterator hi(buf, lenBuf);

        PStringView line = hi.next_view();

        hdr.request = PRequest::create_request(line.get_data(), line.size());


        for (; line; line = hi.next_view())
        {
            if (line.size() == 0)
            {
                break; // empty line = end of header
            }

            const char* p = line.get_data();
            uint32_t len = line.size();

            uint32_t i = 0;
            while (i < len && p[i] != ':')
            {
                ++i;
            }

            if (i == len || i == 0)
            {
                continue; // line not valid
            }

            PStringView key(p, i);
            key = trim_right(key);

            const char* valPtr = p + i + 1;
            uint32_t valLen = len - i - 1;

            while (valLen > 0 && (*valPtr == ' ' || *valPtr == '\t'))
            {
                ++valPtr;
                --valLen;
            }

            PStringView val(valPtr, valLen);


            hdr.hmap.add(key, val);
        }



        return hdr;
    }

    void PHttpHeader::show (uint32_t space) const
    {
        pp(space, "PHttpHeader");
        request.show(space+4);
        hmap.show(space+4);
    }


}
