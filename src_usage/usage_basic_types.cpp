#include "PTools.h"
#include "ObjectMemPool.h"


using namespace ptools;


static void test_basic_types (IObjectMemPool & pool)
{
    int16_t *a = pool.create_object<int16_t >(100);
    int32_t *b = pool.create_object<int32_t >(200);
    float *c = pool.create_object<float>(3.14f);
    PString *d = pool.create_PString("$ goats on the $ enjoying grass", 10, "hill"); // $ universal placeholder for types

    pp("a:$", *a);
    pp("b:$", *b);
    sleep_milliseconds(300); // cross-platform !
    pp("c:$", *c);
    pp("d:$", *d);

    int32_t pos = d->find("hill");
    pp("'hill' found at pos:$", pos); // cross-platform !

    sleep_milliseconds(100);

    PString sName(pool, "Conrad"); // string object on stack - dynamic part automatically freed in destructor
    pp("name is:$", sName);

    pool.free_object(a);
    pool.free_object(b);
    pool.free_object(c);
    pool.free_object(d);
}

ObjectMemPoolWrapper<16, 100, 100> pool;

int main ()
{
    logWriter.flagWriteTime = true;
    test_basic_types(pool);


    return 0;
}