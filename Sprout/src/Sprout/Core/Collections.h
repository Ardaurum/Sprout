#pragma once

#include "Sprout/Core/Core.h"

namespace Sprout
{
    // Sparse Array based on gfxlibrary: https://github.com/gboisse/gfx
    // License: https://github.com/gboisse/gfx/blob/master/LICENSE.md
    template<typename T>
    class SparseArray
    {
    public:
        inline SparseArray() noexcept;
        inline ~SparseArray() noexcept;

        inline SparseArray(SparseArray const& other) noexcept;
        inline SparseArray(SparseArray&& other) noexcept;

        inline SparseArray& operator=(SparseArray const& other) noexcept;
        inline SparseArray& operator=(SparseArray&& other) noexcept;

        inline T* At(uint32_t index);
        inline T& operator[](uint32_t index);
        inline T const* At(uint32_t index) const;
        inline T const& operator[](uint32_t index) const;
        inline bool Has(uint32_t index) const;
        inline bool IsEmpty() const;

        inline T& Insert(uint32_t index);
        inline T& Insert(uint32_t, T const& item);
        inline bool Erase(uint32_t index);
        inline void Clear();

        inline T* GetData();
        inline T const* GetData() const;
        inline uint32_t GetSize() const;
        inline uint32_t GetCapacity() const;

        inline uint32_t GetIndex(uint32_t packedIndex) const;
        inline uint32_t GetPosition(uint32_t index) const;

        inline void Swap(SparseArray& other) noexcept(std::_Is_nothrow_swappable<T>::value)
        {
            std::swap(Data, other.Data);
            std::swap(Indices, other.Indices);
            std::swap(Positions, other.Positions);
            std::swap(Size, other.Size);
            std::swap(Capacity, other.Capacity);
        }

    private:
        void Reserve(uint32_t capacity);

        T* Data;
        uint32_t Size;
        uint32_t Capacity;
        uint32_t* Indices;
        uint32_t* Positions;
    };

    template<typename T>
    void Swap(SparseArray<T>& lhs, SparseArray<T>& rhs) noexcept(noexcept(lhs.Swap(rhs)))
    {
        lhs.Swap(rhs);
    }

    template<typename T>
    SparseArray<T>::SparseArray() noexcept
        : Data(nullptr)
        , Size(0)
        , Capacity(0)
        , Indices(nullptr)
        , Positions(nullptr)
    {
    }

    template<typename T>
    SparseArray<T>::~SparseArray() noexcept
    {
        delete[] Data;
        delete[] Indices;
        delete[] Positions;
    }

    template<typename T>
    SparseArray<T>::SparseArray(SparseArray const& other) noexcept
    {
        T* data = new T[other.Capacity];
        uint32_t* indices = new uint32_t[other.Capacity];
        uint32_t* positions = new uint32_t[other.Capacity];

        for (uint32_t i = 0; i < other.Capacity; ++i)
        {
            data[i] = *new (&data[i]) T(other.Data[i]);
            indices[i] = other.Indices[i];
            positions[i] = other.Positions[i];
        }

        std::swap(Data, data);
        std::swap(Indices, indices);
        std::swap(Positions, positions);
        Capacity = other.Capacity;
        Size = other.Size;
    }

    template<typename T>
    SparseArray<T>::SparseArray(SparseArray&& other) noexcept
    {
        Data = other.Data;
        Indices = other.Indices;
        Positions = other.Positions;
        Size = other.Size;
        Capacity = other.Capacity;

        other.Data = nullptr;
        other.Indices = nullptr;
        other.Positions = nullptr;
        other.Size = 0;
        other.Capacity = 0;
    }

    template<typename T>
    SparseArray<T>& SparseArray<T>::operator=(SparseArray const& other) noexcept
    {
        SparseArray<T> tmp(other);
        this->Swap(tmp);
        return *this;
    }

    template<typename T>
    SparseArray<T>& SparseArray<T>::operator=(SparseArray&& other) noexcept
    {
        SparseArray<T> tmp(std::move(other));
        this->Swap(tmp);
        return *this;
    }

    template<typename T>
    T* SparseArray<T>::At(uint32_t index)
    {
        if (index >= Capacity)
        {
            return nullptr;
        }
        uint32_t const pId = Positions[index];
        if (pId == std::numeric_limits<uint32_t>::max())
        {
            return nullptr;
        }
        return &Data[pId];
    }

    template<typename T>
    T& SparseArray<T>::operator[](uint32_t index)
    {
        T* item = At(index);
        SPROUT_CORE_ASSERT_MSG(item, "Item at index {0} doesn't exist!", index);
        return *item;
    }

    template<typename T>
    T const* SparseArray<T>::At(uint32_t index) const
    {
        if (index >= capacity)
        {
            return nullptr;
        }
        uint32_t const pId = Positions[index];
        if (pId == std::numeric_limits<uint32_t>::max())
        {
            return nullptr;
        }
        return &Data[pId];
    }

    template<typename T>
    T const& SparseArray<T>::operator[](uint32_t index) const
    {
        T const* item = At(index);
        SPROUT_CORE_ASSERT_MSG(item, "Item at index {0} doesn't exist!", index);
        return *item;
    }

    template<typename T>
    bool SparseArray<T>::Has(uint32_t index) const
    {
        if (index >= Capacity)
        {
            return false;
        }
        return Positions[index] != std::numeric_limits<uint32_t>::max();
    }

    template<typename T>
    bool SparseArray<T>::IsEmpty() const
    {
        return Size == 0;
    }

    template<typename T>
    T& SparseArray<T>::Insert(uint32_t index)
    {
        SPROUT_CORE_ASSERT_MSG(index < std::numeric_limits<uint32_t>::max(), "Max Uint32_t is used as indication of missing element, thus index can't be set to it!");
        if (index > Capacity) Reserve(index + 1);
        uint32_t const pId = Positions[index];

        // Overwrite if exists
        if (pId != std::numeric_limits<uint32_t>::max())
        {
            return *new(&Data[pId]) T();
        }
        SPROUT_CORE_ASSERT_MSG(Size < Capacity, "Can't grow the array!");
        Indices[Size] = index;
        Positions[index] = Size;
        return *new(&Data[Size++]) T();
    }

    template<typename T>
    T& SparseArray<T>::Insert(uint32_t index, T const& item)
    {
        SPROUT_CORE_ASSERT_MSG(index < std::numeric_limits<uint32_t>::max(), "Max Uint32_t is used as indication of missing element, thus index can't be set to it!");
        if (index >= Capacity) Reserve(index + 1);
        uint32_t const pId = Positions[index];

        // Overwrite if exists
        if (pId != std::numeric_limits<uint32_t>::max())
        {
            return *new(&Data[pId]) T(item);
        }
        SPROUT_CORE_ASSERT_MSG(Size < Capacity, "Can't grow the array!");
        Indices[Size] = index;
        Positions[index] = Size;
        return *new(&Data[Size++]) T(item);
    }

    template<typename T>
    bool SparseArray<T>::Erase(uint32_t index)
    {
        SPROUT_CORE_ASSERT_MSG(index < Capacity, "Index points to non-existent element!");
        uint32_t const pId = Positions[index];
        if (pId == std::numeric_limits<uint32_t>::max())
        {
            return false;
        }
        SPROUT_ASSERT_MSG(Size > 0, "Array is not initialized!");
        if (pId != Size - 1)
        {
            std::swap(Data[pId], Data[Size - 1]);
            Indices[pId] = Indices[Size - 1];
            Positions[Indices[pId]] = pId;
        }
        Positions[index] = std::numeric_limits<uint32_t>::max();
        Indices[Size] = 0;
        return true;
    }

    template<typename T>
    void SparseArray<T>::Clear()
    {
        for (uint32_t i = 0; i < Size; ++i)
        {
            Positions[Indices[i]] = std::numeric_limits<uint32_t>::max();
            Indices[i] = 0;
        }
        Size = 0;
    }

    template<typename T>
    T* SparseArray<T>::GetData()
    {
        return Data;
    }

    template<typename T>
    T const* SparseArray<T>::GetData() const
    {
        return Data;
    }

    template<typename T>
    uint32_t SparseArray<T>::GetSize() const
    {
        return Size;
    }

    template<typename T>
    uint32_t SparseArray<T>::GetCapacity() const
    {
        return Capacity;
    }

    template<typename T>
    uint32_t SparseArray<T>::GetIndex(uint32_t position) const
    {
        SPROUT_CORE_ASSERT_MSG(position < Size, "Position points at an element outside of this SparseArray! Positions: {0}, Size: {1}", position, Size);
        return Indices[position];
    }

    template<typename T>
    uint32_t SparseArray<T>::GetPosition(uint32_t index) const
    {
        SPROUT_CORE_ASSERT_MSG(index < Capacity, "Index points at an element outside of this SparseArray! Index: {0}, Capacity: {1}", index, Capacity);
        return Positions[index];
    }

    template<typename T>
    void SparseArray<T>::Reserve(uint32_t capacity)
    {
        uint32_t const newCapacity = std::max(capacity, Capacity + ((Capacity + 2) >> 1));
        T* data = new T[newCapacity];
        uint32_t* indices = new uint32_t[newCapacity];
        uint32_t* positions = new uint32_t[newCapacity];

        for (uint32_t i = 0; i < Capacity; ++i)
        {
            if (i < Size)
            {
                new(&data[i]) T(std::move(Data[i]));
            }
            indices[i] = Indices[i];
            positions[i] = Positions[i];
        }

        for (uint32_t i = Capacity; i < newCapacity; ++i)
        {
            indices[i] = 0;
            positions[i] = std::numeric_limits<uint32_t>::max();
        }

        delete[] Data;
        delete[] Indices;
        delete[] Positions;

        Data = data;
        Indices = indices;
        Positions = positions;
        Capacity = newCapacity;
    }

    // Freelist based on gfxlibrary: https://github.com/gboisse/gfx
    // License: https://github.com/gboisse/gfx/blob/master/LICENSE.md
    class Freelist
    {
    public:
        inline Freelist() noexcept;
        inline explicit Freelist(char const* name) noexcept;
        inline ~Freelist() noexcept;

        inline Freelist(Freelist const& other) = delete;
        inline Freelist(Freelist&& other) noexcept;

        inline Freelist& operator=(Freelist const& other) = delete;
        inline Freelist& operator=(Freelist&& other) noexcept;

        inline bool IsEmpty() const;

        inline uint32_t AllocateSlot();
        inline uint32_t AllocateSlots(uint32_t slot_count);
        inline void FreeSlot(uint32_t slot);
        inline uint32_t GetSize() const;
        inline void Clear();

        inline uint32_t FreeSlotsCount() const;

        inline void Swap(Freelist& other) noexcept
        {
            std::swap(Name, other.Name);
            std::swap(Slots, other.Slots);
            std::swap(SlotCounts, other.SlotCounts);
            std::swap(NextSlot, other.NextSlot);
            std::swap(Size, other.Size);
        }

        static uint32_t const InvalidSlotConst = std::numeric_limits<uint32_t>::max() - 1;

    private:
        inline void Grow(uint32_t slot_count = 1);

        std::string Name;
        uint32_t* Slots;
        uint32_t* SlotCounts;
        uint32_t NextSlot;
        uint32_t Size;
    };

    void Swap(Freelist& lhs, Freelist& rhs) noexcept(noexcept(lhs.Swap(rhs)))
    {
        lhs.Swap(rhs);
    }

    Freelist::Freelist() noexcept
        : Name()
        , Slots(nullptr)
        , SlotCounts(nullptr)
        , NextSlot(std::numeric_limits<uint32_t>::max())
        , Size(0)
    {
    }

    Freelist::Freelist(char const* name) noexcept
        : Name(name)
        , Slots(nullptr)
        , SlotCounts(nullptr)
        , NextSlot(std::numeric_limits<uint32_t>::max())
        , Size(0)
    {
    }

    Freelist::~Freelist() noexcept
    {
#ifdef SPROUT_DEBUG
        uint32_t const freeSlotsCount = FreeSlotsCount();
        SPROUT_CORE_ASSERT_MSG(freeSlotsCount == Size, "Freelist: {0} wasn't freed properly; detected memory leak: {1}", Name.empty() ? "Freelist" : Name, Size - freeSlotsCount);
#endif //! _DEBUG
        Name = {};
        delete[] Slots;
        delete[] SlotCounts;
    }

    Freelist::Freelist(Freelist&& other) noexcept
    {
        Name = std::move(other.Name);
        Slots = other.Slots;
        SlotCounts = other.SlotCounts;
        NextSlot = other.NextSlot;
        Size = other.Size;

        other.Name = {};
        other.Slots = nullptr;
        other.SlotCounts = nullptr;
        other.NextSlot = std::numeric_limits<uint32_t>::max();
        other.Size = 0;
    }

    Freelist& Freelist::operator=(Freelist&& other) noexcept
    {
        Freelist tmp(std::move(other));
        this->Swap(tmp);
        return *this;
    }

    bool Freelist::IsEmpty() const
    {
        return FreeSlotsCount() == Size;
    }

    uint32_t Freelist::AllocateSlot()
    {
        if (NextSlot == std::numeric_limits<uint32_t>::max())
        {
            Grow();
        }
        SPROUT_CORE_ASSERT_MSG(NextSlot != std::numeric_limits<uint32_t>::max(), "Out of memory!");
        uint32_t const slot = NextSlot;
        NextSlot = Slots[NextSlot];
        Slots[slot] = InvalidSlotConst;
        SlotCounts[slot] = 1;
        return slot;
    }

    uint32_t Freelist::AllocateSlots(uint32_t slotCount)
    {
        uint32_t slot = std::numeric_limits<uint32_t>::max();
        if (slotCount == 0)
        {
            return std::numeric_limits<uint32_t>::max();
        }

        if (slotCount == 1)
        {
            return AllocateSlot();
        }

        for (uint32_t nextSlot = NextSlot; nextSlot != std::numeric_limits<uint32_t>::max(); nextSlot = Slots[nextSlot])
        {
            bool isBlockAvailable = true;
            if (nextSlot + slotCount > Size) continue;
            for (uint32_t i = 1; i < slotCount; ++i)
            {
                if (Slots[nextSlot + i] == InvalidSlotConst)
                {
                    isBlockAvailable = false;
                    break;
                }
            }
            if (isBlockAvailable)
            {
                slot = nextSlot;
                break;
            }
        }

        if (slot == std::numeric_limits<uint32_t>::max())
        {
            slot = Size;
            Grow(slotCount);
        }

        SPROUT_CORE_ASSERT_MSG(slot != std::numeric_limits<uint32_t>::max(), "No slot available!");
        SPROUT_CORE_ASSERT_MSG(slot + slotCount <= Size, "Can't allocate: {0} slots", slotCount);

        uint32_t previousSlot = std::numeric_limits<uint32_t>::max();
        uint32_t nextSlot = NextSlot;
        NextSlot = std::numeric_limits<uint32_t>::max();

        for (; nextSlot != std::numeric_limits<uint32_t>::max();)
        {
            if (nextSlot < slot || nextSlot >= slot + slotCount)
            {
                if (previousSlot == std::numeric_limits<uint32_t>::max())
                {
                    NextSlot = nextSlot;
                }
                previousSlot = nextSlot;
                nextSlot = Slots[nextSlot];
            }
            else
            {
                if (previousSlot != std::numeric_limits<uint32_t>::max())
                {
                    Slots[previousSlot] = Slots[nextSlot];
                }
                uint32_t const previousNextSlot = Slots[nextSlot];
                Slots[nextSlot] = InvalidSlotConst;
                nextSlot = previousNextSlot;
            }
        }

        for (uint32_t i = 0; i < slotCount; ++i)
        {
            SlotCounts[i + slot] = (i == 0 ? slotCount : 0);
        }

        return slot;
    }

    void Freelist::FreeSlot(uint32_t slot)
    {
        SPROUT_CORE_ASSERT_MSG(slot < Size, "Trying to free slot which wasn't allocated!");
        if (slot >= Size)
        {
            return;
        }

        SPROUT_CORE_ASSERT_MSG(Slots[slot] == InvalidSlotConst, "Slot is already freed!");
        if (Slots[slot] != InvalidSlotConst)
        {
            return;
        }

        SPROUT_CORE_ASSERT_MSG(SlotCounts[slot] > 0, "Cannot free within a block!");
        if (SlotCounts[slot] == 0)
        {
            return;
        }

        uint32_t i = slot + SlotCounts[slot];
        while (i-- > slot)
        {
            SPROUT_CORE_ASSERT_MSG(i == slot || SlotCounts[i] == 0, "");
            SPROUT_CORE_ASSERT_MSG(Slots[i] == InvalidSlotConst, "Slot is already freed!");
            Slots[i] = NextSlot;
            SlotCounts[i] = 0;
            NextSlot = i;
        }
    }

    uint32_t Freelist::GetSize() const
    {
        return Size;
    }

    void Freelist::Clear()
    {
        delete[] Slots;
        delete[] SlotCounts;

        Slots = nullptr;
        SlotCounts = nullptr;
        NextSlot = std::numeric_limits<uint32_t>::max();
        Size = 0;
    }

    uint32_t Freelist::FreeSlotsCount() const
    {
        uint32_t freeSlotCount = 0;
        for (uint32_t nextSlot = NextSlot; nextSlot != std::numeric_limits<uint32_t>::max(); nextSlot = Slots[NextSlot])
        {
            ++freeSlotCount;  // found an available slot
        }
        return freeSlotCount;
    }

    void Freelist::Grow(uint32_t slotCount)
    {
        uint32_t size = Size + slotCount;
        size += ((size + 2) >> 1);
        uint32_t* slots = new uint32_t[size];
        uint32_t* slotCounts = new uint32_t[size];
        SPROUT_CORE_ASSERT_MSG(slots != nullptr && slotCounts != nullptr, "Out of memory!");

        for (uint32_t i = 0; i < Size; ++i)
        {
            slots[i] = (Slots[i] != std::numeric_limits<uint32_t>::max() ? Slots[i] : Size);
            slotCounts[i] = SlotCounts[i];
        }

        for (uint32_t i = Size; i < size; ++i)
        {
            slots[i] = (i + 1 < size ? i + 1 : std::numeric_limits<uint32_t>::max());
            slotCounts[i] = 0;
        }

        NextSlot = (NextSlot != std::numeric_limits<uint32_t>::max() ? NextSlot : Size);
        delete[] SlotCounts;
        delete[] Slots;
        SlotCounts = slotCounts;
        Slots = slots;
        Size = size;
    }
}