#pragma once

#include <cstddef>

#include "IObjectMemPool.h"
#include "PArrayFixed.h"
#include "PMutex.h"

namespace ptools
{
    using ARRAY_HANDLES = PArrayFixed<HandleMemory>;

    class ObjectMemPool : public IObjectMemPool
    {
    public:

        ObjectMemPool (int32_t block_size, int32_t max_blocks, void * pMem, ARRAY_HANDLES * ar);
        ~ObjectMemPool () override;


        void * blocks_alloc (int countBlocks = 1) override;
        void * alloc (int countBytes) override; // will mostly allocate more bytes as need

        int32_t count_mem_used () const;


        void free_object (void * pObject) override;

        bool saveHandle (HandleMemory handle);

        void freeHandleMemory (HandleMemory handle) override;

        int32_t get_block_size () const override;
        int32_t get_max_blocks () const override;
        int32_t get_total_memsize () const;
        int32_t get_max_mem_free () const override;
        int32_t get_max_blocks_free () const override;

        int32_t show_blocks () const override;


        void show_info (uint32_t space = 0, SHOW show = SHOW::BASIC) const override;


        bool is_error_state () const override;
        int32_t get_last_error () const override;
        void clear_error () const override;

        void clear() override;

        const char * error_as_string () const override;


    private:


        int32_t find_consecutive_blocks (int countBlocks);


    private:
        const int32_t BLOCK_SIZE;
        const int32_t MAX_BLOCKS;
        char * pMemAll = nullptr;
        ARRAY_HANDLES & arHandles;
        mutable PMutex mutexMem;
        mutable int32_t errMP = ERR_NO_ERROR;
    };

    //---------------------------------------------------------------------

    template<
            uint32_t BLOCK_SIZE,
            uint32_t BLOCK_COUNT,
            uint32_t OBJECTS_COUNT
    >
    class ObjectMemPoolWrapper
    {
        char buf[BLOCK_SIZE * BLOCK_COUNT] {}; // user memory
        char buf_ar[OBJECTS_COUNT * sizeof(HandleMemory)] {}; // managing objects

        PArrayFixed<HandleMemory> arHandleMemory;
        ObjectMemPool pool;

    public:

        IObjectMemPool & get_pool()
        {
            return pool;
        }
        operator IObjectMemPool & () // conversion operator for convenience
        {
            return pool;
        }

        explicit ObjectMemPoolWrapper () : arHandleMemory(buf_ar, OBJECTS_COUNT), pool(BLOCK_SIZE, BLOCK_COUNT, buf, &arHandleMemory)
        {
        }

        void show_info (uint32_t space = 0, SHOW show = SHOW::BASIC) const
        {
            pool.show_info(space, show);
        }

        void clear()
        {
            pool.clear();
        }

    };


}

