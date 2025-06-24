// okl, 2025 June 12
// ----------------------
// Some placement new experiment
// Using same buffer to construct 2 Objects at the same place
// It seems that everything is fine - but not recommended to do this !
// Just learning about the mechanism.
// -----------


#include "PTools.h"
#include "PWriter.h"

#include <string>
#include <new>
#include <cstddef>



using namespace ptools;


constexpr size_t StackSize = 1024;
alignas(std::max_align_t) std::byte buf[StackSize];


template<typename T>
T* create_object_without_heap()
{
    constexpr size_t alignment = alignof(T);
    constexpr size_t size = sizeof(T);

    uintptr_t addr = reinterpret_cast<uintptr_t>(buf);
    uintptr_t alignedAddr = (addr + alignment - 1) & ~(alignment - 1);

    if (alignedAddr + size > reinterpret_cast<uintptr_t>(buf + StackSize))
    {
        std::cerr << "Out of stack memory\n";
        return nullptr;
    }

    T* obj = new (reinterpret_cast<void*>(alignedAddr)) T();
    return obj;
}


struct A
{
    A ()
    {
        pp("A::A()");
    }

    ~A ()
    {
        pp("A::~A()");
    }

    void do_something()
    {
        pp("A::do_something, x:$", x);
    }

    int x = 0;
};

int main ()
{
    A * a = create_object_without_heap<A>();  // everything ok !
    pp("Address of a:$", (const void *) a);
    a->x = 99; // everything ok !
    a->do_something(); // everything ok !

    A * b = create_object_without_heap<A>();  // we should never do this ! - it is an experiemnt only !

    pp("Address of b:$", (const void *) b);

    a->do_something();  // we should never do this !  - it is an experiment only !
    b->do_something(); // we should never do this !  - it is an experiment only !

    a->x = 88;

    a->do_something();  // we should never do this !  - it is an experiment only !
    b->do_something(); // we should never do this !  - it is an experiment only !

    std::destroy_at(a);
}

