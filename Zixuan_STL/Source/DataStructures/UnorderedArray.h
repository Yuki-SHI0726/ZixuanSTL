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
template<class Value>
class UnorderedArray
{
private:
    Value* m_pArray;

    size_t m_capacity; // was m_maxSize  
    size_t m_size;     // was m_numElements

public:
    UnorderedArray(size_t capacity);
    UnorderedArray();
    ~UnorderedArray();

    // API
    void Clear();
    void PushBack(const Value& val);
    void PushFront(const Value& val);
    Value& PopBack();
    Value& PopFront();
    void Erase(size_t index);

    Value& operator[](size_t index) const;

    void Print() const;
    std::optional<size_t> Search(const Value& val) const;

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
template<class Value>
inline UnorderedArray<Value>::UnorderedArray(size_t capacity)
    : m_pArray(nullptr)
    , m_capacity(0)
    , m_size(0)
{
    assert(capacity >= 0);
    Expand(capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// Default ctor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline UnorderedArray<Value>::UnorderedArray()
    : m_pArray(nullptr)
    , m_capacity(kInitialCapacity)
    , m_size(0)
{
    Expand(m_capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline UnorderedArray<Value>::~UnorderedArray()
{
    Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Removes all elements from the array, set size back to 0
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::Clear()
{
    Destroy();
    m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::PushBack(const Value& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // Add element to the end of the array
    m_pArray[m_size] = val;
    //new(m_pBuffer + (m_size * sizeof(T))) T(val);  // std::byte* m_pBuffer
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::PushFront(const Value& val)
{
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's reference
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value& UnorderedArray<Value>::PopBack()
{
    // Underflow checking
    assert(!Empty());

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Value>)
        m_pArray[m_size - 1].~T();

    // Reduce array size
    --m_size;

    // Return element
    return m_pArray[m_size];
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array. Return the element's reference
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value& UnorderedArray<Value>::PopFront()
{
    // TODO: insert return statement here
}

//--------------------------------------------------------------------------------------------------------------------
// Takes an index of the element that should be removed, and removes it from the array.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::Erase(size_t index)
{
    assert(index >= 0 && index < m_size && !Empty());

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Value>)
        m_pArray[index].~T();

    // Move every element after the indexed element one spot forward.
    for (size_t i = index; i < (m_size - 1); ++i)
        m_pArray[i] = m_pArray[i + 1];

    // Reduce array size
    --m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Return a reference of a particular element.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value& UnorderedArray<Value>::operator[](size_t index) const
{
    assert(index >= 0 && index < m_size && !Empty());
    return m_pArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Print out every element
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::Print() const
{
    std::cout << "Elements: { ";
    for (size_t i = 0; i < m_size; ++i)
        std::cout << m_pArray[i] << ", ";
    std::cout << "} " << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Linear search
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::optional<size_t> UnorderedArray<Value>::Search(const Value& val) const
{
    for (size_t i = 0; i < m_size; ++i)
    {
        if (m_pArray[i] == val)
            return i;
    }
    return {};
}

//--------------------------------------------------------------------------------------------------------------------
// Bubble Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::BubbleSort()
{
    static_assert(std::is_arithmetic_v<Value>, "Template parameter must be an arithmetic type");

    // Loop through the array from back to begin, this is the unsorted part
    for (size_t unsortedIndex = m_size - 1; unsortedIndex > 0; --unsortedIndex)
    {
        // Loop through the array from begin to sorted part.
        for (size_t keyIndex = 0; keyIndex < unsortedIndex; ++keyIndex)
        {
            // If current key if great than the next element, which means we need to swap their location
            if (m_pArray[keyIndex] > m_pArray[keyIndex + 1])
                std::swap(m_pArray[keyIndex], m_pArray[keyIndex + 1]);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Selection Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::SelectionSort()
{
    static_assert(std::is_arithmetic_v<Value>, "Template parameter must be an arithmetic type");

    // Loop through the array
    for (size_t i = 0; i < m_size; ++i)
    {
        // Initial min index to current index
        size_t minIndex = i;

        // Loop through unsorted part of the array
        for (size_t unsortedIndex = i + 1; unsortedIndex < m_size; ++unsortedIndex)
        {
            // If current element is less than current min element, update min to current index
            if (m_pArray[unsortedIndex] < m_pArray[minIndex])
                minIndex = unsortedIndex;   
        }

        // If min indexed element is less than current element
        if (m_pArray[minIndex] < m_pArray[i])
            std::swap(m_pArray[minIndex], m_pArray[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Insertion Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::InsertionSort()
{
    static_assert(std::is_arithmetic_v<Value>, "Template parameter must be an arithmetic type");

    // Loop through array from index 1
    for (size_t keyIndex = 1; keyIndex < m_size; ++keyIndex)
    {
        // Get the key at current index for comparing
        Value key = m_pArray[keyIndex];

        // Get an index for iterating through the sorted part of the array
        size_t sortedIndex = keyIndex - 1;
         
        // While we need to move key to left
        while (sortedIndex >= 0 && m_pArray[sortedIndex] > key)
        {
            // Moves the element in sorted part
            m_pArray[sortedIndex + 1] = m_pArray[sortedIndex];
            --sortedIndex;
        }

        // Update the key to the correct location
        m_pArray[sortedIndex + 1] = key;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Public Quick-sort interface
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::QuickSort()
{
    //static_assert(std::is_arithmetic_v<Value>, "Template parameter must be an arithmetic type");
    QuickSort(0, m_size - 1);
}

//--------------------------------------------------------------------------------------------------------------------
// Quick Sort implementation
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::QuickSort(size_t start, size_t end)
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
template<class Value>
inline void UnorderedArray<Value>::Shuffle()
{
    for (size_t i = 0; i < m_size; ++i) 
        std::swap(m_pArray[i], m_pArray[rand() % m_size]);
}

//--------------------------------------------------------------------------------------------------------------------
// Unit tests
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::Test()
{
    // Variables for testing
    bool shouldQuit = false;
    size_t i = 0;       // Used as capacity and index
    Value value = 0;		// Be used as value

    // I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
    // Create array
    UnorderedArray<Value> unorderedArray{ static_cast<size_t>(12) };
    unorderedArray.PushBack(static_cast<Value>(13));
    unorderedArray.PushBack(static_cast<Value>(19));
    unorderedArray.PushBack(static_cast<Value>(9));
    unorderedArray.PushBack(static_cast<Value>(5));
    unorderedArray.PushBack(static_cast<Value>(12));
    unorderedArray.PushBack(static_cast<Value>(8));
    unorderedArray.PushBack(static_cast<Value>(7));
    unorderedArray.PushBack(static_cast<Value>(4));
    unorderedArray.PushBack(static_cast<Value>(21));
    unorderedArray.PushBack(static_cast<Value>(2));
    unorderedArray.PushBack(static_cast<Value>(6));
    unorderedArray.PushBack(static_cast<Value>(11));

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
template<class Value>
inline void UnorderedArray<Value>::Expand(size_t newCapacity)
{
    assert(newCapacity >= m_capacity);

    // Update capacity
    m_capacity = newCapacity;

    // Create a new array with new capacity.
    Value* pNewArray = new Value[m_capacity];

    // If the current array has data, copy it over to our new array and deallocate it
    if (m_size > 0)
    {
        std::memcpy(pNewArray, m_pArray, sizeof(Value) * m_size);
        Destroy();
    }

    // Update m_pArray to pNewArray
    m_pArray = pNewArray;
}

//--------------------------------------------------------------------------------------------------------------------
// Delete pArray and set it to nullptr
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void UnorderedArray<Value>::Destroy()
{
    // If the type of elements is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Value>)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_pArray[i].~Value();
    }

    // If m_pArray is not nullptr, deallocate it and set it to nullptr
    if (m_pArray != nullptr)
    {
        delete[] m_pArray;
        m_pArray = nullptr;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Partition algorithm for QuickSort
// The array is divided into four regions.  They are:
//		1) <= pivot, from 0 to i
//		2) > pivot, from i to j 
//		3) Unrestricted / unknown, from j to pivot
//		4) pivot
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline size_t UnorderedArray<Value>::Partition(size_t start, size_t end)
{
#if (PIVOT_PICK == 0)    // Randomized element
    // chose a random index from the array 
    // replace the element at that index with the element at the last index of the array.
    std::swap(pTypeArray[start + (rand() % (end - start))], pTypeArray[end]);

#elif (PIVOT_PICK == 1)    // Median of Three
    // examines the first, middle, and last elements of the array, find the middle's index
    // the value that is in the middle gets swapped with the last element
    std::swap(m_pArray[GetMidIndex(start, end)], m_pArray[end]);

#endif

    Value pivot = m_pArray[end];    // Pivot element value
    size_t i = start - 1;       // i is the last element's index of region 1, which is less than the pivot

    // j is the current processing element's index
    for (size_t j = start; j < end; ++j)
    {
        // See if current element should be placed to region one
        if (m_pArray[j] <= pivot)
        {
            // Grow region 1.
            ++i;

            // Swap last element in region 1 with current processing element.
            std::swap(m_pArray[i], m_pArray[j]);
        }
    }

    // Everything is in its place except for the pivot.  We swap the pivot with the first element of region 2.
    size_t pivotIndex = i + 1;
    std::swap(m_pArray[pivotIndex], m_pArray[end]);

    // return the pivot, which becomes the beginning and end points of the next calls to Partition().
    return pivotIndex;
}

#if PIVOT_PICK == 1
//--------------------------------------------------------------------------------------------------------------------
// Helper function for Median of three QuickSort
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline size_t UnorderedArray<Value>::GetMidIndex(size_t start, size_t end) const
{
    // Get each elements
    Value first = m_pArray[start];
    Value mid = m_pArray[end / 2];
    Value last = m_pArray[end];

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
