#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <cstddef>
#include <iostream>
#include <vector>

#include "macros.hpp"

namespace mds {

template <typename T>
class MemoryPool final
{
public:
    explicit MemoryPool(std::size_t capacity);
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&&) = delete;
    template <typename... V>
    T* allocate(V&&...);
    void deallocate(T* item);
private:
    void updateNextFreeBlockIndex();

    struct ObjectBlock
    {
        T m_object;
        bool m_isFree{true};
    };

    std::vector<ObjectBlock> m_container;
    std::size_t m_nextFreeBlockIndex{0};
};

template <typename T>
MemoryPool<T>::MemoryPool(std::size_t capacity):m_container(capacity, {T(), true})
{
    ASSERT(reinterpret_cast<const ObjectBlock*>(&(m_container[0].m_object)) == &(m_container[0]),
            "T object should be the first member of ObjectBlock");
}

template <typename T>
template <typename... V>
T* MemoryPool<T>::allocate(V&&... vals)
{
    ObjectBlock* pObjectBlock = &m_container[m_nextFreeBlockIndex];
    ASSERT((pObjectBlock->m_isFree), "Expected free ObjectBlock at " + std::to_string(m_nextFreeBlockIndex));
    auto pNewObject= &(pObjectBlock->m_object);
    pNewObject= new(pNewObject)T(std::forward<V>(vals)...);
    pObjectBlock->m_isFree = false;
    updateNextFreeBlockIndex();
    return pNewObject;
}

template <typename T>
void MemoryPool<T>::updateNextFreeBlockIndex()
{
    auto initialFreeBlockIndex = m_nextFreeBlockIndex;
    while (!m_container[m_nextFreeBlockIndex].m_isFree) {
        ++m_nextFreeBlockIndex;
        if (m_nextFreeBlockIndex == m_container.size()) [[ unlikely ]] {
            m_nextFreeBlockIndex = 0;
        }

        if (m_nextFreeBlockIndex == initialFreeBlockIndex) [[ unlikely ]] {
            ASSERT(m_nextFreeBlockIndex != initialFreeBlockIndex,
                    "MemoryPool out of space.");
        }
    }
}

template <typename T>
void MemoryPool<T>::deallocate(T *item)
{
    auto pObjectBlock = reinterpret_cast<const ObjectBlock*>(item);
    auto rStartingObjectBlock = &m_container[0];
    std::size_t index = pObjectBlock - rStartingObjectBlock;
    std::cout << "Index: " << index << std::endl;
    ASSERT(index >= 0 && index < m_container.size(), "Object doesn't no belong to this memory pool");
    ASSERT(!m_container[index].m_isFree, "Expect in-use memory block at " + std::to_string(index));
    m_container[index].m_isFree = true;
}

}

#endif
