#pragma once

#include "PTools.h"
#include "PBits.h"
#include "IObjectMemPool.h"
#include "PString.h"
#include "PArray.h"

namespace ptools
{
    struct PToken : PStringView
    {
        enum TOK_TYPE
        {
            DELIMITER = 1 << 1
        };

        Bits32 bits; // // user bits ... to describe token

        using PStringView::PStringView;

        bool is_delimiter() const
        {
            return bits.is_flag(TOK_TYPE::DELIMITER);
        }
    };


    struct PTokenizer
    {
        enum class DELIMITER_MODE
        {
            KEEP_DELIMITER,
            SKIP_DELIMITER
        };

        PTokenizer (IObjectMemPool & pool);
        void scan(const char * pStr, const char * pDelims=",", DELIMITER_MODE delimiterMode=DELIMITER_MODE::SKIP_DELIMITER); // can be reused

        PArray<PToken> arToks;  // PToken/Views only

    private:
        IObjectMemPool & pool; // Memory pool for 'sLineTokens'
        PString sLineTokens; // holds the whole Buffer
    };

}
