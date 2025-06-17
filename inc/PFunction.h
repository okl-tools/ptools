// PFunction.h
//
// okl, 2025 June 12
//
// Intention is to replace std::function within embedded environment
//
// -----------------------

#pragma once

#include <stdint.h>

namespace ptools
{

    template<typename T>
    struct remove_reference
    {
        using type = T;
    };
    template<typename T>
    struct remove_reference<T &>
    {
        using type = T;
    };
    template<typename T>
    struct remove_reference<T &&>
    {
        using type = T;
    };

    template<bool B, typename T = void>
    struct enable_if
    {
    };

    template<typename T>
    struct enable_if<true, T>
    {
        using type = T;
    };


    template<typename T>
    struct is_void
    {
        static constexpr bool value = false;
    };
    template<>
    struct is_void<void>
    {
        static constexpr bool value = true;
    };


    template<typename Signature, uint32_t StorageSize = 32>
    struct PFunction;

    template<typename Ret, typename... Args, uint32_t StorageSize>
    struct PFunction<Ret (Args...), StorageSize>
    {
        using func_t = Ret (*) (void *, Args...);
        using copy_t = void (*) (void * dst, const void * src);
        using destroy_t = void (*) (void *);

        alignas(void *) uint8_t storage[StorageSize] = {};
        void * context = nullptr;

        func_t func = nullptr;
        copy_t copier = nullptr;
        destroy_t destroyer = nullptr;

        PFunction () = default;
        ~PFunction ();

        PFunction (const PFunction & other);
        PFunction & operator= (const PFunction & other);



        template<typename F>
        PFunction & operator= (F f);

        template<typename F>
        PFunction (F f);


        template<typename F>
        void bind (F f);


        template<typename T>
        void bind_member (Ret (T::*method) (Args...), T * instance);


        // operator() for void
        template<typename R = Ret>
        typename enable_if<is_void<R>::value, void>::type operator() (Args... args) const
        {
            if (func)
            {
                func(const_cast<void *>(context), args...);
            }
        }

        // operator() for non void
        template<typename R = Ret>
        typename enable_if<!is_void<R>::value, R>::type operator() (Args... args) const
        {
            return func ? func(const_cast<void *>(context), args...) : R{};
        }

        explicit operator bool () const
        {
            return func != nullptr;
        }
    };



    using VOID_FUNCTION = PFunction<void()>;


    class PCLEANER // SIMPLEST RAII cleaner class !
    {
        VOID_FUNCTION clMethod; // will be called within destructor !
    public:
        PCLEANER(VOID_FUNCTION clMethod) : clMethod(clMethod)
        {

        }
        ~PCLEANER()
        {
            clMethod();
        }
    };

}

#include "inline/PFunction_inline.h"