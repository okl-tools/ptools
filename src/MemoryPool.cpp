#include "PTools.h"
#include "ObjectMemPool.h"


namespace ptools
{

    int32_t ObjectMemPool::count_mem_used () const
    {
        int32_t countBytes = 0;

        for (const HandleMemory & handle: arHandles)
        {
            countBytes += (handle.countBlocks * BLOCK_SIZE);
//                countBytes += sizeof (XHandle);
        }

        return countBytes;
    }

    ObjectMemPool::ObjectMemPool (int32_t block_size, int32_t max_blocks, void * pMem, ARRAY_HANDLES * ar) :
        BLOCK_SIZE(block_size),
        MAX_BLOCKS(max_blocks), arHandles(*ar)
    {
        if (BLOCK_SIZE <= 0 || MAX_BLOCKS <= 0 || BLOCK_SIZE % 8 != 0)
        {
            errMP = ERR_BLOCK_SIZE;
            pp("MemoryPool: Invalid BLOCK_SIZE:$ or MAX_BLOCKS:$ or BLOCK_SIZE not divisible by 8", BLOCK_SIZE, MAX_BLOCKS);
            return;
        }

        // Speicherpool initialisieren
        if (pMem == nullptr)
        {
            pp("ERR, MemoryPool: no mem");
            return;
        }
        else
        {
            pMemAll = (char *) pMem;
        }

    }

    ObjectMemPool::~ObjectMemPool ()
    {

    }

    void * ObjectMemPool::blocks_alloc (int countBlocks)
    {
//            pp("MemoryPool::blocks_alloc, countBlocks:$, $ bytes", countBlocks, countBlocks * BLOCK_SIZE);
        PMutexLocker lock(mutexMem);
        if (is_error_state())
        {
            return nullptr;
        }
        if (countBlocks <= 0)
        {
            errMP = ERR_INVALID_BLOCK_COUNT;
            pp("MemoryPool::blocks_alloc, Invalid block count");
            return nullptr;
        }
        int32_t start_index = find_consecutive_blocks(countBlocks);
        if (start_index < 0)
        {
            pp("MemoryPool::blocks_alloc, No consecutive blocks available");
            errMP = ERR_NO_CONSECUTIVE_BLOCKS;
            return nullptr;
        }
        char * block_addr = pMemAll + start_index * BLOCK_SIZE;
        HandleMemory handle{block_addr, countBlocks};
        if (!saveHandle(handle))
        {
            errMP = ERR_HANDLE_FULL;
            pp("MemoryPool::blocks_alloc, Failed to save handle, ARRAY_HANDLES full");
            return nullptr;
        }

        arHandles.sort([] (const HandleMemory & h1, const HandleMemory & h2) -> bool
                        {
                            return h1.pObject < h2.pObject;
                        });

//        pp("USED:$", count_mem_used());

//        show_info(4);

        return block_addr;
    }

    void * ObjectMemPool::alloc (int countBytes)
    {
        int countBlock = countBytes / BLOCK_SIZE;
        if ((countBytes % BLOCK_SIZE) > 0)
        {
            countBlock++;
        }

//        pp("alloc:$ bytes => $ blocks", countBytes, countBlock);

        return blocks_alloc(countBlock);
    }

    void ObjectMemPool::free_object (void * pObject)
    {
        if (pObject == nullptr)
        {
            errMP = ERR_INVALID_BLOCK;
            pp("MemoryPool::free_object - pObject == nullptr");
            return;
        }
        PMutexLocker lock(mutexMem);
        for (uint32_t i = 0; i < arHandles.size(); ++i)
        {
            HandleMemory & handle = arHandles[i];
            if (handle.pObject == pObject)
            {
                freeHandleMemory(handle);
                arHandles.delete_at(i);
                return;
            }
        }
        errMP = ERR_INVALID_BLOCK;
        pp("MemoryPool::free_object - pObject not found");
    }

    bool ObjectMemPool::saveHandle (HandleMemory handle)
    {
        uint32_t pos = 0;
        for (; pos < arHandles.size(); ++pos)
        {
            if (arHandles[pos].pObject > handle.pObject)
            {
                break;
            }
        }
        return arHandles.insert_at(pos, handle);
    }

    void ObjectMemPool::freeHandleMemory (HandleMemory handle)
    {
        if (handle.pObject == nullptr || handle.countBlocks <= 0)
        {
            errMP = ERR_INVALID_BLOCK;
            pp("freeHandleMemory: Invalid handle");
            return;
        }
        //errMP = ERR_NO_ERROR;
    }

    int32_t ObjectMemPool::get_max_blocks_free () const
    {
//        PMutexLocker lock(mutexMem);

        int32_t counter_max = 0;
        int32_t counter = 0;

        for (int32_t iBlock = 0; iBlock < MAX_BLOCKS; iBlock++)
        {
            bool flagIsFree = true;
            char * current_addr = pMemAll + (iBlock * BLOCK_SIZE);

            for (const auto & handle: arHandles)
            {
                void * handle_end = static_cast<char *>(handle.pObject) + handle.countBlocks * BLOCK_SIZE;
                if (current_addr >= handle.pObject && current_addr < handle_end)
                {
                    flagIsFree = false;
                    break;
                }
            }
            if (flagIsFree)
            {
                counter++;
                counter_max = std::max(counter_max, counter);
            }
            else
            {
                counter = 0;
            }
            iBlock++;
        }

        return counter_max;
    }

    int32_t ObjectMemPool::get_max_mem_free () const
    {
        return get_max_blocks_free() * BLOCK_SIZE;
    }

    int32_t ObjectMemPool::show_blocks () const
    {
        PMutexLocker lock(mutexMem);


//            pp("SHOW BLOCKS");

        int32_t max_consecutive = 0;
        int32_t current_consecutive = 0;
        int32_t current_index = 0;

        while (current_index < MAX_BLOCKS)
        {
            bool flagIsFree = true;
            char * current_addr = pMemAll + (current_index * BLOCK_SIZE);
            for (const auto & handle: arHandles)
            {
                void * handle_end = static_cast<char *>(handle.pObject) + handle.countBlocks * BLOCK_SIZE;
                if (current_addr >= handle.pObject && current_addr < handle_end)
                {
                    flagIsFree = false;
                    break;
                }
            }
            if (flagIsFree)
            {
                pp_string(".");
                current_consecutive++;
                max_consecutive = std::max(max_consecutive, current_consecutive);
            }
            else
            {
                pp_string("X");
                current_consecutive = 0;
            }
            current_index++;
        }

        pp_string("<<<\n");

        pp();

        //errMP = ERR_NO_ERROR;
        return max_consecutive;
    }

    void ObjectMemPool::show_info (uint32_t space, SHOW show) const
    {
        pp(space, "MemoryPool: MAX_MEM:$, BLOCK_SIZE:$, MAX_BLOCK_COUNT:$, MAX_OBJECTS:$, count objects:$, used mem:$, max-free:$",
           get_total_memsize(),
           BLOCK_SIZE,
           MAX_BLOCKS,
           arHandles.capacity(), // max objects
           arHandles.size(), // count objects
           count_mem_used(),
           get_max_blocks_free());

        if (is_error_state())
        {
            pp(space, "MemoryPool: err:$, <<<$>>>", errMP, error_as_string());
        }
        else
        {

        }

        if (show == SHOW::FULL && arHandles.size() > 0)
        {
            arHandles.show(space + 4);
        }
    }


    const char * ObjectMemPool::error_as_string () const
    {
        switch (errMP)
        {
            case ERR_NO_ERROR:
                return "ERR_NO_ERROR";
            case ERR_BLOCK_SIZE:
                return "ERR_BLOCK_SIZE";
            case ERR_COUNT_BLOCKS:
                return "ERR_COUNT_BLOCKS";
            case ERR_ALLOC:
                return "ERR_ALLOC";
            case ERR_INVALID_BLOCK_COUNT:
                return "ERR_INVALID_BLOCK_COUNT";
            case ERR_INVALID_BLOCK:
                return "ERR_INVALID_BLOCK";
            case ERR_ALREADY_FREE:
                return "ERR_ALREADY_FREE";
            case ERR_NO_CONSECUTIVE_BLOCKS:
                return "ERR_NO_CONSECUTIVE_BLOCKS";
            case ERR_HANDLE_FULL:
                return "ERR_HANDLE_FULL";
            case ERR_BLOCKS_RANGE:
                return "ERR_BLOCKS_RANGE";
            case ERR_MISC:
                return "ERR_MISC";
        }

        return "UNKNOWN ERROR";
    }


    int32_t ObjectMemPool::find_consecutive_blocks (int countBlocks)
    {
        if (countBlocks <= 0 || countBlocks > MAX_BLOCKS)
        {
            errMP = ERR_INVALID_BLOCK_COUNT;
            return -1;
        }
        const int32_t MEM_NEED = countBlocks * BLOCK_SIZE;
        int32_t current_index = 0;

        while (current_index <= (MAX_BLOCKS - countBlocks))
        {
            bool flagFree = true;
            const char * start_addr = pMemAll + (current_index * BLOCK_SIZE);
            const char * end_addr = start_addr + MEM_NEED;
            for (const auto & handle: arHandles)
            {
                const char * handle_end = ((const char *) handle.pObject) + (handle.countBlocks * BLOCK_SIZE);
                if (!(end_addr <= handle.pObject || start_addr >= handle_end))
                {
                    flagFree = false;
                    break;
                }
            }
            if (flagFree)
            {
//                    errMP = ERR_NO_ERROR;
                return current_index;
            }
//                pp(8, "current_index:$", current_index);
            current_index++;
        }

        pp("MemoryPool::find_consecutive_blocks, not enough blocks in a row for $ blocks needed", countBlocks);

        errMP = ERR_COUNT_BLOCKS;
        return -1;
    }

    int32_t ObjectMemPool::get_block_size () const
    {
        return BLOCK_SIZE;
    }

    int32_t ObjectMemPool::get_max_blocks () const
    {
        return MAX_BLOCKS;
    }

    int32_t ObjectMemPool::get_total_memsize () const
    {
        return BLOCK_SIZE * MAX_BLOCKS;
    }

    bool ObjectMemPool::is_error_state () const
    {
        return errMP != 0;
    }

    uint32_t ObjectMemPool::get_last_error () const
    {
        return errMP;
    }

    void ObjectMemPool::clear_error () const
    {
        errMP = ERR_NO_ERROR;
    }

    void ObjectMemPool::clear ()
    {
        arHandles.clear();
        errMP = ERR_NO_ERROR;
    }

}
