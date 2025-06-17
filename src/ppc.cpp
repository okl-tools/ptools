#include "pp.h"
#include "PWriter.h"

//extern ptools::PWriter_HAL_UART uartWriter;

using namespace ptools;

extern "C" {
uint32_t get_microseconds ();


void ppc (const char * pStr)
{
    //HAL_UART_Transmit(&huart3, (uint8_t*)pStr, strlen(pStr), HAL_MAX_DELAY); // klappt !!!

    ptools::pp(pStr);
}
void pp_first_line (const char * pStr)
{
    //HAL_UART_Transmit(&huart3, (uint8_t*)pStr, strlen(pStr), HAL_MAX_DELAY); // klappt !!!

    logWriter.write_trailer();

    if (pStr == nullptr)
    {
        logWriter.write_cstring("nullptr");
    }
    else
    {
        while (*pStr)
        {
            if (*pStr == '\r' || *pStr == '\n')
            {
                break;
            }
            logWriter.write_char(*pStr);
            pStr++;
        }
    }
    //uartWriter.write_cstring(pStr);
    logWriter.write_LF();

}

void ppInt (int val)
{
    ptools::pp("val:$", val);
}

void ppStatus (uint32_t status, const char * pMessage)
{
    ptools::pp("status:$ - '$'", status, pMessage);
}
void ppVal (uint32_t val, const char * pMessage)
{
    ptools::pp("val:$ - '$'", val, pMessage);
}

void pp_int (int32_t val)
{
    ptools::logWriter.sprintF("$", val);
//    ptools::mmStream.sprintF("$", val);
}
void pp_uint (uint32_t val)
{
    ptools::logWriter.sprintF("$", val);
//    ptools::mmStream.sprintF("$", val);
}

void pp_int_hex (int32_t val)
{
//    ptools::mmStream.write_int_hex(val);
}
void pp_uint_hex (uint32_t val)
{
//    ptools::mmStream.write_uint_hex(val);
}

void pp_uint_as_MAC (uint32_t val)
{
    pp_MAC((uint8_t *) &val);
}


void pp_MAC (uint8_t * mac)
{
    if (mac == nullptr)
    {
        ptools::pp("mac == nullptr");
        return;
    }

    uint32_t mac0 = mac[0];
    uint32_t mac1 = mac[1];
    uint32_t mac2 = mac[2];
    uint32_t mac3 = mac[3];
    uint32_t mac4 = mac[4];
    uint32_t mac5 = mac[5];

    pp_uint_hex(mac0);
    pp_string(":");
    pp_uint_hex(mac1);
    pp_string(":");
    pp_uint_hex(mac2);
    pp_string(":");
    pp_uint_hex(mac3);
    pp_string(":");
    pp_uint_hex(mac4);
    pp_string(":");
    pp_uint_hex(mac5);

    //mmStream.sprintF("$:$:$:$:$:$", mac0, mac1, mac2, mac3, mac4, mac5);

}



//Beispiel mit IP 192.168.1.10 (hex: 0xC0A8010A):
//uint32_t ip = 0xC0A8010A;
//ppIP(ip);

void ppIP (uint32_t ip)
{
    uint8_t * p = (uint8_t *) &ip;
    uint32_t i3 = p[3];
    uint32_t i2 = p[2];
    uint32_t i1 = p[1];
    uint32_t i0 = p[0];

//    ptools::pp("IP:$.$.$.$", i3, i2, i1, i0);
    ptools::pp("IP:$.$.$.$", i0, i1, i2, i3);
}


void write_debug (const char * pStr)
{
    ptools::pp(pStr);
}


void ppUIntMessage (const char * pMsg, uint32_t val)
{
    ptools::pp(pMsg, val);
}

void ppIntMessage (const char * pMsg, int32_t val)
{
    ptools::pp(pMsg, val);
}

void pp_string (const char * pStr)
{
    if (pStr == nullptr)
    {
        ptools::logWriter.write_cstring("nullptr");
    }
    else
    {
        ptools::logWriter.write_cstring(pStr);
    }
}

void pp_print_buf (const char * pBuf, uint32_t buflen)
{
    if (pBuf)
    {
        ptools::logWriter.write_mem(pBuf, buflen);
    }
    else
    {
        ptools::logWriter.write_mem("null", 4);
    }


}

uint32_t pp_time_ms ()
{
    return get_milliseconds();
}


}
