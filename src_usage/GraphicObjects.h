// GraphicObjects.h - used in polymorphism.cpp
// to demonstrate how to handle a little class hierarchy in context of ptools
//
//
// GraphicObject
// |-Line
// |-Circle
// |-Rectangle
//
// It's not about to teach how C++ polymorphism works.
// I assume this example is understood.
// -----------------------------------------
// Small example about C++ polymorphism in context of ptools
//
// Animal
// |-Dog
// |-Donkey
//
// struct Animal
// {
//     virtual void make_noise() = 0;
// };
//
// struct Dog : Animal
// {
//     void make_noise() override { cout << "BARK"; }
// };
//
// struct Donkey : Animal
// {
//     void make_noise() override  { cout << "hee-haw";  }
// };
//
//
//  // PC context only
//  // -------------------------------
//  Animal * a = new Dog;     // PC ok ...but within embedded environment this is mostly "vorbidden", because new ... is not deterministic. Specially in RTOS
//  a->make_noise(); // BARK
//
//
// //  What you can do is make use of memory pool and make use of C++ "placement new" mechanism.
//
//
//
//
//  // FreeRTOS,threadX context with ptools
//  // -----------------------------------------------
//  // instantiate globally
//  using memBlocks = StaticMemorySetup<16, 500, 100>; // this is one simulated heap - you can have more. 16 bytes each block, 500 blocks, 100 objects ( any object)
//
//  // within a function
//  void my_farm()
//  {
//      IMemoryPool & pool = memBlocks::get_pool();
//      Animal * a = pool.create_object<Dog>();
//      a->make_noise(); // BARK
//      ...
//      pool.free_object(a);
//  }
////
// ---------------------------------


#pragma once

#include <cstdint>
#include "PWriter.h"

using namespace ptools; // for convience only


struct Color
{
    uint8_t r{0};     // red component (0–255)
    uint8_t g{0};     // green component (0–255)
    uint8_t b{0};     // blue component (0–255)
    uint8_t a{255};   // alpha component (0–255), 255 = fully opaque

    static Color black ()
    {
        return {0, 0, 0, 255};
    }

    static Color white ()
    {
        return {255, 255, 255, 255};
    }

    static Color red ()
    {
        return {255, 0, 0, 255};
    }

    static Color green ()
    {
        return {0, 255, 0, 255};
    }

    static Color blue ()
    {
        return {0, 0, 255, 255};
    }

    static Color transparent ()
    {
        return {0, 0, 0, 0};
    }


    void to_writer (PWriter & writer) const // compiler detects this as "concept" ! => PWriter.h
    {
        writer.sprintF("rgba{$,$,$,$}", r, g, b, a);
    }
};

struct Point
{
    float x{0.0f};
    float y{0.0f};

    Point (float x, float y) : x(x), y(y)
    {
    }

    Point ()
    {
    }

    Point (const Point & pt)
    {
        x = pt.x;
        y = pt.y;
    }


    static Point origin ()
    {
        return {0.0f, 0.0f};
    }

    void to_writer (PWriter & writer) const // compiler detects this as "concept" ! => PWriter.h
    {
        writer.sprintF("xy{$,$}", x, y);
    }

};

struct Size
{
    float w {0.0f};   // width
    float h {0.0f};   // height

    Size () = default;

    Size (float width, float height)
            : w(width), h(height)
    {
    }

    Size (const Size & sz)
    {
        w = sz.w;
        h = sz.h;
    }

    bool is_empty () const
    {
        return (w <= 0.0f || h <= 0.0f);
    }

    static Size zero ()
    {
        return {0.0f, 0.0f};
    }

    void to_writer (PWriter & writer) const
    {
        writer.sprintF("size{$,$}", w, h);
    }
};

// we do not have RTTI - so we have an enumeration to resolve our types easily by switch/case
enum GRAPHIC_OBJECT
{
    GO_NO_TYPE = 0,
    GO_CIRCLE=1,
    GO_LINE=2,
    GO_RECTANGLE=3
};



// Base type for all drawable objects
struct GraphicObject
{
    uint8_t type=GO_NO_TYPE;
    Point position;    // logical center or anchor point
    Color color;       // color of the object
    bool visible{true}; // visibility flag

    GraphicObject(GRAPHIC_OBJECT t, const Point& pos, const Color& col)
            : type(t), position(pos), color(col)
    {
    }

    virtual void draw (uint32_t space=0) = 0; // pure virtual for polymorphic drawing

    GraphicObject () = default;
    virtual ~GraphicObject () = default;

    virtual void to_writer (PWriter & writer) const  = 0;


};

struct Circle : GraphicObject
{
    Circle ()
    {
        type = GRAPHIC_OBJECT::GO_CIRCLE;
    }

    Circle (const Point & position, float radius, const Color & col=Color::black()) : GraphicObject(GRAPHIC_OBJECT::GO_CIRCLE, position, col), radius(radius)
    {
        type = GRAPHIC_OBJECT::GO_CIRCLE;
    }

    float radius{0.0f};

    void to_writer (PWriter & writer) const  // compiler detects this as "concept" ! => PWriter.h
    {
        writer.sprintF("pos:$, radius:$, color:$", position, radius, color);
    }

    void draw (uint32_t space=0) override
    {
        pp(space, "Circle::draw");
        pp(space+4, "pos:$", position);
        pp(space+4, "radius:$", radius);
        pp(space+4, "color:$", color);
    }
};

struct Rectangle : GraphicObject
{
    Size size;

    Rectangle ()
    {
        type = GRAPHIC_OBJECT::GO_RECTANGLE;
    }
    Rectangle (const Point & position, const Size & size, const Color & col=Color::black()): GraphicObject(GRAPHIC_OBJECT::GO_RECTANGLE, position, col), size(size)
    {
        type = GRAPHIC_OBJECT::GO_RECTANGLE;
    }

    void to_writer (PWriter & writer) const  // compiler detects this as "concept" ! => PWriter.h, pp(...) example, so you can write  Rectangle r;  pp("r:$", r);
    {
        writer.sprintF("pos:$, size:$, color:$", position, size, color);
    }

    void draw (uint32_t space=0) override
    {
        pp(space, "Rectangle::draw - simulation");
        pp(space+4, "pos:$", position);
        pp(space+4, "size:$", size);
        pp(space+4, "color:$", color);

    }
};

struct Line : GraphicObject
{
    Point startPoint;
    Point endPoint;
    float thickness{1.0f};

    Line ()
    {
        type = GRAPHIC_OBJECT::GO_LINE;
    }

    Line (const Point & startPoint, const Point & endPoint, float thickness, const Color & col=Color::black()) : GraphicObject(GRAPHIC_OBJECT::GO_LINE, {0,0}, col),
            startPoint(startPoint), endPoint(endPoint), thickness(thickness)
    {
        type = GRAPHIC_OBJECT::GO_LINE;
    }

    void to_writer (PWriter & writer) const // compiler detects this as "concept" ! => PWriter.h, pp(...) example, so you can write  Line line;  pp("line here at:$", line);
    {
        writer.sprintF("startPoint:$, endPoint:$, thickness:$, color:$", startPoint, endPoint, thickness, color);
    }

    void draw (uint32_t space=0) override
    {
        pp(space, "Line::draw - simulation");
        pp(space+4, "startPoint:$", startPoint);
        pp(space+4, "endPoint:$", endPoint);
        pp(space+4, "thickness:$", thickness);
        pp(space+4, "color:$", color);

    }
};
