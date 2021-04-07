#pragma once
#include "Utils/StructureManager.h"

#include <assert.h>
#include <cstring>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <optional>
#include <conio.h>

//--------------------------------------------------------------------------------------------------------------------
// Ordered array class, I didn't make it derived from UnorderedArray class because we want data structures as fast as possible
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
class OrderedArray
{
private:
    Value* m_pArray;

    size_t m_capacity; // was m_maxSize  
    size_t m_size;     // was m_numElements
    bool m_isIncreasingOrder;   // Used for ordering decreasing or increasing

public:
    OrderedArray(size_t capacity, bool isIncreasingOrder = true);
    OrderedArray(bool isIncreasingOrder = true);
    ~OrderedArray();

    // API
    void Clear();
    void Push(const Value& val);
    Value& Pop();
    void Remove(size_t index);
    Value& operator[](size_t index) const;
    void Print() const;
    void Reverse();
    std::optional<size_t> Search(const Value& val);

    // Getters
    size_t GetSize() const { return m_size; }
    size_t GetCapacity() const { return m_capacity; }

    // Test
    static void Test();

private:
    void Expand(size_t newCapacity);
    void Destroy();
    void InternalPush(const Value& val);
    std::optional<size_t> BinarySearch(const Value& val, size_t start, size_t end);
};

//--------------------------------------------------------------------------------------------------------------------
// Ctor, takes in the size of the array and dynamically allocate in the ctor, takes in a boolean of ordering method
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline OrderedArray<Value>::OrderedArray(size_t capacity, bool isIncreasingOrder)
    : m_pArray(nullptr)
    , m_capacity(capacity)
    , m_size(0)
    , m_isIncreasingOrder{ isIncreasingOrder }
{
    assert(capacity >= 0);
    Expand(capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// Default ctor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline OrderedArray<Value>::OrderedArray(bool isIncreasingOrder)
    : m_pArray(nullptr)
    , m_capacity(kInitialCapacity)
    , m_size(0)
    , m_isIncreasingOrder{ isIncreasingOrder }
{
    Expand(m_capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline OrderedArray<Value>::~OrderedArray()
{
    Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Removes all elements from the array, set size back to 0
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Clear()
{
    std::memset(m_pArray, 0, sizeof(Value) * m_size);
    m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Push(const Value& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // For adding the first element to the array
    if (m_size == 0)
        m_pArray[0] = val;
    // If it's not the first element in the array, find then add element to correct location
    else
        InternalPush(val);

    // Increase size
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value& OrderedArray<Value>::Pop()
{
    // Error checking
    assert(m_size > 0);

    // If the element is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Value>)
        m_pArray[m_size - 1].~T();

    // Reduce array size
    --m_size;

    // Return element
    return m_pArray[m_size];
}

//--------------------------------------------------------------------------------------------------------------------
// Takes an index of the element that should be removed, and removes it from the array.
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Remove(size_t index)
{
    assert(index >= 0 && index <= (m_size - 1) && m_size > 0);

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
inline Value& OrderedArray<Value>::operator[](size_t index) const
{
    assert(index >= 0 && index <= (m_size - 1));
    return m_pArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Print out every element
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Print() const
{
    std::cout << "Elements: { ";
    for (size_t i = 0; i < m_size; ++i)
        std::cout << m_pArray[i] << ", ";
    std::cout << "} " << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Reverse the ordering method in array
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Reverse()
{
    // Update order boolean
    m_isIncreasingOrder = !m_isIncreasingOrder;

    // Reverse the whole array
    for (size_t i = 0; i < m_size / 2; ++i)
        std::swap(m_pArray[i], m_pArray[m_size - i - 1]);
}

//--------------------------------------------------------------------------------------------------------------------
// Purpose: Search a certain value passed in
//
// Parameter:
//      - val: The value to search in the structure
//
// Return:
//      - the index of the found element
//      - -1 if the element is not found.
//
// Time:  O(log(n))
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::optional<size_t> OrderedArray<Value>::Search(const Value& val)
{
    return BinarySearch(val, 0, m_size);
}

template<class Value>
inline void OrderedArray<Value>::Test()
{
    // Variables for testing
    bool shouldQuit = false;
    Value value = 0;		// Be used as capacity, value, and index
    size_t i = 0;       // Used as capacity and index
    bool isIncreasingOrder = true;
    char operationInput = ' ';

    // I'm tired typing in initial size and elements to test stuff
#if _DEBUG
    // Create array
    OrderedArray<Value> orderedArray{ static_cast<size_t>(12) };
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));
    orderedArray.Push(static_cast<Value>(rand() % 100));

#else
    // Get capacity from user
    std::cout << "Enter initial capacity: ";
    std::cin >> i;

    // Get capacity from user
    std::cout << "\nEnter array's ordering method: " << std::endl;
    std::cout << "1) Increasing " << std::endl;
    std::cout << "2) Decreasing" << std::endl;
    operationInput = _getch();
    system("cls");

    // Create array
    OrderedArray<Value> orderedArray{ i, (operationInput == '1') };
#endif

    // Loop work
    while (!shouldQuit)
    {
        // Print array
        orderedArray.Print();

        // Get input
        operationInput = StructureManager::Get().GetOperation(DataStructure::kOrderedArray);

        // Do work
        switch (operationInput)
        {
        case '0':
            std::cout << "Enter pushing value: ";
            std::cin >> value;
            orderedArray.Push(value);
            break;

        case '1':
            orderedArray.Clear();
            break;

        case '2':
            orderedArray.Pop();
            break;

        case '3':
            std::cout << "Enter removing index: ";
            std::cin >> value;
            orderedArray.Remove(value);
            break;

        case '4':
            orderedArray.Reverse();
            break;

        case '5':
        {
            std::cout << "Enter searching value: ";
            std::cin >> value;

            std::optional<size_t> result = orderedArray.Search(value);
            if (result.has_value())
                std::cout << result.value() << std::endl;
            else
                std::cout << "Value is not found." << std::endl;

            system("pause");
            break;
        }
        case 'q':
            shouldQuit = true;
            break;
        }

        system("cls");
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Create a bigger array, update capacity, copy and move elements from previous array to the new one
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::Expand(size_t newCapacity)
{
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
inline void OrderedArray<Value>::Destroy()
{
    // If the type of elements is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<Value>)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_pArray[i].~T();
    }

    // If m_pArray is not nullptr, deallocate it and set it to nullptr
    if (m_pArray != nullptr)
    {
        delete[] m_pArray;
        m_pArray = nullptr;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Push a new element into the current location
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void OrderedArray<Value>::InternalPush(const Value& val)
{
    // Walk through the array
    for (size_t i = 0; i < m_size; ++i)
    {
        // If current element is greater than the inserting value
        if (m_isIncreasingOrder ? (m_pArray[i] > val) : (m_pArray[i] < val))
        {
            // Move every elements one spot backward
            for (size_t j = m_size; j > i; --j)
                m_pArray[j] = m_pArray[j - 1];

            // Update current spot to value
            m_pArray[i] = val;

            // Get out of the outter loop
            break;
        }
        // if it's the last element in the array
        if (i == (m_size - 1))
        {
            // Insert element to the end of the array, since the previous element is less than the new value
            m_pArray[i + 1] = val;

            // Get out of the outter loop
            break;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Recursive binary search
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::optional<size_t> OrderedArray<Value>::BinarySearch(const Value& val, size_t start, size_t end)
{
    assert(start <= end);

    // Get mid point
    const size_t midPointIndex = (start + end) / 2;

    // Recursively call this function
    // If mid point index is the value we are looking for, return the index of mid point index
    if (m_pArray[midPointIndex] == val)
        return midPointIndex;

    // If can't find it after log(n) times
    else if (start == end)
        return {};

    // If the searching value is less than current midpoint value, search it again in the less half array
    else if (val < m_pArray[midPointIndex])
        return BinarySearch(val, start, midPointIndex);

    // If the searching value is greater than current midpoint value, search it again in the larger half array
    else
        return BinarySearch(val, midPointIndex + 1, end);
}
