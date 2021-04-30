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
template<class T>
class OrderedArray
{
private:
    std::byte* m_pBuffer;

    size_t m_capacity;
    size_t m_size;     
    bool m_isIncreasingOrder;   // Used for ordering decreasing or increasing

public:
    OrderedArray(size_t capacity, bool isIncreasingOrder = true);
    OrderedArray(bool isIncreasingOrder = true);
    ~OrderedArray();

    // API
    void Clear();
    void Push(const T& val);
    void Push(T&& val);
    template <class... Args> void Emplace(Args&&... args);
    T Pop();
    void Erase(size_t index);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    void Print() const;
    void Reverse();
    std::optional<size_t> Search(const T& val) const;

    // Getters
    size_t GetSize() const { return m_size; }
    size_t GetCapacity() const { return m_capacity; }
    bool Empty() const { return m_size <= 0; }

    // Test
    static void Test();

private:
    void Expand(size_t newCapacity);
    void Destroy();
    void InternalPush(const T& val);
    template <class... Args> void InternalEmplace(Args&&... args);
    std::optional<size_t> BinarySearch(const T& val, size_t start, size_t end) const;
};

//--------------------------------------------------------------------------------------------------------------------
// Ctor, takes in the size of the array and dynamically allocate in the ctor, takes in a boolean of ordering method
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline OrderedArray<T>::OrderedArray(size_t capacity, bool isIncreasingOrder)
    : m_pBuffer(nullptr)
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
template<class T>
inline OrderedArray<T>::OrderedArray(bool isIncreasingOrder)
    : m_pBuffer(nullptr)
    , m_capacity(kInitialCapacity)
    , m_size(0)
    , m_isIncreasingOrder{ isIncreasingOrder }
{
    Expand(m_capacity);
}

//--------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline OrderedArray<T>::~OrderedArray()
{
    Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Removes all elements from the array, set size back to 0
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::Clear()
{
    Destroy();
    m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::Push(const T& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // For adding the first element to the array
    if (m_size == 0)
        new(m_pBuffer) T(val);  // std::byte* m_pBuffer
    // If it's not the first element in the array, find then add element to correct location
    else
        InternalPush(val);

    // Increase size
    ++m_size;
}

template<class T>
inline void OrderedArray<T>::Push(T&& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // For adding the first element to the array
    if (m_size == 0)
        new(m_pBuffer) T(val);  // std::byte* m_pBuffer
    // If it's not the first element in the array, find then add element to correct location
    else
        InternalPush(std::forward<T>(val));

    // Increase size
    ++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Removes the last element of the array.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline T OrderedArray<T>::Pop()
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
// Takes an index of the element that should be removed, and removes it from the array.
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::Erase(size_t index)
{
    assert(index >= 0 && index <= (m_size - 1) && m_size > 0);

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
inline T& OrderedArray<T>::operator[](size_t index)
{
    assert(index >= 0 && index < m_size && !Empty());
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    return pTypeArray[index];
}

template<class T>
inline const T& OrderedArray<T>::operator[](size_t index) const
{
    assert(index >= 0 && index < m_size && !Empty());
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    return pTypeArray[index];
}

//--------------------------------------------------------------------------------------------------------------------
// Print out every element
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::Print() const
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    std::cout << "Elements: { ";
    for (size_t i = 0; i < m_size; ++i)
        std::cout << pTypeArray[i] << ", ";
    std::cout << "} " << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Reverse the ordering method in array
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::Reverse()
{
    // Update order boolean
    m_isIncreasingOrder = !m_isIncreasingOrder;

    // Reverse the whole array
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
    for (size_t i = 0; i < m_size / 2; ++i)
        std::swap(pTypeArray[i], pTypeArray[m_size - i - 1]);
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
template<class T>
inline std::optional<size_t> OrderedArray<T>::Search(const T& val) const
{
    return BinarySearch(val, 0, m_size);
}

template<class T>
inline void OrderedArray<T>::Test()
{
    // Variables for testing
    bool shouldQuit = false;
    T value = 0;		// Be used as capacity, value, and index
    size_t i = 0;       // Used as capacity and index
    bool isIncreasingOrder = true;
    char operationInput = ' ';

    // I'm tired typing in initial size and elements to test stuff
#if _DEBUG
    // Create array
    OrderedArray<T> orderedArray{ static_cast<size_t>(12) };
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));
    orderedArray.Push(static_cast<T>(rand() % 100));

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
            orderedArray.Erase(value);
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
template<class T>
inline void OrderedArray<T>::Expand(size_t newCapacity)
{
    const size_t newBufferSize = newCapacity * sizeof(T);

    std::byte* pNewBuffer = new std::byte[newBufferSize];

    // if the current buffer has data, copy it over to our new buffer and deallocate
    if (m_capacity > 0)
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
inline void OrderedArray<T>::Destroy()
{
    // If the type of elements is not trivially destructible, call it's destructor
    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
        {
            pTypeArray[i].~T();
        }
    }

    // If m_pArray is not nullptr, deallocate it and set it to nullptr
    if (m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = nullptr;
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Push a new element into the current location
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void OrderedArray<T>::InternalPush(const T& val)
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Walk through the array
    for (size_t i = 0; i < m_size; ++i)
    {
        // If current element is greater than the inserting value
        if (m_isIncreasingOrder ? (pTypeArray[i] > val) : (pTypeArray[i] < val))
        {
            // Move every elements one spot backward
            for (size_t j = m_size; j > i; --j)
                pTypeArray[j] = pTypeArray[j - 1];

            // Update current spot to value
            new(m_pBuffer + (i * sizeof(T))) T(val);

            // Get out of the outter loop
            break;
        }
        // if it's the last element in the array
        if (i == (m_size - 1))
        {
            // Insert element to the end of the array, since the previous element is less than the new value
            new(m_pBuffer + ((i + 1) * sizeof(T))) T(val);

            // Get out of the outter loop
            break;
        }
    }
}

template<class T>
template <class... Args>
inline void OrderedArray<T>::InternalEmplace(Args&&... args)
{
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Walk through the array
    for (size_t i = 0; i < m_size; ++i)
    {
        // If current element is greater than the inserting value
        if (m_isIncreasingOrder ? (pTypeArray[i] > T(std::forward<Args>(args)...)) : (pTypeArray[i] < T(std::forward<Args>(args)...)))
        {
            // Move every elements one spot backward
            for (size_t j = m_size; j > i; --j)
                pTypeArray[j] = pTypeArray[j - 1];

            // Update current spot to value
            new(m_pBuffer + (i * sizeof(T))) T(std::forward<Args>(args)...);

            // Get out of the outter loop
            break;
        }
        // if it's the last element in the array
        if (i == (m_size - 1))
        {
            // Insert element to the end of the array, since the previous element is less than the new value
            new(m_pBuffer + ((i + 1) * sizeof(T))) T(std::forward<Args>(args)...);

            // Get out of the outter loop
            break;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Recursive binary search
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline std::optional<size_t> OrderedArray<T>::BinarySearch(const T& val, size_t start, size_t end) const
{
    assert(start <= end);
    T* pTypeArray = reinterpret_cast<T*>(m_pBuffer);

    // Get mid point
    const size_t midPointIndex = (start + end) / 2;

    // Recursively call this function
    // If mid point index is the value we are looking for, return the index of mid point index
    if (pTypeArray[midPointIndex] == val)
        return midPointIndex;

    // If can't find it after log(n) times
    else if (start == end)
        return {};

    // If the searching value is less than current midpoint value, search it again in the less half array
    else if (val < pTypeArray[midPointIndex])
        return BinarySearch(val, start, midPointIndex);

    // If the searching value is greater than current midpoint value, search it again in the larger half array
    else
        return BinarySearch(val, midPointIndex + 1, end);
}

template<class T>
template<class ...Args>
inline void OrderedArray<T>::Emplace(Args && ...args)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        Expand(m_capacity * kExpandMultiplier);

    // For adding the first element to the array
    if (m_size == 0)
        new(m_pBuffer) T(std::forward<Args>(args)...);  // std::byte* m_pBuffer
    // If it's not the first element in the array, find then add element to correct location
    else
        InternalEmplace(std::forward<Args>(args)...);

    ++m_size;
}
