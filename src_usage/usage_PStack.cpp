#include "PTools.h"
#include "ObjectMemPool.h"
#include "PStack.h"


using namespace ptools;


static void test1(IObjectMemPool & pool)
{
    PStack<int> st(pool);

    for (int i=0; i<5; i++)
    {
        st.push(i);
    }

    pp(4, "stack with $ elements", st.size());
    while (!st.is_empty())
    {
        int val = st.top();
        pp("$", val);
        st.pop();
    }

    pool.show_info(4);

}

ObjectMemPoolWrapper<16, 500, 500> pool;


int main ()
{
//    logWriter.flagWriteTime = true; // make no sense - too fast - always 0
    pool.show_info();
    test1(pool);
    pool.show_info();


    return 0;
}