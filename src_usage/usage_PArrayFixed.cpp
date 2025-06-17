#include "PTools.h"
#include "PArrayFixed.h"


using namespace ptools;

static void test_array_delete_at ()
{
    const int SIZE = 10;
    int buf[SIZE];
    PArrayFixed<int> ar(buf, SIZE);

    rand_seed_by_milliseconds();


    for (int i = 0; i < SIZE; i++)
    {
        ar.push_back(rand_next_int(0, 1000));
    }

    ar.sort();
    ar.show(0);
    pp();
    pp("delete [3]");
    pp();
    ar.delete_at(3);
    ar.show(0);

}

static void test_array_insert_at ()
{
    const int SIZE = 15;
    int buf[SIZE];
    PArrayFixed<int> ar(buf, SIZE);

    rand_seed_by_milliseconds();


    for (int i = 0; i < 10; i++)
    {
        ar.push_back(rand_next_int(0, 1000));
    }

    ar.sort();
    ar.show(0);
    pp();

    for (int i = 100; i < 150; i++)
    {
        ar.insert_at(3, i);
    }

    ar[3] = 10000;

    ar.show(0, 15);

}

static void test_find ()
{
    pp("main_array::find_first");

    struct Point
    {
        int x = 0, y = 0, z = 0;
        void to_writer(PWriter & writer) const
        {
            writer.sprintF("($,$,$)", x, y, z);
        }
        bool equals(const Point & pt) const
        {
            return x == pt.x && y == pt.y && z == pt.z;
        }
    };

    const int SIZE = 10;
    int buf[SIZE];
    PArrayFixed<Point> ar(buf, SIZE);

    for (int i = 0; i < SIZE; i++)
    {
        ar.push_back({i * 100, i * 200, i * 300});
    }

    ar.show();

    Point ptSearch{800, 1600, 2400};
//    Point ptSearch{800, 1601, 2400};

//    ar.sort([](const Point & pt1, const Point & pt2)->bool
//            {
//                return pt1.z == pt2.z;
//    });
    int index = ar.find_specific([] (const Point & pt1, const Point & pt2) -> bool
                                 {
                                     return pt1.equals(pt2);
                                 }, ptSearch);

    pp("find at: $", index);

}


int main ()
{
    pp("main misc");
    test_array_delete_at();
//    test_find();
//    test_array_insert_at();


    return 0;
}


