#pragma once
#include "Utils/StructureManager.h"

#include <assert.h>
#include <cstring>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <numeric>
#include <optional>
#include <utility>


// Pivot choosing macro for quicksort
// 0 = randomized
// 1 = median of three
// 2 = last element
#define PIVOT_PICK 1

//--------------------------------------------------------------------------------------------------------------------
// Unorded array class
//--------------------------------------------------------------------------------------------------------------------
template<class T>
class UnorderedArray
{
private:
    std::byte* m_pBuffer;

    size_t m_capacity; // was m_maxSize  
    size_t m_size;     // was m_numElements

public:
    UnorderedArray(size_t capacity = kInitialCapacity);
    UnorderedArray(const UnorderedArray& other);
    UnorderedArray(UnorderedArray&& other) noexcept;
    UnorderedArray& operator=(const UnorderedArray& other);
    UnorderedArray& operator=(UnorderedArray&& other) noexcept;
    ~UnorderedArray();

    // API
    // Modifiers
    void Clear();
    void PushBack(const T& val);
    void PushBack(T&& val);
    void PushFront(const T& val);
    template <class... Args> void EmplaceBack(Args&&... args);
    T PopBack();
    T PopFront();
    void Erase(size_t index);
    
    // Getters
    const T& operator[](size_t index) const;
    T& operator[](size_t index);
    std::optional<size_t> Search(const T& val) const;

    // Utils
    void Print() const;

    // Sorting algorithms
    void BubbleSort();
    void SelectionSort();
    void InsertionSort();
    void QuickSort();
    void Shuffle();

    // Getters
    size_t GetSize() const { return m_size; }
    size_t GetCapacity() const { return m_capacity; }
    bool Empty() const { return m_size <= 0; }

    // Test
    static void Test();

private:
    void Expand(size_t newCapacity);
    void Destroy();
    void QuickSort(size_t start, size_t end);
    size_t Partition(size_t start, size_t end);

#if PIVOT_PICK == 1
    size_t GetMidIndex(size_t start, size_t end) const;
#endif
};

//--------------------------------------------------------------------------------------------------------------------
// Ctor, takes in the size of the array and dynamically allocate in the ctor
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline UnorderedArray<T>::UnorderedArray(size_t capacity /*= kInitialCapacity*/)
    : m_pBuffer(nullptr)
    , m_capacity(capacity)
    , m_size(0)
{
    assert(m_capacity >= 0);
    Expand(m_capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// Copy ctor
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline UnorderedArray<T>::UnorderedArray(const UnorderedArray& other)
    : m_pBuffer(nullptr)
    , m_capacity(other.m_capacity)
    , m_size(other.m_size)
{
    // If the other's buffer exists
    if (other.m_pBuffer)
    {
        // Allocate new memory for buffer
        m_pBuffer = new std::byte[m_capacity * sizeof(T)];

        // Copy everything over
        T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
        T* pOtherTypeArray = reinterpret_cast<T*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) T(pOtherTypeArray[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Move ctor
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline UnorderedArray<T>::UnorderedArray(UnorderedArray&& other) noexcept
    : m_pBuffer(nullptr)
    , m_capacity(other.m_capacity)
    , m_size(other.m_size)
{
    // If other's buffer exists
    if (other.m_pBuffer)
    {
        // Allocate a new buffer
        m_pBuffer = new std::byte[m_capacity * sizeof(T)];

        // Move everything over
        T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
        T* pOtherTypeArray = reinterpret_cast<T*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) T(std::move(pOtherTypeArray[i]));

        // Destroy the other's stuff
        other.Clear();
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Move ctor
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline UnorderedArray<T>& UnorderedArray<T>::operator=(const UnorderedArray& other)
{
    // Edge-case checking
    if (this == &other)
        return *this;

    // Clean old buffer
    delete[] m_pBuffer;
    m_pBuffer = nullptr;

    // Copy everything over
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    // If the other's buffer exists
    if (other.m_pBuffer)
    {
        // Allocate new memory for buffer
        m_pBuffer = new std::byte[m_capacity * sizeof(T)];

        // Copy everything over
        T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
        T* pOtherTypeArray = reinterpret_cast<T*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) T(pOtherTypeArray[i]);
    }

    return (*this);
}

template<class T>
inline UnorderedArray<T>& UnorderedArray<T>::operator=(UnorderedArray&& other) noexcept
{
    // Edge-case checking
    if (this == &other)
        return *this;

    // Clean old buffer
    delete[] m_pBuffer;
    m_pBuffer = nullptr;

    // Move everything over
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_pBuffer = other.m_pBuffer;

    // Clear the other
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_pBuffer = nullptr;

    return (*this);
}

//--------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline UnorderedArray<T>::~UnorderedArray()
{
    Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Removes all elements from the array, set size back to 0
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Clear()
{
    Destroy();
    m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::PushBack(const T& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    //// Add element to the end of the array
    //m_pArray[m_size] = val;

    new(m_pBuffer + (m_size * sizeof(T))) T(val);  // std::byte* m_pBuffer
    ++m_size;
}

template<class T>
inline void UnorderedArray<T>::PushBack(T&& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    new(m_pBuffer + (m_size * sizeof(T))) T(val);  // std::byte* m_pBuffer
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Emplace an object at the end of the array
//--------------------------------------------------------------------------------------------------------------------
template<class T>
template<class ...Args>
inline void UnorderedArray<T>::EmplaceBack(Args && ...args)
{
    if (m_size >= m_capacity)
        Expand(m_capacity > 0 ? static_cast<size_t>(m_capacity * kExpandMultiplier) : kInitialCapacity);

    new(m_pBuffer + (m_size * sizeof(T))) T(std::forward<Args>(args)...);
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::PushFront(const T& val)
{
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's reference
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline T UnorderedArray<T>::PopBack()
{
    // Underflow checking
    assert(!Empty());

    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    T object = pTypeArray[m_size - 1];

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<T>)
        pTypeArray[m_size - 1].~T();

    // Reduce array size
    --m_size;

    // Return element
    return object;
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's reference
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline T UnorderedArray<T>::PopFront()
{
    // TODO: insert return statement here
}

//--------------------------------------------------------------------------------------------------------------------
// Takes an index of the element that should be removed, and removes it from the array.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Erase(size_t index)
{
    assert(index < m_size && !Empty());

    // Create TypeArray
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<T>)
        pTypeArray[index].~T();

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index, pTypeArray + index + 1, (m_size - index) * sizeof(T));

    // Reduce array size
    --m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Return a reference of a particular element.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline const T& UnorderedArray<T>::operator[](size_t index) const
{
    assert(index < m_size && !Empty());
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    return pTypeArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Return a reference of a particular element.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline T& UnorderedArray<T>::operator[](size_t index)
{
    assert(index < m_size && !Empty());
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    return pTypeArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Print out every element
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Print() const
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    std::cout << "Elements: { ";
    for (size_t i = 0; i < m_size; ++i)
        std::cout << pTypeArray[i] << ", ";
    std::cout << "} " << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Linear search
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline std::optional<size_t> UnorderedArray<T>::Search(const T& val) const
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    for (size_t i = 0; i < m_size; ++i)
    {
        if (pTypeArray[i] == val)
            return i;
    }
    return {};
}

//--------------------------------------------------------------------------------------------------------------------
// Bubble Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::BubbleSort()
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Loop through the array from back to begin, this is the unsorted part
    for (size_t unsortedIndex = m_size - 1; unsortedIndex > 0; --unsortedIndex)
    {
        // Loop through the array from begin to sorted part.
        for (size_t keyIndex = 0; keyIndex < unsortedIndex; ++keyIndex)
        {
            // If current key if great than the next element, which means we need to swap their location
            if (pTypeArray[keyIndex] > pTypeArray[keyIndex + 1])
                std::swap(pTypeArray[keyIndex], pTypeArray[keyIndex + 1]);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Selection Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::SelectionSort()
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Loop through the array
    for (size_t i = 0; i < m_size; ++i)
    {
        // Initial min index to current index
        size_t minIndex = i;

        // Loop through unsorted part of the array
        for (size_t unsortedIndex = i + 1; unsortedIndex < m_size; ++unsortedIndex)
        {
            // If current element is less than current min element, update min to current index
            if (pTypeArray[unsortedIndex] < pTypeArray[minIndex])
                minIndex = unsortedIndex;
        }

        // If min indexed element is less than current element
        if (pTypeArray[minIndex] < pTypeArray[i])
            std::swap(pTypeArray[minIndex], pTypeArray[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Insertion Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::InsertionSort()
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Loop through array from index 1
    for (size_t keyIndex = 1; keyIndex < m_size; ++keyIndex)
    {
        // Get the key at current index for comparing
        T key = pTypeArray[keyIndex];

        // Get an index for iterating through the sorted part of the array
        size_t sortedIndex = keyIndex - 1;

        // While we need to move key to left
        while (sortedIndex >= 0 && pTypeArray[sortedIndex] > key)
        {
            // Moves the element in sorted part
            pTypeArray[sortedIndex + 1] = pTypeArray[sortedIndex];
            --sortedIndex;
        }

        // Update the key to the correct location
        pTypeArray[sortedIndex + 1] = key;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Public Quick-sort interface
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::QuickSort()
{
    QuickSort(0, m_size - 1);
}

//--------------------------------------------------------------------------------------------------------------------
// Quick Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::QuickSort(size_t start, size_t end)
{
    if (start < end && end != std::numeric_limits<size_t>::max())
    {
        size_t pivotIndex = Partition(start, end);
        QuickSort(start, pivotIndex - 1);
        QuickSort(pivotIndex + 1, end);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Randomly rearranges elements in the array
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Shuffle()
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    for (size_t i = 0; i < m_size; ++i)
        std::swap(pTypeArray[i], pTypeArray[rand() % m_size]);
}

//--------------------------------------------------------------------------------------------------------------------
// Unit tests
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Test()
{
    // Variables for testing
    bool shouldQuit = false;
    size_t i = 0;       // Used as capacity and index
    T value = 0;		// Be used as value

    // I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
    // Create array
    UnorderedArray<T> unorderedArray{ static_cast<size_t>(12) };
    unorderedArray.PushBack(static_cast<T>(13));
    unorderedArray.PushBack(static_cast<T>(19));
    unorderedArray.PushBack(static_cast<T>(9));
    unorderedArray.PushBack(static_cast<T>(5));
    unorderedArray.PushBack(static_cast<T>(12));
    unorderedArray.PushBack(static_cast<T>(8));
    unorderedArray.PushBack(static_cast<T>(7));
    unorderedArray.PushBack(static_cast<T>(4));
    unorderedArray.PushBack(static_cast<T>(21));
    unorderedArray.PushBack(static_cast<T>(2));
    unorderedArray.PushBack(static_cast<T>(6));
    unorderedArray.PushBack(static_cast<T>(11));

#else
    // Get capacity from user
    std::cout << "Enter initial capacity: ";
    std::cin >> i;
    system("cls");

    // Create array
    UnorderedArray<Value> unorderedArray{ (size_t)i };
#endif

    // Loop work
    while (!shouldQuit)
    {
        // Print array
        unorderedArray.Print();

        // Get input
        char operationInput = StructureManager::Get().GetOperation(DataStructure::kUnorderedArray);

        // Do work
        switch (operationInput)
        {
        case '0':
            std::cout << "Enter pushing value: ";
            std::cin >> value;
            unorderedArray.PushBack(value);
            break;

        case '1':
            unorderedArray.Clear();
            break;

        case '2':
            unorderedArray.PopBack();
            break;

        case '3':
            std::cout << "Enter removing index: ";
            std::cin >> i;
            unorderedArray.Erase(i);
            break;

        case '4':            unorderedArray.InsertionSort();            break;
        case '5':            unorderedArray.SelectionSort();            break;
        case '6':            unorderedArray.BubbleSort();            break;
        case '7':            unorderedArray.QuickSort();            break;
        case '8':            unorderedArray.Shuffle();            break;
        case 'q':            shouldQuit = true;            break;
        }

        system("cls");
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Create a bigger array, update capacity, copy and move elements from previous array to the new one
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Expand(size_t newCapacity)
{
    const size_t newBufferSize = newCapacity * sizeof(T);
    std::byte* pNewBuffer = new std::byte[newBufferSize];

    // if the current buffer has data, copy it over to our new buffer and deallocate
    if (m_size > 0)
    {
        std::memcpy(pNewBuffer, m_pBuffer, sizeof(T) * m_size);
        delete[] m_pBuffer;
    }

    // point to the new buffer and new capacity
    m_pBuffer = pNewBuffer;
    m_capacity = newCapacity;
}

//--------------------------------------------------------------------------------------------------------------------
// Delete pArray and set it to nullptr
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void UnorderedArray<T>::Destroy()
{
    // If the type of elements is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            pTypeArray[i].~T();
    }

    delete[] m_pBuffer;
    m_pBuffer = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------
// Partition algorithm for QuickSort
// The array is divided into four regions.  They are:
//		1) <= pivot, from 0 to i
//		2) > pivot, from i to j 
//		3) Unrestricted / unknown, from j to pivot
//		4) pivot
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline size_t UnorderedArray<T>::Partition(size_t start, size_t end)
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

#if (PIVOT_PICK == 0)    // Randomized element
    // chose a random index from the array 
    // replace the element at that index with the element at the last index of the array.
    std::swap(pTypeArray[start + (rand() % (end - start))], pTypeArray[end]);

#elif (PIVOT_PICK == 1)    // Median of Three
    // examines the first, middle, and last elements of the array, find the middle's index
    // the value that is in the middle gets swapped with the last element
    std::swap(pTypeArray[GetMidIndex(start, end)], pTypeArray[end]);

#endif

    T pivot = pTypeArray[end];    // Pivot element value
    size_t i = start - 1;       // i is the last element's index of region 1, which is less than the pivot

    // j is the current processing element's index
    for (size_t j = start; j < end; ++j)
    {
        // See if current element should be placed to region one
        if (pTypeArray[j] <= pivot)
        {
            // Grow region 1.
            ++i;

            // Swap last element in region 1 with current processing element.
            std::swap(pTypeArray[i], pTypeArray[j]);
        }
    }

    // Everything is in its place except for the pivot.  We swap the pivot with the first element of region 2.
    size_t pivotIndex = i + 1;
    std::swap(pTypeArray[pivotIndex], pTypeArray[end]);

    // return the pivot, which becomes the beginning and end points of the next calls to Partition().
    return pivotIndex;
}

#if PIVOT_PICK == 1
//--------------------------------------------------------------------------------------------------------------------
// Helper function for Median of three QuickSort
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline size_t UnorderedArray<T>::GetMidIndex(size_t start, size_t end) const
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Get each elements
    T first = pTypeArray[start];
    T mid = pTypeArray[end / 2];
    T last = pTypeArray[end];

    // Compare and return mid index
    if (first > mid)
    {
        if (mid > last)
            return end / 2;
        else if (first > last)
            return end;
        else
            return start;
    }
    else
    {
        if (first > last)
            return start;
        else if (mid > last)
            return end;
        else
            return end / 2;
    }
}
#endif

