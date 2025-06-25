# ptools - power tools - v0.43
### C++ 20 power tools for embedded and linux

Branch **dev** - devlopment.
Wrapping more and more old C stuff...

Bringing modern C++20 to bare metal or RTOS-based embedded systems is challenging.
ptools' aim is make a bridge between raw C power and modern C++.
STL is not designed to work on small devices. But there are many good things like
std::string, std::map, std::vector, ... we don't really want to miss.
Specially if you come from C++ desktop/frontend/backend developing to embedded world.

**ptools** is a like STL-light ... or better like STL-light-light

- PString //  std::string and a little like QString 
- P* templates
  - PMap // std::map
  - PArray // std::vector .... // I prefer array instead of vector because of 2D/3D vectors i often use...
  - PStack
  - PTokenizer
  
- ptools::json::Node // a full DOM like tree. Parsing and writing json strings

**source** is under MIT License</br>
The total size is around 160 KB, with 100 KB being source code and the rest made up of examples.

Develope some stuff under linux. Let it run under linux.
Change toolchain and compile it for arm - STM's NUCLEO-H723ZG for example.

**Requirements** , modern C++ 20 toolchain linux (and toolchain arm), cmake <br/>
(Windows toolchains were actually not tested)

Main reason why STL *is forbidden* on embedded platforms is heavily usage of heap.
Heap access is often non-deterministic. And it's encapsulated totally within STL.  
For C++ developers on PCs it's fine. They can concentrate on developing 
algorithms and applications. 

C developer on little devices must have an extra eye on resources.
**new** or even *normal malloc* is a no go. So there are memory pools in any variation.  
---
Focus on ptools now

**IObjectMemPool** is the interface to manage memory and objects.
It'a obvious that we can allocate and free mem. So far so good.<br/> 
C++ mechanism "placement new" allows us to have something like **new**/**delete** in deterministic manner. 
Some lines of code show us how to use. 

<pre> 
ObjectMemPoolWrapper<16, 100, 100> globObjMem; // we have a global pool with blocksize 16, 100 blocks and 100 objects


// very simple - shows principal usage 
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
    pp("'hill' found at pos:$", pos); 

    sleep_milliseconds(100);

    PString sName(pool, "Conrad"); // string object on stack - dynamic part automatically freed by destructor
    pp("name is:$", sName);

    pool.free_object(a);
    pool.free_object(b);
    pool.free_object(c);
    pool.free_object(d);
}

int main ()
{
    test_basic_types(globObjMem.get_pool()); // get_pool() returns a reference to an IObjectMemPool object.
    //test_basic_types(globObjMem); // or shorter, ObjectMemPoolWrapper has a conversion operator
    return 0;
}

</pre>
creates this output (columns: number, time, [threadID] )

<pre> 
[000001] [00:00:00,000] a:100
[000002] [00:00:00,000] b:200
[000003] [00:00:00,300] c:3.14
[000004] [00:00:00,300] d:10 goats on the hill enjoying grass
[000005] [00:00:00,300] 'hill' found at pos:16
[000006] [00:00:00,400] name is:Conrad
</pre>


## void pp (...) - power print

The ptools started with power print. I wrote my favorite logger and continued ....

pp works with variadic template mechanism.
pp is type safe and very easy to handle. 
output goes to console or to embedded UART interface. 
<pre>

pp(); // empty line
pp("hello");
pp("There are $ goats on the $", 10, "hill");

struct Color
{
    uint8_t r{0};     // red component (0–255)
    uint8_t g{0};     // green component (0–255)
    uint8_t b{0};     // blue component (0–255)
    uint8_t a{255};   // alpha component (0–255), 255 = fully opaque
    void to_writer (PWriter & writer) const // compiler detects this as "concept" ! => PWriter.h
    {
        writer.sprintF("rgba{$,$,$,$}", r, g, b, a);
    }
};

Color colRed{255,0,0,255};

pp("color is:$", colRed); 

</pre>









