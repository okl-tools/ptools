#pragma once

#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <concepts>
#include <cctype>

#include "PBasics.h"
#include "PFunction.h"
#include "PFormat.h"


namespace ptools
{

    enum ERR_CLASS
    {
        ERR_CLASS_GENERAL,
        ERR_CLASS_UART,
        ERR_CLASS_MEMORY,
        ERR_CLASS_SOCKET,
    };

    struct PResult
    {
        int err = 0;
        int errClass = 0;
        char msg[80];

        bool isOK () const
        {
            return err == 0;
        }

        bool isError () const
        {
            return err != 0;
        }

        operator bool () const
        {
            return isOK();
        }

        bool is_GENERAL() const
        {
            return errClass == ERR_CLASS_GENERAL;
        }
        bool is_SOCKET() const
        {
            return errClass == ERR_CLASS_SOCKET;
        }
        bool is_UART() const
        {
            return errClass == ERR_CLASS_UART;
        }

        bool is_MEMORY() const
        {
            return errClass == ERR_CLASS_MEMORY;
        }


        void reset ()
        {
            err = 0;
            errClass = 0;
        }
    };



}