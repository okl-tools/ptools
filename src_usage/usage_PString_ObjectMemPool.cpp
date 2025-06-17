// About memory- and object management
// Dynamic objects within embedded world are more or less forbidden - i mean the "new" operator specially.
// It's all about non-deterministic behaviour - specially if you make use of STL. (std::string, std::map, ...)
//
// With a Memory Pool and some "clever" C++ 20 template function we can create objects by call of "create_object" and delete the by "free_object"
// To be "mem-management-algorithm" independent we have an interface "IObjectMemPool".
// If memory is need or dynamic string or map etc... we make use of the interface IObjectMemPool.
//
// Normal use case is to have a global object like this
//
// ObjectMemPoolWrapper<16, 100, 100> objMem;
//
//
// void f (IObjectMemPool & pool)
// {
//    PString s(pool, "hello");  // behaviour like std::string, ecapsulates some dynamic stuff inside....which is connected to the pool
//                               // ~PString .... releases handle of pool
//    ...
//    ...
//    ...
// }
//
// int main()
// {
//    f(objMem.get_pool())
//    // or shorter f(objMem) ...because of conversion operator
// }
//
//
//
// and wherever you need a IObjectMemPool object you put in "objMem". or "objMem.get_pool()"  - question of taste, both delegates an IObjectMemPool object.
// -----------------
//
// The code here shows some details about one concrete implementation of IObjectMemPool which is ObjectMemPool.
// ObjectMemPool needs blockSize,blockCount and objectCount and 2 buffers.
// An ObjectMemPool instance is wrapped into template ObjectMemPoolWrapper instance normally.
//
// For codes who want to see what's behind the mem/obj wrapper.
//
//

#include "PTools.h"
#include "ObjectMemPool.h"

using namespace ptools;


static void test_streaming (IObjectMemPool & pool)
{
    PString s(pool, "Red");
    s<< " ";
    s<< "sky";
    s<< " ";
    s<< "at";
    s<< " ";
    s<< "night";
    s<< ",";
    s<< " ";
    s<< "sailors";
    s<< " ";
    s<< "delight";

    pp("s: <<<$>>>", s);
}


// 1. Instead of
//ObjectMemPoolWrapper<32, 2500, 200> objMemPool;


// write this for more control - if need
const int32_t BLOCK_SIZE = 32;
const int32_t BLOCK_COUNT = 2500;
const int32_t OBJECTS_COUNT = 200;
char buf[BLOCK_SIZE * BLOCK_COUNT] {}; // user memory
char buf_ar[OBJECTS_COUNT * sizeof(HandleMemory)] {}; // managing objects


int main ()
{
    // 2. Instead of this
    // test_streaming(objMemPool); // conversion operator !

    // write this for more control - if need
    PArrayFixed<HandleMemory> arHandleMemory(buf_ar, OBJECTS_COUNT);
    ObjectMemPool objMemPool(BLOCK_SIZE, BLOCK_COUNT, buf, &arHandleMemory);
    test_streaming(objMemPool);


    return 0;
}