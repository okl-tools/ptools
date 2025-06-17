/*
 * PBits.h
 *
 *      Author: okl
 */

#pragma once


#include <string>


namespace ptools
{
    template<typename INTEGER>
    class PBits
    {
    public:

        PBits(INTEGER v = 0);
        PBits(const PBits &bitFlags);
        PBits(PBits &&bitFlags);
        PBits & operator=(PBits &&bitFlags);
        PBits & operator=(const PBits &bitFlags);
        PBits & operator=(INTEGER flags);

        bool set_flag(INTEGER bit, bool flagSet=true);
        void set_all_flags(INTEGER bit);
        void reset_flags();
        bool toggle_flag(INTEGER bit);

        void clear_bits(INTEGER bits);


        void set_bit_number(INTEGER bitNumber, bool flg = true);


        INTEGER flags() const;

        bool is_flag(INTEGER bit) const;
        INTEGER and_flag(INTEGER bit) const;

        bool is_zero_flags() const;


        operator bool() const
        { return _flags != 0; }

        bool operator==(INTEGER bit); // same like isFlag(bit); compare ONE flag
        bool operator!=(INTEGER bit); // same like !isFlag(bit); compare ONE flag


    public:
        bool operator&(INTEGER bit) = delete;
        bool operator^(INTEGER bit) = delete;
        bool operator|(INTEGER bit) = delete;

        bool operator&(INTEGER bit) const = delete;
        bool operator^(INTEGER bit) const = delete;
        bool operator|(INTEGER bit) const = delete;


    private:
        //alignas(sizeof(INTEGER)) std::atomic<INTEGER> _flags;
        alignas(sizeof(INTEGER)) INTEGER _flags;
    };

    using Bits32 = PBits<uint32_t>;


}

#include "inline/PBits_inline.h"