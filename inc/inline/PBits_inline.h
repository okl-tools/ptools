#pragma once

#include <bitset>

namespace ptools
{

#define CHECKER_BIT(var,pos) (((var)>>(pos)) & 1)



    template<typename INTEGER>
    inline PBits<INTEGER>::PBits(INTEGER v)
    {
        _flags = v;
    }


    template<typename INTEGER>
    inline bool PBits<INTEGER>::operator==(INTEGER bit)
    {
        return is_flag(bit);
    }

    template<typename INTEGER>
    inline bool PBits<INTEGER>::operator!=(INTEGER bit)
    {
        return !is_flag(bit);
    }


    template<typename INTEGER>
    inline bool PBits<INTEGER>::toggle_flag(INTEGER bit)
    {
        bool flagEnabled = is_flag(bit);

        return set_flag(bit, !flagEnabled);
    }

    template<typename INTEGER>
    inline void PBits<INTEGER>::clear_bits(INTEGER bits)
    {
        _flags &= ~bits; // Clear only file type bits

//        bool flagEnabled = is_flag(bit);
//        return set_flag(bit, !flagEnabled);
    }



    template<typename INTEGER>
    inline bool PBits<INTEGER>::set_flag(INTEGER bit, bool flg)
    {

        INTEGER flagsOld = _flags;

        if (flg)
        {
            _flags |= bit;
        }
        else
        {
            _flags &= (~bit);
        }

        return flg;
    }


    template<typename INTEGER>
    inline void PBits<INTEGER>::reset_flags()
    {
        _flags = 0;
    }


    template<typename INTEGER>
    inline bool PBits<INTEGER>::is_zero_flags() const
    {
        return _flags == 0;
    }

    template<typename INTEGER>
    inline void PBits<INTEGER>::set_all_flags(INTEGER bit)
    {
        _flags = bit;
    }

    template<typename INTEGER>
    inline INTEGER PBits<INTEGER>::flags() const
    {
        return _flags;
    }


    template<typename INTEGER>
    inline void PBits<INTEGER>::set_bit_number(INTEGER bitNumber, bool flg)
    {
        INTEGER bit = 1 << bitNumber;
        //_flags = flg ? _flags | bit : _flags & (~bit);

        if (flg)
        {
            _flags |= bit;
        }
        else
        {
            _flags &= (~bit);
        }


    }


    template<typename INTEGER>
    inline PBits<INTEGER>::PBits(const PBits & bitFlags)
    {
        _flags = bitFlags.flags();
    }

    template<typename INTEGER>
    inline PBits<INTEGER>::PBits(PBits && bitFlags)
    {
        _flags = bitFlags.flags();

        bitFlags._flags = 0;


    }

    template<typename INTEGER>
    inline PBits<INTEGER> & PBits<INTEGER>::operator=(PBits && bitFlags)
    {
        _flags = bitFlags.flags();

        bitFlags._flags = 0;

        return *this;
    }

    template<typename INTEGER>
    inline PBits<INTEGER> & PBits<INTEGER>::operator=(const PBits & bitFlags)
    {
        _flags = bitFlags.flags();

        return *this;
    }

    template<typename INTEGER>
    inline PBits<INTEGER> & PBits<INTEGER>::operator=(INTEGER f)
    {
        _flags = f;
//        pp("_flags:$, size:$", _flags, sizeof (_flags));
        return *this;
    }


    template<typename INTEGER>
    inline bool PBits<INTEGER>::is_flag(INTEGER bits) const
    {
        return (_flags & bits) != 0;
    }

    template<typename INTEGER>
    inline INTEGER PBits<INTEGER>::and_flag(INTEGER bits) const
    {
        return (_flags & bits);
    }


}
