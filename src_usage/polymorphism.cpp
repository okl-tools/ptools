#include "PTools.h"
#include "ObjectMemPool.h"
#include "PArray.h"

#include "GraphicObjects.h"


using namespace ptools;




static void test_color()
{
    Color col1{122,122,34,255};
    Color col2{32,12,44,200};
    Color colRed{255,0,0,255};

    pp("colors - col1:$, col2:$", col1, col2);
}

static void test_rect()
{
    Rectangle r;
    r.position = {100,200};
    r.size = {50.32f, 49.87f};
    r.color = Color::red();

    pp("r:$", r);

    r.draw();
}

static void test_line()
{
    Line line;
    line.startPoint = {100, 200};
    line.endPoint = {230, 450};
    line.color = Color::black();
    line.thickness = 1.2f;

    pp("line:$", line);
    line.draw();
}

static void test_circle()
{
    Circle circle({200,300}, 45.6f, Color::red());

    pp("circle:$", circle);

    circle.draw();
}


static void test_geo_objects()
{
    test_color();
    pp();
    test_line();
    pp();
    test_circle();
    pp();
    test_rect();
    pp();
}


static void test_dynamic_color(IObjectMemPool & pool)
{
    Color *col1 = pool.create_object<Color> (122,122,34,255);
    Color *col2 = pool.create_object<Color> (32,12,44,200);

    pp(4, "colors - col1:$, col2:$", *col1, *col2);
    pool.show_info(4);

    pool.free_object(col1);
    pool.free_object(col2);
}



static void test_some_rect_objects(IObjectMemPool & pool)
{
    PArray<GraphicObject*> ar(pool);
    pool.show_info(0, SHOW::FULL);

    for (int i=0; i< 5; i++)
    {
        Rectangle * r = pool.create_object<Rectangle>(); // feels like heap - but NOT !
        r->position = Point {100.0f+i, 200.0f + i};
        r->size = {50.32f, 49.87f};
        r->color = Color::red();

        ar.push_back(r);
    }

    pool.show_info(0, SHOW::FULL);

    for (GraphicObject * obj: ar)
    {
        pool.free_object(obj);
    }
    //ar.free_pointer_objects(); // same as loop above !

    pool.show_info(0, SHOW::FULL);


}

static void test_dynamic_random_rects(IObjectMemPool & pool)
{
    PArray<Rectangle*> arObjects(pool);
    pool.show_info(0);

    for (int i=0; i< 5; i++)
    {
        Rectangle * r = pool.create_object<Rectangle>(); // feels like heap - but NOT !

        r->position = Point {100.0f+i, 200.0f + i};
        r->size = {50.32f, 49.87f};
        r->color = Color::red();

        arObjects.push_back(r);
    }


    PString s(pool);
    auto fPrint = [&s](uint32_t space, uint32_t  index, const Rectangle * rect)
    {
        s.cleanup();
        rect->to_writer(s);
        pp(space, "[$]=Rectangle {$}", index, s);
    };

    arObjects.show_objects(fPrint, 4);


//    pool.show_info(0, SHOW::FULL);
    pool.show_info(0);

    arObjects.free_pointer_objects();

//    pool.show_info(0, SHOW::FULL);
    pool.show_info(0, SHOW::BASIC);


}

static GraphicObject * create_random_object(IObjectMemPool & pool)
{
    uint8_t type = rand_next_int(GRAPHIC_OBJECT::GO_CIRCLE, GRAPHIC_OBJECT::GO_RECTANGLE);

    switch (type)
    {
        case GRAPHIC_OBJECT::GO_CIRCLE:
            return pool.create_object<Circle>();
        case GRAPHIC_OBJECT::GO_LINE:
            return pool.create_object<Line>();
        case GRAPHIC_OBJECT::GO_RECTANGLE:
            return pool.create_object<Rectangle>();
    }

    return nullptr;

}

static void test_dynamic_random_geo_objects(IObjectMemPool & pool)
{
    PArray<GraphicObject*> arObjects(pool);
    pool.show_info(4);

    for (int i=0; i< 5; i++)
    {
        GraphicObject * gObject = create_random_object(pool);
        if (!gObject)
        {
            pp("gObject == nullptr, should not be here !");
            continue;
        }

        gObject->position = Point {100.0f + i, 200.0f + i};
        gObject->color = Color::red();

        arObjects.push_back(gObject);
    }

    for (GraphicObject * graphicObject: arObjects)
    {
        graphicObject->draw(8); // simulate drawing !
    }


    // use this
    arObjects.free_pointer_objects();

    // instead of this - but like you want
//    for (GraphicObject * gObject: arObjects)
//    {
//        pool.free_object(gObject);
//    }



//    pool.show_info(0, SHOW::FULL);
    pool.show_info(4, SHOW::BASIC);


}



ObjectMemPoolWrapper<16, 500, 500> objMem;



int main ()
{

    rand_seed_by_milliseconds();


    objMem.show_info();

//    test_color();
    //test_rect();
//    test_line();
//    test_circle();

//    test_geo_objects();
//    test_dynamic_random_rects(pool);
    test_dynamic_random_geo_objects(objMem);
//    pool.show_info(0, SHOW::FULL);
    objMem.show_info(0, SHOW::BASIC);



    return 0;
}

