#include "PTools.h"
#include "ObjectMemPool.h"
#include "PRequest.h"
#include "PHttpHeader.h"


using namespace ptools;




// <Method> <Request-Target> <HTTP-Version>\r\n
//  POST /api/led HTTP/1.1
//  GET / HTTP/1.1
//  GET /STM32H7xx.html HTTP/1.1
//  GET /STM32H7xx_files/stm32.jpg HTTP/1.1


static const char * get_example_http_header_v_1_1_GET()
{
    return
            "GET /index.html HTTP/1.1\r\n"
            "Host: 192.168.1.100\r\n"
            "User-Agent: MyBrowser/1.0\r\n"
            "Accept: text/html,application/xhtml+xml\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Connection: keep-alive\r\n"
            "Content-Length: 2\r\n"
            "\r\n"
            "OK";
}

static void test_PRequest()
{

    const char* pRequest = "GET /STM32H7xx.html HTTP/1.1";

    pp("main_pstring::test_PRequest <<<$>>>", pRequest);

    PRequest request = PRequest::create_request(pRequest, string_len(pRequest));

    request.show();

}

static void show_pstring_conversion_operator(const char * p)
{
    pp("show_pstring_conversion_operator, p <<<$>>>", p);
}

static void test_http_header(IObjectMemPool & pool)
{

    const char* pRequest = get_example_http_header_v_1_1_GET();

    PHttpHeader header = PHttpHeader::create_header(pool, pRequest, string_len(pRequest));


//    header.hmap.show();
    header.show();

    PStringView ext = header.request.requestTarget.get_file_extension();
    pp("req <<<$>>>", header.request.requestTarget);
    pp("ext <<<$>>>", ext);

    if (ext.is_file_extension("htm", "html"))
    {
        pp("yes, html page");
    }
    else
    {
        pp("no html ?");
    }

    PString ps = header.request.requestTarget.to_PString(pool);

    pp("ps, req <<<$>>>", ps);

    show_pstring_conversion_operator(ps);


    pool.show_info();
}


ObjectMemPoolWrapper<32, 2500, 200> pool;

// GET /urk.html HTTP/1.1


int main (int argc, char * argv[])
{
    test_http_header(pool);

//    PString s(pool);
//    PString * s2 = PString::create(&pool);

    return 0;
}