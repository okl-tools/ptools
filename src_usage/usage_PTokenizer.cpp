#include "PTools.h"
#include "ObjectMemPool.h"
#include "PTokenizer.h"


using namespace ptools;


static void scan_for_tokens (IObjectMemPool & pool, PTokenizer::DELIMITER_MODE delimiterMode)
{
//    const char * pStr = "Red sky at night, sailor’s delight; red sky in morning, sailor’s warning."; // "A traditional English farmer's proverb about the weather."
    const char * pStr = "Wenn der Hahn kräht auf dem Mist, ändert sich's Wetter oder es bleibt wie es ist !"; // "A traditional German farmer's proverb about the weather."
    pp("$", pStr);

    PTokenizer toks(pool);

    toks.scan(pStr, " ,.:;", delimiterMode);

    pp("Token:$", toks.arToks.size());

    for (const PToken & view: toks.arToks)
    {
        if (view.is_delimiter())
        {
            pp("<<<$>>> - DELIM", view);
        }
        else
        {
            pp("<<<$>>>", view);
        }
    }

}



ObjectMemPoolWrapper<16,100,100> objMem;


int main ()
{
    scan_for_tokens(objMem, PTokenizer::DELIMITER_MODE::SKIP_DELIMITER );
//    scan_for_tokens(objMem, PTokenizer::DELIMITER_MODE::KEEP_DELIMITER );

    return 0;
}