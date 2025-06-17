// PMap.h
//
// okl, 2025 June 13
//
// PMap is a natural ordered map
// Simple
// Not fast, but working for small data
// ----------------------------------------

#pragma once

#include <vector>
#include "PArray.h"
#include "IObjectMemPool.h"

namespace ptools
{

    template<typename K, typename V>
    struct PMap
    {
        struct KeyVal
        {
            K key;
            V val;
        };

        PArray<KeyVal> vItems;

        PMap (IObjectMemPool & pool): vItems(pool)
        {
//            pp("sizeof DynArray<KeyVal>:$", sizeof(DynArray<KeyVal>));
        }


        V * find (const K & key)
        {
            for (auto & kv: vItems)
            {
                if (kv.key == key)
                {
                    return &kv.val;
                }
            }
            return nullptr;
        }

        const V * find (const K & key) const
        {
            for (const auto & kv: vItems)
            {
                if (kv.key == key)
                {
                    pp(4, "find <<<$>>> = <<<$>>>", kv.key, key);
                    return &kv.val;
                }
            }

//            pp("find nix");
            return nullptr;
        }

        bool contains (const K & key) const
        {
            return find(key) != nullptr;
        }


        V get_val (const K & key, const V & default_val = V{}) const
        {
            const V * v = find(key);

            return v ?
                   *v :
                   default_val;
        }


        V & at (const K & key)
        {
            V * v = find(key);
            if (!v)
            {
                pp("PMap::at, <<<$>>> not found", key);
//                static V val;
//                return val;
            }

            return *v;
        }
        const V & at (const K & key) const
        {
            V * v = find(key);
            if (!v)
            {
                pp("PMap::at, <<<$>>> not found", key);
//                static V val;
//                return val;
            }

            return *v;
        }
        bool replace (const K & key, const V & val)
        {
            if (find(key))
            {
                at(key) = val;
                return true;
            }
            return false;
        }

        void insert (K && key, V && val)
        {
            if (find(key))
            {
                pp("PMap::insert, already found!");
                return;
            }
//            vItems.push_back({key, val});

//            KeyVal kv ;
//            kv.key = std::move(key);
//            kv.val = std::move(val);

//            vItems.emplace_back(std::move(kv));
            vItems.emplace_back(std::move(KeyVal(std::move(key), std::move(val))));
        }

        void add (const K & key, const V & val)
        {
            if (contains(key))
            {
                pp("CONTAINS <$>", key);
                replace(key, val);
//                pNC->node.mapValues.insert(pNC->currentKey, n);
            }
            else
            {
//                pp("ATACH NODE, map:$", pNC->node.mapValues.size());
                insert_copy(key, val);
            }

        }
        void insert_copy (const K & key, const V & val)
        {
            if (find(key))
            {
                pp("PMap::insert_copy, already found!");
                return;
            }

//            pp("PMap::insert_copy, [$]:<<<$>>>", key, val);


//            vItems.push_back({key, val});

//            KeyVal kv ;
//            kv.key = std::move(key);
//            kv.val = std::move(val);

//            vItems.emplace_back(std::move(kv));
//            vItems.emplace_back(std::move(KeyVal(std::move(key), std::move(val))));
            vItems.push_back(KeyVal(key, val));
        }

//        void insert (const K & key, const V & val)
//        {
//            if (find(key))
//            {
//                return;
//            } // optional: no overwrite
//            vItems.push_back({key, val});
//        }
//
//        V & operator[] (const K & key)
//        {
//            if (V * v = find(key))
//            {
//                return *v;
//            }
//            vItems.push_back({key, V{}});
//            return vItems.back().val;
//        }

        KeyVal * get_key_val(const K & key)
        {
            if (V * v = find(key))
            {
                return *v;
            }

            return nullptr;
        }

        size_t size () const
        {
            return vItems.size();
        }

        auto begin ()
        {
            return vItems.begin();
        }

        auto end ()
        {
            return vItems.end();
        }

        auto begin () const
        {
            return vItems.begin();
        }

        auto end () const
        {
            return vItems.end();
        }

        void show(uint32_t space=0) const
        {
            pp(space, "PMap, size:$", size());

            for(const auto &kv : *this)
            {
                pp(space+4, "[$] = '$'", kv.key, kv.val);
            }

        }

        IObjectMemPool & get_pool()
        {
            return vItems.get_pool();
        }

    };

}
