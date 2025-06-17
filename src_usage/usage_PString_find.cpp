#include "PTools.h"
#include "ObjectMemPool.h"


using namespace ptools;


static void test_find (IObjectMemPool & pool)
{
    const char * pStr = "Wenn der Hahn kräht auf dem Mist, ändert sich's Wetter oder es bleibt wie es ist !";
    pp(pStr);
    pp("-");

    PString ps(pool, pStr);

    const char * pSearch = "kräht";


    const char * pFind = string_find(pStr, pSearch);
    if (pFind)
    {
        pp("$", pFind);
    }

    pp("ps:<<<$>>>", ps);

    PStringView view = ps.find_view("kräht");
    if (view)
    {
        pp("$", view);
    }
    else
    {
        pp("no view:$", view);
    }

}


static void test_find_first_not_of (IObjectMemPool & pool)
{
//    PString text (pool, "jodel hello world");
    PString text(pool, "   \t  hello world");

//    int32_t pos = text.find_first_not_of("jo delx");
    int32_t pos = text.find_first_not_of(" \t");
    pp("pos:$", pos);
    if (pos >= 0)
    {
        pp("First vowel found at position: $ ($)", pos, text[pos]);
    }
    else
    {
        pp("not found");
    }

}

static void test_find_first (IObjectMemPool & pool)
{
    PString text(pool, "brrk hello world");
    const char * vowels = "aeiou";

    int32_t pos = text.find_first_of(vowels);
    pp("pos:$", pos);

    if (pos >= 0)
    {
        pp("First vowel found at position: $ ($)", pos, text[pos]);
    }
    else
    {
        pp("No vowels found.");
    }
}


static void test_find_2 (IObjectMemPool & pool)
{
    const char * pStr = "89 Enten und 45 Kamele und 88 Hühner sind da!";

    pp("s <<<$>>>", pStr);

    PString s(pool, pStr);

    int32_t i = s.find("Kamele");
    pp("i:$", i);


    i = s.find_first_of("m9aK", 3);
    pp("i:$", i);


    i = s.find_first_of("Kam9", 3);
    pp("i:$", i);

    i = s.find_first_not_of("89 Enten");
    pp("i:$", i);




}

ObjectMemPoolWrapper<16, 100, 100> pool;

int main ()
{

    test_find(pool);
//    test_find_first(pool);

//    test_find_first_not_of_std();
//    test_find_first_not_of(pool);
    //test_tokenizer1(pool);


    return 0;
}