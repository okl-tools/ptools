#include "PTools.h"
#include "PFunction.h"


using namespace ptools;


void say_hello(int id)
{
    pp("say_hello: $", id);
}

void test_func()
{
    PFunction<void(int)> f;
    f.bind(say_hello);
    f(1);
}


void test_lambda()
{
    PFunction<void(int)> f;

    f.bind([](int x)
           {
               pp("lambda no capture: $", x);
           });

    f(2);
}

static void test_lambda_capture_bind()
{
    int base = 10;

    PFunction<void(int)> f;

    f.bind([base](int x)
           {
               pp("lambda + capture: $", base + x);
           });

    f(3);


}


static void test_lambda_capture()
{
    int base = 10;

    PFunction<int(int,int)> f  = [base](int a, int b) -> int
    {
        pp("calc, base:$, $+$", base, a, b);
        return base+a+b;
    };

    int result = f(3,4);

    pp("result:$", result);


}


static int do_something_with_functor(PFunction<int(int,int)> & f)
//static int do_something_with_functor(auto f) // mmmmh, bad style ?
{
    int a = 900;
    int b = 600;

    pp(4, "do_something_with_functor...call with args, a:$, b:$", a, b);

    return f(a,b);

}

static void test_add()
{
    pp("test_add");
    int base = 10;

    PFunction<int(int,int)> f  = [base](int a, int b) -> int
    {
        pp("functor 'calc', capture:base:$, $+$", base, a, b);
        return base+a+b;
    };

    int result = do_something_with_functor(f);

    pp("result:$", result);


}

static void test_mult()
{
    int base = 10;

    PFunction<int(int,int)> f  = [base](int a, int b) -> int
    {
        pp("calc, base:$, $+$", base, a, b);
        return base+a*b;
    };

    int result = do_something_with_functor(f);

    pp("result:$", result);


}


int main ()
{
//    test_func();
//    test_lambda();
//    test_lambda_capture_bind();
//    test_lambda_capture();

    test_add();
//    pp();
//    test_mult();
    return 0;
}


