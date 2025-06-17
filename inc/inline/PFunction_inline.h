#pragma once

namespace ptools
{
    template<typename Ret, typename... Args, uint32_t StorageSize>
    template<typename F>
    inline PFunction<Ret (Args...), StorageSize>::PFunction (F f)
    {
        bind(f);
    }

    template<typename Ret, typename... Args, uint32_t StorageSize>
    template<typename F>
    inline PFunction<Ret(Args...), StorageSize> &  PFunction<Ret(Args...), StorageSize>::operator= (F f)
    {
        bind(f);
        return *this;
    }

    template<typename Ret, typename... Args, uint32_t StorageSize>
    template<typename T>
    inline void PFunction<Ret (Args...), StorageSize>::bind_member(Ret (T::*method)(Args...), T* instance)
    {
        struct Call
        {
            T* obj;
            Ret (T::*m)(Args...);

            Ret operator()(Args... args)
            {
                return (obj->*m)(args...);
            }
        };

        Call call = {instance, method};
        bind(call);
    }


    template<typename Ret, typename... Args, uint32_t StorageSize>
    template<typename F>
    inline void PFunction<Ret (Args...), StorageSize>::bind (F f)
    {
        using Functor = typename remove_reference<F>::type;

        static_assert(sizeof(Functor) <= StorageSize, "Capture too large for PFunction");

        func = [] (void * ctx, Args... args) -> Ret
        {
            return (*reinterpret_cast<Functor *>(ctx))(args...);
        };

        copier = [] (void * dst, const void * src)
        {
            new(dst) Functor(*reinterpret_cast<const Functor *>(src));
        };

        destroyer = [] (void * ptr)
        {
            reinterpret_cast<Functor *>(ptr)->~Functor();
        };

        context = storage;
        new(context) Functor(f); // Kopie
    }


    template<typename Ret, typename... Args, uint32_t StorageSize>
    inline PFunction<Ret(Args...), StorageSize> &  PFunction<Ret(Args...), StorageSize>::operator= (const PFunction & other)
    {
        if (this != &other)
        {
            if (destroyer)
            {
                destroyer(context);
            }

            func = other.func;
            copier = other.copier;
            destroyer = other.destroyer;
            context = storage;

            if (copier)
            {
                copier(storage, other.context);
            }
        }

        return *this;

    }


    template<typename Ret, typename... Args, uint32_t StorageSize>
    inline PFunction<Ret (Args...), StorageSize>::PFunction (const PFunction & other)
    {
        *this = other;
    }

    template<typename Ret, typename... Args, uint32_t StorageSize>
    inline PFunction<Ret (Args...), StorageSize>::~PFunction ()
    {
        if (destroyer)
        {
            destroyer(context);
        }
    }

}
