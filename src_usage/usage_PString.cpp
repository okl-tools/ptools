#include "PTools.h"
#include "ObjectMemPool.h"


using namespace ptools;




static void test_insert (IObjectMemPool & pool)
{

    const char * pABC = "abcdefghijklmnopqrstuvwxyz";
    PString s(pool, pABC);
    pp("s <<<$>>>, size:$", s, s.size());

    const char * pPart = "-it is hot today-and it will be hotter tomorrow-";
    pp("insert <<<$>>> 5 x at pos 3", pPart);

    for (int i=0; i<5; i++)
    {
        s.insert_at(3, pPart, string_len(pPart));
    }


    pp("s <<<$>>>, size:$", s, s.size());


}

ObjectMemPoolWrapper<16, 100, 100> pool;

int main ()
{

    test_insert(pool);


    return 0;
}