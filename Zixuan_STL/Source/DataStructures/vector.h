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

template<typename Vector>
class vector_iterator
{
public:
    using ValueType = typename Vector::ValueType;

private:
    ValueType* m_ptr;

public:
    // Member functions
    constexpr vector_iterator(ValueType* ptr);

    // Meant to be virtual
    vector_iterator& operator++();
    vector_iterator operator++(int);
    vector_iterator& operator--();
    vector_iterator operator--(int);

    ValueType& operator[](size_t index) { return *(m_ptr + index); }
    ValueType* operator->() { return m_ptr; }
    ValueType& operator*() { return *m_ptr; }
    bool operator==(const vector_iterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const vector_iterator& other) const { return m_ptr != other.m_ptr; }
};

template<typename Vector>
inline constexpr vector_iterator<Vector>::vector_iterator(ValueType* ptr)
    : m_ptr{ ptr }
{
}

template<typename Vector>
inline vector_iterator<Vector>& vector_iterator<Vector>::operator++()
{
    m_ptr++;
    return *this;
}

template<typename Vector>
inline vector_iterator<Vector> vector_iterator<Vector>::operator++(int)
{
    vector_iterator iterator = *this;
    ++(*this);
    return iterator;
}

template<typename Vector>
inline vector_iterator<Vector>& vector_iterator<Vector>::operator--()
{
    m_ptr--;
    return *this;
}

template<typename Vector>
inline vector_iterator<Vector> vector_iterator<Vector>::operator--(int)
{
    vector_iterator iterator = *this;
    --(*this);
    return iterator;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unorded array class
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
class vector
{
public:
    using ValueType = Type;
    using iterator = vector_iterator<vector<Type>>;

private:
    std::byte* m_pBuffer;
    size_t m_capacity; 
    size_t m_size;    

public:
    // Member functions
    constexpr vector(size_t capacity = kInitialCapacity);
    constexpr vector(const vector& other);
    constexpr vector(vector&& other) noexcept;
    constexpr vector& operator=(const vector& other);
    constexpr vector& operator=(vector&& other) noexcept;
    constexpr ~vector();

    // Element access
    constexpr Type& at(size_t index);
    constexpr const Type& at(size_t index) const;
    constexpr Type& operator[](size_t index);
    constexpr const Type& operator[](size_t index) const;
    constexpr Type& front();
    constexpr const Type& front() const;
    constexpr Type& back();
    constexpr const Type& back() const;
    constexpr Type* data() noexcept { return reinterpret_cast<Type*>(m_pBuffer); }
    constexpr const Type* data() const noexcept { return reinterpret_cast<Type*>(m_pBuffer); }
    
    // Iterators
    constexpr iterator begin() noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer)); }
    constexpr const iterator begin() const noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer)); }
    constexpr const iterator cbegin() const noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer)); }
    constexpr iterator end() noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer) + m_size); }
    constexpr const iterator end() const noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer) + m_size); }
    constexpr const iterator cend() const noexcept { return iterator(reinterpret_cast<Type*>(m_pBuffer) + m_size); }

    // Capacity
    constexpr bool empty() const noexcept { return m_size <= 0; }
    constexpr size_t size() const noexcept { return m_size; }
    constexpr size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }
    constexpr void reserve(size_t newCapacity);
    constexpr size_t capacity() const noexcept { return m_capacity; }
    constexpr void shrink_to_fit();

    // Modifiers
    constexpr void clear() noexcept;
    constexpr void insert(size_t index, const Type& val);
    constexpr void insert(size_t index, Type&& val);
    template <class... Args> constexpr void emplace(size_t index, Args&&... args);
    constexpr void erase(size_t index);
    constexpr void push_back(const Type& val);
    constexpr void push_back(Type&& val);
    constexpr void push_front(const Type& val);
    constexpr void push_front(Type&& val);
    template <class... Args> constexpr void emplace_back(Args&&... args);
    template <class... Args> constexpr void emplace_front(Args&&... args);
    constexpr void pop_back();
    constexpr void resize(size_t count);
    constexpr void swap(vector& other) noexcept { Swap(*this, other); }

    // Non-member functions
    

    // Additional stuff
    void Print(bool horizontal = true) const;
    std::optional<size_t> Search(const Type& val) const;
    void BubbleSort();
    void SelectionSort();
    void InsertionSort();
    void QuickSort();
    void Shuffle();

    // Testings
    static void InteractiveTest();
    static bool UnitTest();

private:
    void _update_buffer_with_new_capacity(size_t newCapacity);
    void _destroy();
    void _internal_quicksort(size_t start, size_t end);
    size_t _partition(size_t start, size_t end);

#if PIVOT_PICK == 1
    size_t _get_mid_index(size_t start, size_t end) const;
#endif
};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Ctor, takes in the size of the array and dynamically allocate in the ctor
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>::vector(size_t capacity /*= kInitialCapacity*/)
    : m_pBuffer(nullptr)
    , m_capacity(capacity)
    , m_size(0)
{
    assert(m_capacity >= 0);
    _update_buffer_with_new_capacity(m_capacity);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy ctor
// Time:  O(n)
// Space: O(n)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>::vector(const vector& other)
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move ctor
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>::vector(vector&& other) noexcept
    : m_pBuffer(other.m_pBuffer)
    , m_capacity(other.m_capacity)
    , m_size(other.m_size)
{
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_pBuffer = nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move ctor
// Time:  O(n)
// Space: O(n)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>& vector<Type>::operator=(const vector& other)
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
        m_pBuffer = new std::byte[m_capacity * sizeof(Type)];

        // Copy everything over
        Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
        Type* pOtherTypeArray = reinterpret_cast<Type*>(other.m_pBuffer);
        for (size_t i = 0; i < m_size; ++i)
            new(pTypeArray + i) Type(pOtherTypeArray[i]);
    }

    return (*this);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move ctor
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>& vector<Type>::operator=(vector&& other) noexcept
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// The destructor will need to clean up and deallocate any memory that was allocated in the constructor.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr vector<Type>::~vector()
{
    _destroy();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a reference to the element at specified index, with bounds checking.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr Type& vector<Type>::at(size_t index)
{
    assert(index <= m_size && !empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a const reference to the element at specified location pos, with bounds checking.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr const Type& vector<Type>::at(size_t index) const
{
    assert(index <= m_size && !empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a reference to the element at specified location pos. No bounds checking is performed.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr const Type& vector<Type>::operator[](size_t index) const
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a const reference to the element at specified location pos.No bounds checking is performed.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr Type& vector<Type>::operator[](size_t index)
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[index];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a reference to the first element in the container. Calling front on an empty container is undefined.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr Type& vector<Type>::front()
{
    assert(!empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[0];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a const reference to the first element in the container. Calling front on an empty container is undefined.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr const Type& vector<Type>::front() const
{
    assert(!empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[0];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a reference to the last element in the container. Calling back on an empty container causes undefined behavior.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr Type& vector<Type>::back()
{
    assert(!empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[m_size - 1];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a reference to the last element in the container. Calling back on an empty container causes undefined behavior.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr const Type& vector<Type>::back() const
{
    assert(!empty());
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    return pTypeArray[m_size - 1];
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Increase the capacity of the vector to a value that's greater or equal to newCapacity. 
// If newCapacity is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void vector<Type>::reserve(size_t newCapacity)
{
    if (newCapacity > m_capacity)
        _update_buffer_with_new_capacity(newCapacity);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Requests the removal of unused capacity.
// If reallocation occurs, all iterators, including the past the end iterator, and all references to the elements are invalidated. If no reallocation takes place, no iterators or references are invalidated.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void vector<Type>::shrink_to_fit()
{
    _update_buffer_with_new_capacity(m_size);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Erases all elements from the container. After this call, size() returns zero.
// Time:  O(n), n = size()
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::clear() noexcept
{
    _destroy();
    m_size = 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Inserts elements at the specified location in the container.
// Time:  O(n), n = distance(index, m_size)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void vector<Type>::insert(size_t index, const Type& val)
{
    assert(index <= m_size);

    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index + 1, pTypeArray + index, (m_size - index) * sizeof(Type));

    new(m_pBuffer + (index * sizeof(Type))) Type(val);
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Inserts elements at the specified location in the container.
// Time:  O(n), n = distance(index, m_size)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void vector<Type>::insert(size_t index, Type&& val)
{
    assert(index <= m_size);

    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index + 1, pTypeArray + index, (m_size - index) * sizeof(Type));

    new(m_pBuffer + (index * sizeof(Type))) Type(val);
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Inserts a new element into the container directly before pos.
// Time:  O(n), n = distance(index, m_size)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class ...Args>
inline constexpr void vector<Type>::emplace(size_t index, Args&&... args)
{
    assert(index <= m_size);

    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index + 1, pTypeArray + index, (m_size - index) * sizeof(Type));

    new(m_pBuffer + (index * sizeof(Type))) Type(std::forward<Args>(args)...);
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Erases the specified elements from the container.
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::erase(size_t index)
{
    assert(index < m_size && !empty());

    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    if constexpr (!std::is_trivially_destructible_v<Type>)
        pTypeArray[index].~Type();

    // Move every element after the indexed element one spot forward.
    std::memmove(pTypeArray + index, pTypeArray + index + 1, (m_size - index) * sizeof(Type));

    --m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
// Time: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::push_back(const Type& val)
{
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    new(m_pBuffer + (m_size * sizeof(Type))) Type(val); 
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the end of the array
// Time: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::push_back(Type&& val)
{
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    new(m_pBuffer + (m_size * sizeof(Type))) Type(val); 
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::push_front(const Type& val)
{
    // If the array is full, expand it
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    // Create the new element at front
    new(m_pBuffer) Type(val);

    // Increment size
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Takes in a value to be inserted at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::push_front(Type&& val)
{    
    // If the array is full, expand it
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity * kExpandMultiplier);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    // Create the new element at front
    new(m_pBuffer) Type(val);

    // Increment size
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Emplace an object at the end of the array
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class ...Args>
constexpr void vector<Type>::emplace_back(Args&& ...args)
{
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity > 0 ? static_cast<size_t>(m_capacity * kExpandMultiplier) : kInitialCapacity);

    new(m_pBuffer + (m_size * sizeof(Type))) Type(std::forward<Args>(args)...);
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Emplace an object at the begin of the array
// Note: This is an O(n) time operation. If you don't care about how things order in this array, use EmplaceBack & Swap instead
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class ...Args>
constexpr void vector<Type>::emplace_front(Args&& ...args)
{
    if (m_size >= m_capacity)
        _update_buffer_with_new_capacity(m_capacity > 0 ? static_cast<size_t>(m_capacity * kExpandMultiplier) : kInitialCapacity);

    // Shift each element one spot towards to the end in order to create a spot for the new inserted value.
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    std::memmove(pTypeArray + 1, pTypeArray, m_size * sizeof(Type));

    new(m_pBuffer) Type(std::forward<Args>(args)...);
    ++m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Removes the last element of the container. Calling pop_back on an empty container results in undefined behavior.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void vector<Type>::pop_back()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    if constexpr (!std::is_trivially_destructible_v<Type>)
        pTypeArray[m_size - 1].~Type();

    --m_size;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Resizes the container to contain count elements.
// If the current size is greater than count, the container is reduced to its first count elements.
// If the current size is less than count, additional default-inserted elements are appended
// Time:  O(n), Linear in the difference between the current size and count. Additional complexity possible due to reallocation if capacity is less than count
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void vector<Type>::resize(size_t count)
{
    // Size is greater than count, we need to delete things from the buffer
    if (m_size > count)
    {
        // If the type of elements is not trivially destructible, call it's destructor
        if constexpr (!std::is_trivially_destructible_v<Type>)
        {
            Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
            for (size_t i = count - 1; i < m_size; ++i)
                pTypeArray[i].~Type();
        }

        _update_buffer_with_new_capacity(count);
    }
    // Size is less than count, add default values to the end of the buffer
    else
    {
        size_t loopTime = count - m_size;
        for (size_t i = 0; i < loopTime; ++i)
            emplace_back();
    }

    m_size = count;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Print out every element
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::Print(bool horizontal /*= true*/) const
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Linear search
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline std::optional<size_t> vector<Type>::Search(const Type& val) const
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

    for (size_t i = 0; i < m_size; ++i)
    {
        if (pTypeArray[i] == val)
            return i;
    }
    return {};
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bubble Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::BubbleSort()
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Selection Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::SelectionSort()
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Insertion Sort implementation
// Time:  O(n ^ 2)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::InsertionSort()
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Public Quick-sort interface
// Time:  O(nlog(n))
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::QuickSort()
{
    _internal_quicksort(0, m_size - 1);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Quick Sort implementation
// Time:  O(nlog(n))
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::_internal_quicksort(size_t start, size_t end)
{
    if (start < end && end != std::numeric_limits<size_t>::max())
    {
        size_t pivotIndex = _partition(start, end);
        _internal_quicksort(start, pivotIndex - 1);
        _internal_quicksort(pivotIndex + 1, end);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Randomly rearranges elements in the array
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::Shuffle()
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
    for (size_t i = 0; i < m_size; ++i)
        Swap(pTypeArray[i], pTypeArray[rand() % m_size]);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unit tests
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::InteractiveTest()
{
    // Variables for testing
    bool shouldQuit = false;
    size_t i = 0;       // Used as capacity and index
    Type value = 0;		// Be used as value

    // I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
    // Create array
    vector<Type> unorderedArray{ static_cast<size_t>(12) };
    unorderedArray.push_back(static_cast<Type>(13));
    unorderedArray.push_back(static_cast<Type>(19));
    unorderedArray.push_back(static_cast<Type>(9));
    unorderedArray.push_back(static_cast<Type>(5));
    unorderedArray.push_back(static_cast<Type>(12));
    unorderedArray.push_back(static_cast<Type>(8));
    unorderedArray.push_back(static_cast<Type>(7));
    unorderedArray.push_back(static_cast<Type>(4));
    unorderedArray.push_back(static_cast<Type>(21));
    unorderedArray.push_back(static_cast<Type>(2));
    unorderedArray.push_back(static_cast<Type>(6));
    unorderedArray.push_back(static_cast<Type>(11));

#else
    // Get capacity from user
    std::cout << "Enter initial capacity: ";
    std::cin >> i;
    system("cls");

    // Create array
    UnorderedArray<Type> unorderedArray{ (size_t)i };
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
            unorderedArray.push_back(value);
            break;

        case '1':
            unorderedArray.clear();
            break;

        case '2':
            unorderedArray.pop_back();
            break;

        case '3':
            std::cout << "Enter removing index: ";
            std::cin >> i;
            unorderedArray.erase(i);
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unit Test for vector
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline bool vector<Type>::UnitTest()
{
    //---------------------------------------------------------------
    // Prepare vector data. Having a vector with kTestSize int in order
    //---------------------------------------------------------------
    vector<Type> testVec;
    for (size_t i = 0; i < kTestSize; ++i)
    {
        testVec.emplace_back(i);

        // front
        if (testVec.front() != i)
            RETURN_ERROR("vector::front()");

        // back
        if (testVec.back() != i)
            RETURN_ERROR("vector::back()");
    }

    for (size_t i = 0; i < kTestSize; ++i)
    {
        //---------------------------------------------------------------
        // Element access
        //---------------------------------------------------------------
        // at(index)
        if (testVec.at(i) != i)
            RETURN_ERROR("vector::at(index)");

        // operator[] 
        if (testVec[i] != i)
            RETURN_ERROR("vector::operator[]");
    }

    //---------------------------------------------------------------
    // Capacity
    //---------------------------------------------------------------
    testVec.shrink_to_fit();
    if (testVec.capacity() != testVec.size())
        RETURN_ERROR("vector::shrink_to_fit");

    //---------------------------------------------------------------
    // Success
    //---------------------------------------------------------------
    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Create a bigger array, update capacity, copy and move elements from previous array to the new one
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::_update_buffer_with_new_capacity(size_t newCapacity)
{
    const size_t newBufferSize = newCapacity * sizeof(Type);
    std::byte* pNewBuffer = new std::byte[newBufferSize];

    // Is trivially copy able
    // if the current buffer has data, copy it over to our new buffer and deallocate
    std::memcpy(pNewBuffer, m_pBuffer, sizeof(Type) * m_size);
    delete[] m_pBuffer;

    // point to the new buffer and set new capacity
    m_pBuffer = pNewBuffer;
    m_capacity = newCapacity;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Delete pArray and set it to nullptr
// Time:  O(n)
// Space: O(1)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void vector<Type>::_destroy()
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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// _partition algorithm for QuickSort
// The array is divided into four regions.  They are:
//		1) <= pivot, from 0 to i
//		2) > pivot, from i to j 
//		3) Unrestricted / unknown, from j to pivot
//		4) pivot
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline size_t vector<Type>::_partition(size_t start, size_t end)
{
    Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

#if (PIVOT_PICK == 0)    // Randomized element
    // choose a random index from the array 
    // replace the element at that index with the element at the last index of the array.
    Swap(pTypeArray[start + (rand() % (end - start))], pTypeArray[end]);

#elif (PIVOT_PICK == 1)    // Median of Three
    // examines the first, middle, and last elements of the array, find the middle's index
    // the value that is in the middle gets swapped with the last element
    Swap(pTypeArray[_get_mid_index(start, end)], pTypeArray[end]);
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

    // return the pivot, which becomes the beginning and end points of the next calls to _partition().
    return pivotIndex;
}

#if PIVOT_PICK == 1
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Helper function for Median of three QuickSort
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline size_t vector<Type>::_get_mid_index(size_t start, size_t end) const
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
