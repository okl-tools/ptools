#pragma one


namespace ptools
{


    template <typename K, typename V, size_t N>
    inline bool PMapFixed<K, V, N>::insert(const K& key, const V& val)
    {
        if (count >= N)
        {
            return false;
        }

        for (size_t i = 0; i < count; ++i)
        {
            if (entries[i].key == key)
            {
                return false;
            }
        }

        entries[count++] = { key, val };
        return true;
    }

    template <typename K, typename V, size_t N>
    inline V* PMapFixed<K, V, N>::find(const K& key)
    {
        for (size_t i = 0; i < count; ++i)
        {
            if (entries[i].key == key)
            {
                return &entries[i].val;
            }
        }
        return nullptr;
    }

    template <typename K, typename V, size_t N>
    inline const V* PMapFixed<K, V, N>::find(const K& key) const
    {
        for (size_t i = 0; i < count; ++i)
        {
            if (entries[i].key == key)
            {
                return &entries[i].val;
            }
        }
        return nullptr;
    }

    template <typename K, typename V, size_t N>
    inline size_t PMapFixed<K, V, N>::size() const
    {
        return count;
    }

    template <typename K, typename V, size_t N>
    inline typename PMapFixed<K, V, N>::Entry* PMapFixed<K, V, N>::begin()
    {
        return entries;
    }

    template <typename K, typename V, size_t N>
    inline typename PMapFixed<K, V, N>::Entry* PMapFixed<K, V, N>::end()
    {
        return entries + count;
    }

    template <typename K, typename V, size_t N>
    inline const typename PMapFixed<K, V, N>::Entry* PMapFixed<K, V, N>::begin() const
    {
        return entries;
    }

    template <typename K, typename V, size_t N>
    inline const typename PMapFixed<K, V, N>::Entry* PMapFixed<K, V, N>::end() const
    {
        return entries + count;
    }
}
