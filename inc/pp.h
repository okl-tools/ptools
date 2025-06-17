#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif
    void ppc(const char *pStr); // like printf("%s\r\n", pStr);
    void pp_first_line(const char *pStr); // until first (CR)LF or end
    void set_hal_time(int flag); // if flag == 1, HAL_GetTick, otherwise tx_time_get
    void ppInt(int val); // like printf("%d\r\n", val);
    void ppIP(uint32_t val); // internal val conversion and like printf("IP:%d.%d.%d.%d\r\n", i3, i2, i1, i);
    void ppUIntMessage(const char * pMsg, uint32_t val); // special, if pMsg contains "$" it will be replaced with val, it goes internal to pp(...)
    void ppIntMessage(const char * pMsg, int32_t val); // special, if pMsg contains "$" it will be replaced with val, it goes internal to pp(...)

    void ppVal(uint32_t val, const char *pMessage); // like printf("val:%d - '%s'\r\n", val, pMessage);
    void ppStatus(uint32_t status, const char *pMessage); // like printf("status:%d - '%s'\r\n", status, pMessage);
    void pp_MAC(uint8_t* mac);
    void pp_uint_as_MAC(uint32_t);
    void pp_int(int32_t val);
    void pp_int_hex(int32_t val);
    void pp_uint(uint32_t val);
    void pp_uint_hex(uint32_t val);
    void pp_string(const char * pStr);

    void pp_print_buf (const char * pBuf, uint32_t buflen);


    // ----------
    uint32_t pp_time_ms (); // intern
    void sleep_milliseconds(uint32_t ms);
    uint32_t get_milliseconds();
    uint32_t pp_time_ms(); // @DEPRECATED - use get_milliseconds !
    void write_string_buf(const char *pStr); // DEPRECATED, like printf("%s\r\n", pStr);
    uint32_t get_thread_ID();

#ifdef __cplusplus
}


#endif
