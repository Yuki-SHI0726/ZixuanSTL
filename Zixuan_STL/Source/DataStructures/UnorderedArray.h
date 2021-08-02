#pragma once
#include "Utils/StructureManager.h"
#include "Utils/Helpers.h"

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
template<class Type>
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
    void PushBack(const Type& val);
    void PushBack(Type&& val);
    void PushFront(const Type& val);
    void PushFront(Type&& val);
    template <class... Args> void EmplaceBack(Args&&... args);
    template <class... Args> void EmplaceFront(Args&&... args);
    Type PopBack();
    Type PopFront();
    void Erase(size_t index);
    
    // Getters
    const Type& operator[](size_t index) const;
    Type& operator[](size_t index);
    std::optional<size_t> Search(const Type& val) const;

    // Utils
    void Print(bool horizontal = true) const;

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
template<class Type>
inline UnorderedArray<Type>::UnorderedArray(size_t capacity /*= kInitialCapacity*/)
    : m_pBuffer(nullptr)
    , m_capacity(capacity)
    , m_size(0)
{
    assert(m_capacity >= 0);
    Expand(m_capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// Copy ctor
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline UnorderedArray<Type>::UnorderedArray(const UnorderedArray& other)
    : m_pBuffer(nullptr)
    , m_capacity(other.m_capacity)
    , m_size(other.m_size)
{
    // If the other's buffer exists
    if (other.m_pBuffer)
    {
        // Allocate new memory for buffer
        m_pBuffer = new std::byte[m_capacity * sizeof(Type)];

        // Copy everything over
        Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
        Type* pOtherTypeArray = reinterpret_cast<Type*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) Type(pOtherTypeArray[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Move ctor
// Time:  O(1)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline UnorderedArray<Type>::UnorderedArray(UnorderedArray&& other) noexcept
    : m_pBuffer(other.m_pBuffer)
    , m_capacity(other.m_capacity)
    , m_size(other.m_size)
{
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_pBuffer = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------
// Move ctor
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline UnorderedArray<Type>& UnorderedArray<Type>::operator=(const UnorderedArray& other)
{
    // Edge-case checking
    if (this == &other)
        return *this;

    // Clean old buffer
    delete[] m_pBuffer;
    m_pBuffer = other.m_pBuffer;

    // Copy everything over
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    other.m_pBuffer = nullptr;

    // If the other's buffer exists
    if (other.m_pBuffer)
    {
        // Allocate new memory for buffer
        m_pBuffer = new std::byte[m_capacity * sizeof(Type)];

        // Copy everything over
        Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
        Type* pOtherTypeArray = reinterpret_cast<Type*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) Type(pOtherTypeArray[i]);
    }

    return (*this);
}

//--------------------------------------------------------------------------------------------------------------------
// Move ctor
// Time:  O(1)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline UnorderedArray<Type>& UnorderedArray<Type>::operator=(UnorderedArray&& other) noexcept
{
    // Edge-case checking
    if (this == &other)
        return *this;

    // Clean old buffer
    delete[] m_pBuffer;

    // Move everything over
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_pBuffer = other.m_pBuffer;

    // Clear the other
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_pBuffer = nullptr;

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline UnorderedArray<Type>::~UnorderedArray()
{
    Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Removes all elements from the array, set size back to 0
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Clear()
{
    Destroy();
    m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::PushBack(const Type& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    //// Add element to the end of the array
    //m_pArray[m_size] = val;

    new(m_pBuffer + (m_size * sizeof(Type))) Type(val); 
    ++m_size;
}

template<class Type>
inline void UnorderedArray<Type>::PushBack(Type&& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    new(m_pBuffer + (m_size * sizeof(Type))) Type(val); 
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Emplace an object at the end of the array
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class ...Args>
inline void UnorderedArray<Type>::EmplaceBack(Args && ...args)
{
    if (m_size >= m_capacity)
        Expand(m_capacity > 0 ? static_cast<size_t>(m_capacity * kExpandMultiplier) : kInitialCapacity);

    new(m_pBuffer + (m_size * sizeof(Type))) Type(std::forward<Args>(args)...);
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Emplace an object at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class ...Args>
inline void UnorderedArray<Type>::EmplaceFront(Args && ...args)
{
    if (m_size >= m_capacity)
        Expand(m_capacity > 0 ? static_cast<size_t>(m_capacity * kExpandMultiplier) : kInitialCapacity);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    new(m_pBuffer) Type(std::forward<Args>(args)...);
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::PushFront(const Type& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    // Create the new element at front
    new(m_pBuffer) Type(val);

    // Increment size
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::PushFront(Type&& val)
{    
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    // Create the new element at front
    new(m_pBuffer) Type(val);

    // Increment size
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's copy
// Time:  O(1)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline Type UnorderedArray<Type>::PopBack()
{
    // Underflow checking
    assert(!Empty());

    // Grab the last item in the UnorderedArray
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    Type object = pTypeArray[m_size - 1];

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Type>)
        pTypeArray[m_size - 1].~Type();

    // Reduce array size
    --m_size;

    // Return element
    return object;
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's copy.
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use swap & pop instead
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline Type UnorderedArray<Type>::PopFront()
{
    // Underflow checking
    assert(!Empty());

    // Grab the first item's copy in the UnorderedArray
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    Type object = pTypeArray[0];

    // Shift every element one spot forward to fill the deleted item
    Erase(0);

    // Return element
    return object;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes an index of the element that should be removed, and removes it from the array.
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Erase(size_t index)
{
    assert(index < m_size && !Empty());

    // Create TypeArray
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Type>)
        pTypeArray[index].~Type();

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index, pTypeArray + index + 1, (m_size - index) * sizeof(Type));

    // Reduce array size
    --m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Return a reference of a particular element.
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type& UnorderedArray<Type>::operator[](size_t index) const
{
    assert(index < m_size && !Empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Return a reference of a particular element.
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline Type& UnorderedArray<Type>::operator[](size_t index)
{
    assert(index < m_size && !Empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Print out every element
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Print(bool horizontal /*= true*/) const
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    if (horizontal)
    {
        std::cout << "Elements: { ";
        for (size_t i = 0; i < m_size; ++i)
            std::cout << pTypeArray[i] << ", ";
    }
    else
    {
        std::cout << "Elements: { \n";
        for (size_t i = 0; i < m_size; ++i)
            std::cout << pTypeArray[i] << "\n";
    }

    std::cout << "} " << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Linear search
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline std::optional<size_t> UnorderedArray<Type>::Search(const Type& val) const
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    for (size_t i = 0; i < m_size; ++i)
    {
        if (pTypeArray[i] == val)
            return i;
    }
    return {};
}

//--------------------------------------------------------------------------------------------------------------------
// Bubble Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::BubbleSort()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Loop through the array from back to begin, this is the unsorted part
    for (size_t unsortedIndex = m_size - 1; unsortedIndex > 0; --unsortedIndex)
    {
        // Loop through the array from begin to sorted part.
        for (size_t keyIndex = 0; keyIndex < unsortedIndex; ++keyIndex)
        {
            // If current key if great than the next element, which means we need to swap their location
            if (pTypeArray[keyIndex] > pTypeArray[keyIndex + 1])
                Swap(pTypeArray[keyIndex], pTypeArray[keyIndex + 1]);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Selection Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::SelectionSort()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

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
            Swap(pTypeArray[minIndex], pTypeArray[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Insertion Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::InsertionSort()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Loop through array from index 1
    for (size_t keyIndex = 1; keyIndex < m_size; ++keyIndex)
    {
        // Get the key at current index for comparing
        Type key = pTypeArray[keyIndex];

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
// Time:  O(nlog(n))
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::QuickSort()
{
    QuickSort(0, m_size - 1);
}

//--------------------------------------------------------------------------------------------------------------------
// Quick Sort implementation
// Time:  O(nlog(n))
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::QuickSort(size_t start, size_t end)
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
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Shuffle()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    for (size_t i = 0; i < m_size; ++i)
        Swap(pTypeArray[i], pTypeArray[rand() % m_size]);
}

//--------------------------------------------------------------------------------------------------------------------
// Unit tests
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Test()
{
    // Variables for testing
    bool shouldQuit = false;
    size_t i = 0;       // Used as capacity and index
    Type value = 0;		// Be used as value

    // I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
    // Create array
    UnorderedArray<Type> unorderedArray{ static_cast<size_t>(12) };
    unorderedArray.PushBack(static_cast<Type>(13));
    unorderedArray.PushBack(static_cast<Type>(19));
    unorderedArray.PushBack(static_cast<Type>(9));
    unorderedArray.PushBack(static_cast<Type>(5));
    unorderedArray.PushBack(static_cast<Type>(12));
    unorderedArray.PushBack(static_cast<Type>(8));
    unorderedArray.PushBack(static_cast<Type>(7));
    unorderedArray.PushBack(static_cast<Type>(4));
    unorderedArray.PushBack(static_cast<Type>(21));
    unorderedArray.PushBack(static_cast<Type>(2));
    unorderedArray.PushBack(static_cast<Type>(6));
    unorderedArray.PushBack(static_cast<Type>(11));

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
template<class Type>
inline void UnorderedArray<Type>::Expand(size_t newCapacity)
{
    const size_t newBufferSize = newCapacity * sizeof(Type);
    std::byte* pNewBuffer = new std::byte[newBufferSize];

    // if the current buffer has data, copy it over to our new buffer and deallocate
    if (m_size > 0)
    {
        std::memcpy(pNewBuffer, m_pBuffer, sizeof(Type) * m_size);
        delete[] m_pBuffer;
    }

    // point to the new buffer and new capacity
    m_pBuffer = pNewBuffer;
    m_capacity = newCapacity;
}

//--------------------------------------------------------------------------------------------------------------------
// Delete pArray and set it to nullptr
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void UnorderedArray<Type>::Destroy()
{
    // If the type of elements is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Type>)
    {
        Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            pTypeArray[i].~Type();
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
template<class Type>
inline size_t UnorderedArray<Type>::Partition(size_t start, size_t end)
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

#if (PIVOT_PICK == 0)    // Randomized element
    // choose a random index from the array 
    // replace the element at that index with the element at the last index of the array.
    std::swap(pTypeArray[start + (rand() % (end - start))], pTypeArray[end]);

#elif (PIVOT_PICK == 1)    // Median of Three
    // examines the first, middle, and last elements of the array, find the middle's index
    // the value that is in the middle gets swapped with the last element
    Swap(pTypeArray[GetMidIndex(start, end)], pTypeArray[end]);

#endif

    Type pivot = pTypeArray[end];    // Pivot element value
    size_t i = start - 1;         // i is the last element's index of region 1, which is less than the pivot

    // j is the current processing element's index
    for (size_t j = start; j < end; ++j)
    {
        // See if current element should be placed to region one
        if (pTypeArray[j] <= pivot)
        {
            // Grow region 1.
            ++i;

            // Swap last element in region 1 with current processing element.
            Swap(pTypeArray[i], pTypeArray[j]);
        }
    }

    // Everything is in its place except for the pivot.  We swap the pivot with the first element of region 2.
    size_t pivotIndex = i + 1;
    Swap(pTypeArray[pivotIndex], pTypeArray[end]);

    // return the pivot, which becomes the beginning and end points of the next calls to Partition().
    return pivotIndex;
}

#if PIVOT_PICK == 1
//--------------------------------------------------------------------------------------------------------------------
// Helper function for Median of three QuickSort
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline size_t UnorderedArray<Type>::GetMidIndex(size_t start, size_t end) const
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Get each elements
    Type first = pTypeArray[start];
    Type mid = pTypeArray[end / 2];
    Type last = pTypeArray[end];

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

