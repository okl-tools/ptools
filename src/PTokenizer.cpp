#include "PString.h"
#include "PTools.h"
#include "PTokenizer.h"

namespace ptools
{

    static PToken get_token (PString & ps, uint32_t posFrom=0, uint32_t countChars=MAX_UINT32)
    {
        if (posFrom >= ps.size())
        {
            //return PStringView(nullptr, 0);
        }

//        const char * p = data + posFrom;
        const char * p = ps.get_data();

        uint32_t partLen = ps.size() - posFrom;
        if (countChars < partLen)
        {
            partLen = countChars;
        }

        return PToken(p, partLen);
    }



    PTokenizer::PTokenizer (IObjectMemPool & pool) : pool(pool), arToks(pool), sLineTokens(pool)
    {
    }

    void PTokenizer::scan (const char * pStr, const char * delimiters, DELIMITER_MODE delimiterMode)
    {
        sLineTokens.cleanup();
        sLineTokens.write(pStr);


        int delimPos = 0, tokenPos = 0, pos = 0;

        if (sLineTokens.size() < 1)
        {
            return;
        }


        while (1)
        {
            delimPos = sLineTokens.find_first_of(delimiters, pos);
            tokenPos = sLineTokens.find_first_not_of(delimiters, pos);

            if (delimPos >=0)
            {
                if (tokenPos >=0)
                {
                    if (tokenPos < delimPos)
                    {
                        arToks.push_back(sLineTokens.get_view<PToken>(pos, delimPos - pos));
                    }
                }

                if (delimiterMode == DELIMITER_MODE::KEEP_DELIMITER)
                {
                    PToken tok = sLineTokens.get_view<PToken>(delimPos, 1);
                    tok.bits.set_flag(PToken::TOK_TYPE::DELIMITER); // set multi purpose user bits ....
                    arToks.push_back(tok);
                }

                pos = delimPos + 1;
            }
            else
            {
                if (tokenPos >=0)
                {
                    arToks.push_back(sLineTokens.get_view<PToken>(pos));
                }
                break;
            }
        }
    }
}