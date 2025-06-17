#include "PTools.h"
#include "PMapFixed.h"

#include "PWriter.h"
#include "ObjectMemPool.h"
#include "PArray.h"
#include "PTools.h"



using namespace ptools;


static void test_int_array(IObjectMemPool & pool)
{

    PArray<int> ar(pool);


    pool.show_info(4);

    pp("-> 38 new Elements");
    for (int i = 0; i < 38; ++i)
    {
        ar.push_back(i + 1);
    }

//    factory.show(4);
    ar.show(8);

    pp("-----");

    pp("-> delete 3 Elements");
    for (int i = 0; i < 3; i++)
    {
        ar.remove_at(1);
    }

    pool.show_info(4);
    ar.show(8);

    pp("-> 12 new Elements");
    for (int i = 0; i < 12; ++i)
    {
        ar.push_back(100 + i + 1);
    }

    ar.show(8);
    pool.show_info(4);

    pp("---------------------------");
    ar.insert_at(11, 999);
    pool.show_info(4);
    ar.show(8, 13);

    pp("---------------------------");

    ar.clear();

    pool.show_info(4);


}

// Demonstration purpose only - watch memory pool!
static void full_copy_show_array(PArray<PString>  ar)
{
    pp(16, "SUB SUB");
    ar.show(16);
    ar.get_pool().show_info(16);

}
static void test_string_array(IObjectMemPool & pool)
{
    PArray<PString> ar(pool);
    ar.emplace_back(std::move(PString(pool, "Albert")));
    ar.emplace_back(std::move(PString(pool, "Berta")));
    ar.emplace_back(std::move(PString(pool, "Cäsar")));
    pool.show_info(4);

    ar.show(4);
    full_copy_show_array(ar);

    pool.show_info(4);

}

ObjectMemPoolWrapper<8, 800, 200> pool;

int main ()
{
    rand_seed_by_milliseconds();

    pool.show_info();

    test_int_array(pool);
//    test_string_array(pool);

    pool.show_info();

    return 0;
}


