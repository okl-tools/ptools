// MemoryPoolRTOS.h

#pragma once

//#ifdef H7_FREE_RTOS


#include "IObjectMemPool.h"

#include <cmsis_os2.h>
#include <cstddef>
#include <stdint.h>

namespace ptools
{

    struct Mutex
    {
        osMutexId_t handleMutex;

        Mutex()
        {
            handleMutex = osMutexNew(nullptr);

            if (!handleMutex)
            {
                pp("⚠️  Mutex creation failed!");
            }
        }

        ~Mutex()
        {
            if (handleMutex)
            {
                osMutexDelete(handleMutex);
            }
        }

        // Nicht kopierbar
        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;

        osMutexId_t id() const
        {
            return handleMutex;
        }
    };

    struct MutexGuard
    {
        osMutexId_t handle;

        explicit MutexGuard(osMutexId_t mtx)
                : handle(mtx)
        {
            if (handle)
            {
                osMutexAcquire(handle, osWaitForever);
            }
            else
            {
                pp("MutexGuard: null handle!");
            }
        }

        ~MutexGuard()
        {
            if (handle)
            {
                osMutexRelease(handle);
            }
        }

        // Nicht kopierbar
        MutexGuard(const MutexGuard&) = delete;
        MutexGuard& operator=(const MutexGuard&) = delete;
    };

    //--------------------------------------------------------

    template<uint32_t BLOCK_SIZE, uint32_t MAX_BLOCK_COUNT>
    struct MemoryPoolRTOS : public IObjectMemPool
    {

        // Fehlercodes
        enum class ERR_POOL
        {
            SUCCESS = 0, INVALID_PARAM, OUT_OF_MEMORY, INVALID_BLOCK, ALREADY_FREE, MUTEX_INIT_FAILED
        };

        // Konstruktor: Initialisiert den zusammenhängenden Speicherpool und den Mutex
        MemoryPoolRTOS (void * memory = nullptr) : pPoolMemory(memory), flagIsOwner(false)
        {
            if (BLOCK_SIZE == 0 || MAX_BLOCK_COUNT == 0)
            {
                errPool = ERR_POOL::INVALID_PARAM;
                return;
            }

            // Wenn kein Speicher übergeben wurde, malloc verwenden
            if (pPoolMemory == nullptr)
            {
                pPoolMemory = malloc(BLOCK_SIZE * MAX_BLOCK_COUNT);
                flagIsOwner = true;
            }
            if (pPoolMemory == nullptr)
            {
//                osMutexDelete(mutex_id_);
//                mutex_id_ = nullptr;
                errPool = ERR_POOL::OUT_OF_MEMORY;
                return;
            }


            // Initialisiere freie Blöcke und Adressen
            for (int32_t i = 0; i < MAX_BLOCK_COUNT; ++i)
            {
                arBlockFree[i] = true;
                arBlockAdr[i] = static_cast<char *>(pPoolMemory) + i * BLOCK_SIZE;
            }

            errPool = ERR_POOL::SUCCESS;
        }

        // Allokiert countBlocks zusammenhängende Blöcke
        void * blocks_alloc (int countBlocks = 1) override
        {
            if (countBlocks <= 0 || countBlocks > static_cast<int>(MAX_BLOCK_COUNT))
            {
                errPool = ERR_POOL::INVALID_PARAM;
                return nullptr;
            }

            MutexGuard lock(myMutex.id());

            // Suche nach countBlocks zusammenhängenden freien Blöcken
            int32_t start_index = find_consecutive_blocks(countBlocks);
            if (start_index == MAX_BLOCK_COUNT)
            {
                errPool = ERR_POOL::OUT_OF_MEMORY;
                return nullptr;
            }

            // Markiere Blöcke als belegt
            for (int32_t i = start_index; i < start_index + countBlocks; ++i)
            {
                arBlockFree[i] = false;
            }

            errPool = ERR_POOL::SUCCESS;
            return arBlockAdr[start_index];
        }

        // Gibt countBlocks Blöcke ab block frei
        void blocks_free (void * block, int countBlocks = 1)
        {
            pp("MemoryPoolRTOS::blocks_free, $ blocks", countBlocks);
            if (block == nullptr || countBlocks <= 0 || countBlocks > static_cast<int>(MAX_BLOCK_COUNT))
            {
                errPool = ERR_POOL::INVALID_PARAM;
                pp("MemoryPoolRTOS::blocks_free, INVALID_PARAM");
                return;
            }

            MutexGuard lock(myMutex.id());


            // Finde den Index des Blocks
            int32_t index = get_block_index(block);
            if (index == MAX_BLOCK_COUNT || index + countBlocks > MAX_BLOCK_COUNT)
            {
                errPool = ERR_POOL::INVALID_BLOCK;
                pp("MemoryPoolRTOS::blocks_free, INVALID_BLOCK");
                return;
            }

            // Prüfe, ob die Blöcke tatsächlich belegt sind
            for (int32_t i = index; i < index + countBlocks; ++i)
            {
                if (arBlockFree[i])
                {
                    errPool = ERR_POOL::ALREADY_FREE;
                    pp("MemoryPoolRTOS::blocks_free, ALREADY_FREE");
                    return;
                }
            }

            // Markiere Blöcke als frei
            for (int32_t i = index; i < index + countBlocks; ++i)
            {
                arBlockFree[i] = true;
            }

            errPool = ERR_POOL::SUCCESS;
        }

        // Gibt die Blockgröße zurück
        int32_t get_block_size () const override
        {
            return BLOCK_SIZE;
        }

        // Gibt die maximale Anzahl an Blöcken zurück
        int32_t get_max_blocks () const override
        {
            return MAX_BLOCK_COUNT;
        }

        void show_info(uint16_t space) const
        {
//            MutexGuard lock(myMutex.id());
            pp(space, "MemoryPoolRTOS: BLOCK_SIZE:$, MAX_BLOCK_COUNT:$, free blocks:$", BLOCK_SIZE, MAX_BLOCK_COUNT, get_max_blocks_free());
        }

        void freeHandleMemory (HandleMemory handle) override
        {

        }

//        void saveHandle (XHandle handle) override
//        {
//
//        }

        void free_object (void * pObject) override
        {

        }


        // Gibt die maximale Anzahl zusammenhängender freier Blöcke zurück
        int32_t get_max_blocks_free () const override
        {

            MutexGuard lock(myMutex.id());

            int32_t max_consecutive = 0;
            int32_t current_consecutive = 0;

            for (int32_t i = 0; i < MAX_BLOCK_COUNT; ++i)
            {
                if (arBlockFree[i])
                {
                    current_consecutive++;
                    if (current_consecutive > max_consecutive)
                    {
                        max_consecutive = current_consecutive;
                    }
                }
                else
                {
                    current_consecutive = 0;
                }
            }

//            osMutexRelease(mutex_id_);
            errPool = ERR_POOL::SUCCESS;
            return max_consecutive;
        }

        // Destruktor: Gibt den Speicher und den Mutex frei
        ~MemoryPoolRTOS () override
        {
            if (flagIsOwner && pPoolMemory != nullptr)
            {
                free(pPoolMemory);
            }
        }

        // Gibt den letzten Fehler zurück
        ERR_POOL get_error () const
        {
            return errPool;
        }

    private:
        // Hilfsfunktion: Findet den Startindex von countBlocks zusammenhängenden freien Blöcken
        int32_t find_consecutive_blocks (int countBlocks)
        {
            int32_t consecutive = 0;
            int32_t start_index = MAX_BLOCK_COUNT;

            for (int32_t i = 0; i < MAX_BLOCK_COUNT; ++i)
            {
                if (arBlockFree[i])
                {
                    if (consecutive == 0)
                    {
                        start_index = i;
                    }
                    consecutive++;
                    if (consecutive == static_cast<int32_t>(countBlocks))
                    {
                        return start_index;
                    }
                }
                else
                {
                    consecutive = 0;
                    start_index = MAX_BLOCK_COUNT;
                }
            }
            return MAX_BLOCK_COUNT; // Keine ausreichend großen freien Blöcke gefunden
        }

        // Hilfsfunktion: Ermittelt den Index eines Blocks anhand seiner Adresse
        int32_t get_block_index (void * block)
        {
            char * ptr = static_cast<char *>(block);
            ptrdiff_t offset = ptr - static_cast<char *>(pPoolMemory);
            if (offset < 0 || offset % BLOCK_SIZE != 0)
            {
                return MAX_BLOCK_COUNT; // Ungültige Adresse
            }
            int32_t index = offset / BLOCK_SIZE;
            if (index >= MAX_BLOCK_COUNT)
            {
                return MAX_BLOCK_COUNT; // Außerhalb des Pools
            }
            return index;
        }

        void * pPoolMemory; // Zeiger auf den zusammenhängenden Speicherpool
        bool flagIsOwner; // Gibt an, ob der Pool-Speicher von der Klasse allokiert wurde
        bool arBlockFree[MAX_BLOCK_COUNT]; // Statisches Array für freie/belegte Blöcke
        void * arBlockAdr[MAX_BLOCK_COUNT]; // Statisches Array für Blockadressen
        mutable ERR_POOL errPool = ERR_POOL::SUCCESS; // Speichert den letzten Fehler (mutable für const-Methoden)

        mutable Mutex myMutex;
    };
}

//#endif
