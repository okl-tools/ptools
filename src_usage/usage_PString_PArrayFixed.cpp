#include "PTools.h"
#include "PArray.h"
#include "ObjectMemPool.h"


using namespace ptools;


struct A
{
    int x = 0, y = 0, z = 0;

    void show (int16_t space)
    {
        pp(space, "a:{$,$,$}", x, y, z);
    }
};


static void do_some_pstring_stuff (IObjectMemPool & pool)
{
//    PString *s = pool.create_object<PString>();
    PString * ps = PString::create(&pool);
    PString & s = *ps;
//    s->set_memory_pool(&pool);
    //s->reserve(1000);

    s = "Jodel";

    pp("s:'$'", s);

    s += "+Schrotthaufen";

    s.sprintF(",tra la la und so");
    pp("s:'$'", s);

    pp("info, s.size:$, s.capacity:$", s.size(), s.capacity());
    pool.show_info();


//    pool.show_info();
    s.cleanup();
    pool.show_info();

    pool.free_object(ps);

    pool.show_info();
}


static void write_abc (PString & s, int countChars)
{
    char ch = 'A';
    for (int i = 0; i < countChars; i++)
    {
        if (ch == 'J')
        {
            if (s.write_char('.').isError())
            {
                break;
            }
            ch = 'A';
            continue;
        }
        else
        {
            s.write_char(ch);
        }
        ch++;
    }
}

static void do_some_PArray_pstring_stuff (IObjectMemPool & pool)
{
    using namespace std;
//    vector<PString> v;

//    char mem [1000];
    PArray<PString> v(pool);
//    vector<PString> v;




    for (uint32_t i = 0; i < 100; i++)
    {
        if (pool.is_error_state())
        {
            if (pool.get_last_error() == ERR_COUNT_BLOCKS)
            {
                pool.clear_error();
            }
            break;
        }

        PString s(&pool);

//        write_abc(s, 500);
//        s.fill_up_rand_LETTERS(rand_next_int(100, 500));
//        s.fill_up_rand_LETTERS(rand_next_int(10, 50));
        s.fill_with_rand_LETTERS(rand_next_int(7, 17));

        pp();
        pool.show_blocks();
        pp();


        pp("$, size:$, capacity:$", s, s.size(), s.capacity());
//        v.push_back(std::move(s));
        v.emplace_back(std::move(s));
    }

    pool.show_info();

    for (const PString & s: v)
    {
        pp("$,len:$, capacity:$", s, s.size(), s.capacity());
    }

    pool.show_info();

    pool.show_blocks();


}

static void do_some_dynamic_pstring_stuff (IObjectMemPool & pool)
{
    using namespace std;
//    vector<PString*> arPString;
    PArray<PString *> arPString(pool);


    for (uint32_t i = 0; i < 200; i++)
    {
        if (pool.is_error_state())
        {
            if (pool.get_last_error() == ERR_COUNT_BLOCKS)
            {
                pool.clear_error();
            }
            break;
        }

        PString * ps = PString::create(&pool);
        if (ps == nullptr)
        {
            pool.clear_error();
            break;
        }
        PString & s = *ps;

        ps->fill_with_rand_LETTERS(rand_next_int(7, 17));

//        pp();
//        pool.show_blocks();
//        pp();


        pp("PString $, size:$, capacity:$", s, s.size(), s.capacity());
        arPString.push_back(ps);
    }


    pool.show_info();

//    pool.show_blocks();


}


static void do_some_mem_stuff_A_pointer (IObjectMemPool & pool)
{
    pp("do_some_mem_stuff_A_pointer");
    PArray<A *> ar(pool);
    for (int i = 0; i < 10; i++)
    {
        A * aTmp = pool.create_object<A>();
        if (aTmp)
        {
            A & a = *aTmp;
            a.x = 100 + i;
            a.y = 200 + i;
            a.z = 300 + i;

            ar.emplace_back(std::move(aTmp));
        }
        else
        {
            pp("SHIT - no mem");
        }
    }

    for (uint32_t i = 0; i < ar.size(); i++)
    {
        ar[i]->show(4);
    }

}

static void do_some_mem_stuff_A (IObjectMemPool & pool)
{
    pp("do_some_mem_stuff_A");
    PArray<A> ar(pool);
    for (int i = 0; i < 10; i++)
    {
        A a;
        a.x = 100 + i;
        a.y = 200 + i;
        a.z = 300 + i;

        ar.emplace_back(std::move(a));
    }

    for (uint32_t i = 0; i < ar.size(); i++)
    {
        ar[i].show(4);
    }

}

static void test_random_PString_PArrayFixed (IObjectMemPool & pool)
{
    pp("test_random_PString_PArrayFixed");

//    PString arBuf[1000];

    const uint32_t countStrings = 200;
    uint32_t szAr = sizeof(PString) * countStrings;

    void * pMemBackedArray = pool.alloc(szAr);
    if (!pMemBackedArray)
    {
        pp("pMem == nullptr");
        return;
    }

    PArrayFixed<PString> arStr(pMemBackedArray, szAr);


    for (uint32_t i = 0; i < countStrings; i++)
    {
        if (pool.is_error_state())
        {
            if (pool.get_last_error() == ERR_COUNT_BLOCKS)
            {
                pool.clear_error();
            }
            break;
        }

        PString s(&pool);

        s.fill_with_rand_LETTERS(rand_next_int(7, 17));

        arStr.emplace_back(std::move(s));
    }


    auto f = [] (uint32_t space, uint32_t index, const PString & s)
    {
        pp(space, "[$]=$, size:$, capacity:$", index, s.to_string(), s.size(), s.capacity());
    };

    arStr.show_objects(f);

    pool.show_info();


}


ObjectMemPoolWrapper<32, 5500, 1202> objMem;
//ObjectMemPoolWrapper<32, 550, 1202> objMem; // provoke mem error


int main ()
{
    pp("main pstring array");


    IObjectMemPool & pool = objMem.get_pool();

    pool.show_info();


//    do_some_pstring_stuff(pool);
//    do_some_mem_stuff_A1(pool);
//    do_some_mem_stuff_A(pool);
//    do_some_pstring_stuff_random(pool);
//    test_random_PString_PArrayFixed(pool);
    do_some_dynamic_pstring_stuff(pool);

    if (pool.get_last_error() != ptools::ERR_NO_ERROR)
    {
        pp("Error in do_some_string_stuff: $, as string '$'", pool.get_last_error(), pool.error_as_string());
    }
    else
    {
        pool.show_info();
    }
    pool.clear();
//    memPool.show_blocks();

    pool.show_info();


    return 0;
}