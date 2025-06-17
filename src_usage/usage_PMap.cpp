#include "PTools.h"
#include "ObjectMemPool.h"
#include "PMap.h"


using namespace ptools;

static void cc_map(PMap<int, int> m)
{
    pp(16, "main_nat_map::cc_map");

    pp(16, "size:$", m.size());

    for(auto kv : m)
    {
        pp(16, "$:$", kv.key, kv.val);
    }
}


static void test_map_1(IObjectMemPool & pool)
{
    pp("test_map_1");


    PMap<int, int> m(pool);

    m.insert(4,400);
    m.insert(2,200);
    m.insert(5,500);
    m.insert(1,100);
    m.insert(3,300);

    pp("size:$", m.size());

    for(auto kv : m)
    {
        pp("$:$", kv.key, kv.val);
    }

    m.replace(2, 222);

    pp("--");
    for(auto kv : m)
    {
        pp("$:$", kv.key, kv.val);
    }

    cc_map(m);


    pool.show_info(4);

}

static void cc_map_PString(PMap<PString, PString> m)
{
    m.show(16);
    m.get_pool().show_info(16);

    auto mm = m;

    m.show(24);
    m.get_pool().show_info(24);

}


static void test_map_2(IObjectMemPool & pool)
{
    pp("test_map_2");


    PMap<PString, PString> m(pool);

    pp("sizeof NaturalOrderedMap<PString, PString>:$", sizeof (PMap<PString, int>));


    for (int i=0; i<5; i++)
    {
        PString sKey(pool);
        sKey.fill_with_rand_LETTERS(20);
        PString sVal(pool);
        sVal.fill_with_rand_LETTERS(20);
        m.insert(std::move(sKey), std::move(sVal));
    }

    m.show();


    pool.show_info(4);
    cc_map_PString(m);

    pool.show_info(4);

}



static void test_map_3(IObjectMemPool & pool)
{
    pp("test_map_3");

    PMap<PString, PString> m(pool);


    PString sKey(pool, "AA");
    PString sVal(pool, "Allershausen");
    m.insert(std::move(sKey), std::move(sVal));


    sKey = "B";
    sVal = "Berlin";
    m.insert(std::move(sKey), std::move(sVal));

    sKey = "CE";
    sVal = "Celle";
    m.insert(std::move(sKey), std::move(sVal));

    sKey = "CU";
    sVal = "Cuxhafen";
    m.insert(std::move(sKey), std::move(sVal));


    m.show();

    pp("-");

    sKey = "AA";
    PString sVal3(pool, "Aachen");

    m.replace(sKey, sVal3);


    m.show();

}

ObjectMemPoolWrapper<32, 2500, 200> pool;


int main ()
{
    pp("main natural order map *");


    pool.show_info();

//    test_map_1(pool);
//    test_map_2(pool);
    test_map_3(pool);

//    pp("sizeof PString:$", sizeof (PString));

    pool.show_info();

    return 0;
}