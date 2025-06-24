// IMemoryPool.h
//
// okl, 2025 June 12
//
// Interface for memory management in context of ptools
// IMemoryPool is used by PString, DynArray, PStack, NaturalOrderedMap, PTokenizer, Node (json)
// C++ Desktop => C++ embedded:
// Wherever you want to create objects via "new" you could make use of template function "IMemoryPool::create_object"
//
//
//
// Concrete Implementation here: MemoryPool
//

#pragma once


#include "PTools.h"

namespace ptools
{
    struct PString;

    enum MEM_POOL_ERR
    {
        ERR_NO_ERROR = 0,
        ERR_BLOCK_SIZE = 1,
        ERR_COUNT_BLOCKS = 2,
        ERR_ALLOC = 3,
        ERR_INVALID_BLOCK_COUNT = 4,
        ERR_INVALID_BLOCK = 5,
        ERR_NO_CONSECUTIVE_BLOCKS = 6,
        ERR_ALREADY_FREE = 8,
        ERR_HANDLE_FULL = 9,
        ERR_BLOCKS_RANGE = 10,
        ERR_MISC = 100,
    };


    struct HandleMemory
    {
        void * pObject = nullptr;
        int32_t countBlocks = 0;

        bool is_OK () const
        {
            return pObject != nullptr && countBlocks > 0;
        }

        template<typename T>
        T * to_object ()
        {
            return static_cast<T *>(pObject);
        }


        void to_writer (PWriter & writer) const
        {
            char arHex[34];

#ifdef PLATFORM_64BIT
            uint64_t adr = (long long) pObject;
            get_hex_string(adr, arHex);
#else
            uint32_t adr = (uint32_t ) pObject;
            get_hex_string(adr, arHex);
#endif
            writer.sprintF("HandleMemory:$ blocks, start at:$", countBlocks, arHex);
        }


    };


    struct IObjectMemPool
    {
        // Allocate memory - but preferred way is template create_object !
        virtual void * blocks_alloc (int countBlocks = 1) = 0;
        virtual void * alloc (int countBytes) = 0; // according to blocksize + rest

        // Free any kind of object - also raw mem
        virtual void free_object (void * pObject) = 0;

        // Create any kind of object - works with placement new
        template<typename T, typename... Args>
        T * create_object (Args && ... args)
        {
            HandleMemory handle = create_handle_memory<T>(args ...);

            return handle.is_OK() ?
                   handle.to_object<T>() :
                   nullptr;

        }

        PString * create_PString (const char * pStr); // for convience ony, it's the same like:  create_object<PString>() + call of set_obj_mem_pool

        template<typename T, typename... ARGS>
        PString * create_PString (const char * format, const T & value, const ARGS... args);


        virtual int32_t get_block_size () const = 0;
        virtual int32_t get_max_blocks () const = 0;
        virtual int32_t get_max_blocks_free () const = 0;
        virtual int32_t get_max_mem_free () const = 0;


        virtual void show_info (uint32_t space = 0, SHOW show = SHOW::BASIC) const = 0;
        virtual int32_t show_blocks () const = 0;

        virtual bool is_error_state () const = 0;
        virtual uint32_t get_last_error () const = 0;
        virtual void clear_error () const = 0;
        virtual const char * error_as_string () const = 0;
        virtual void clear () = 0;


        virtual ~IObjectMemPool ()
        {
        }

    protected:

        virtual void freeHandleMemory (HandleMemory handle) = 0;

        template<typename T, typename... Args>
        HandleMemory create_handle_memory (Args && ... args)
        {
            // check for correct blocksize

            const int32_t block_size = get_block_size();
            if (block_size == 0)
            {
                pp("create_object: Invalid block size");
                return HandleMemory{nullptr, ERR_BLOCK_SIZE};
            }

            // calc amount of blocks for T
            const int32_t required_size = sizeof(T);
            const int32_t countBlocks = (required_size + block_size - 1) / block_size;

            const int countFreeBlocks = get_max_blocks_free();

            if (countBlocks > countFreeBlocks)
            {
                pp("create_handle_memory: Not enough blocks");
                return HandleMemory{nullptr, ERR_COUNT_BLOCKS};
            }

            // alloc memory
            void * ptr = blocks_alloc(countBlocks);
            if (ptr == nullptr)
            {
                pp("create_handle_memory: Allocation failed");
                return HandleMemory{nullptr, ERR_ALLOC};
            }

            return HandleMemory{new(ptr) T(std::forward<Args>(args)...), countBlocks};
        }

    };


}