#pragma once

#include <assert.h>
#include "Utils/StructureManager.h"

//--------------------------------------------------------------------------------------------------------------------
// CircularQueue with a fixed size array.
// Will wrap around by overriding elements which are already at the front of the queue when full 
//--------------------------------------------------------------------------------------------------------------------
template<class T>
class CircularQueue
{
private:
	T* m_pCircularQueue;

	size_t m_capacity; // was m_maxSize  
	size_t m_size;     // was m_numElements

	// Used for tracking head and tail in the array
	size_t m_frontIndex;
	size_t m_rearIndex;

public:
	CircularQueue();
	CircularQueue(size_t capacity);
	~CircularQueue();

	// API
	void Enqueue(const T& val);
	T& Dequeue();
	void Print() const;
	void Clear();
	T& Rear() const;
	T& Front() const;
	bool Empty() const { return m_size <= 0; }
	size_t GetSize() const { return m_size; }
	size_t GetCapacity() const { return m_capacity; }

	// Tests
	static void Test();

private:
	void Destroy();
};

template<class T>
inline CircularQueue<T>::CircularQueue()
	: m_pCircularQueue{ nullptr }
	, m_capacity{ kInitialCapacity }
	, m_size{ 0 }
	, m_frontIndex{ 0 }
	, m_rearIndex{ 0 }
{
	m_pCircularQueue = new T[kInitialCapacity];
}

template<class T>
inline CircularQueue<T>::CircularQueue(size_t capacity)
	: m_pCircularQueue(nullptr)
	, m_capacity(capacity)
	, m_size(0)
	, m_frontIndex{ 0 }
	, m_rearIndex{ 0 }
{
	m_pCircularQueue = new T[m_capacity];
}

template<class T>
inline CircularQueue<T>::~CircularQueue()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Will wrap around by overriding elements which are already at the front of the queue when full 
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void CircularQueue<T>::Enqueue(const T& val)
{
	// We have two condition here
	// 1. The queue is full, we need to overwrite the head, move head and tail one spot forward, and don't increment size
	// 2. The queue is not full, do work normally

	// Check whether queue is full
	// If Queue.head = Queue.tail, and there is some value in there, Queue is full
	if (m_size >= m_capacity)
	{
		// Clean what's already in the tail index
		if constexpr (!std::is_trivially_destructible_v<T>)
			m_pCircularQueue[m_rearIndex].~T();

		// Move head index forward
		++m_frontIndex;

		// Wrap around if front hit max size
		if (m_frontIndex >= kInitialCapacity)
			m_frontIndex = 0;
	}
	// If the queue is not full, increment size
	else
	{
		++m_size;
	}

	// Enqueue the data
	m_pCircularQueue[m_rearIndex] = val;

	// Increment tail index
	++m_rearIndex;

	// Wrap around if tail hit max size
	if (m_rearIndex >= kInitialCapacity)
		m_rearIndex = 0;
}

template<class T>
inline T& CircularQueue<T>::Dequeue()
{
	// Underflow checking
	// If Queue.head = Queue.tail, and there is no value in there, Queue overflow
	assert(m_size > 0);

	// Get return value
	T& pVal = m_pCircularQueue[m_frontIndex];

	// If the element is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<T>)
		m_pCircularQueue[m_frontIndex].~T();

	// Increment head
	++m_frontIndex;

	// Wrap around if head hit max size
	if (m_frontIndex >= kInitialCapacity)
		m_frontIndex = 0;

	// Decrement size
	--m_size;

	return pVal;
}

template<class T>
inline void CircularQueue<T>::Print() const
{
	std::cout << "Queue: { ";

	// Tracking current index to print
	size_t current = m_frontIndex;
	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << m_pCircularQueue[current] << ", ";
		++current;
		if (current >= kInitialCapacity)
			current = 0;
	}

	std::cout << "} " << std::endl;
}

template<class T>
inline void CircularQueue<T>::Clear()
{
	std::memset(m_pCircularQueue, 0, sizeof(T) * m_size);
	m_frontIndex = 0;
	m_rearIndex = 0;
	m_size = 0;
}

template<class T>
inline T& CircularQueue<T>::Rear() const
{
	assert(m_size > 0);
	return m_pCircularQueue[(m_rearIndex == 0) ? (m_capacity - 1) : (m_rearIndex - 1)];
}

template<class T>
inline T& CircularQueue<T>::Front() const
{
	assert(m_size > 0);
	return m_pCircularQueue[m_frontIndex];
}

template<class T>
inline void CircularQueue<T>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	T value = 0;		// Be used value

	// I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
	// Create queue
	CircularQueue<T> circularQueue{ kInitialCapacity };
	circularQueue.Enqueue(static_cast<T>(0));
	circularQueue.Enqueue(static_cast<T>(1));
	circularQueue.Enqueue(static_cast<T>(2));
	circularQueue.Enqueue(static_cast<T>(3));
	circularQueue.Enqueue(static_cast<T>(4));
	circularQueue.Enqueue(static_cast<T>(5));
	circularQueue.Enqueue(static_cast<T>(6));
	circularQueue.Enqueue(static_cast<T>(7));
	circularQueue.Enqueue(static_cast<T>(8));
	circularQueue.Enqueue(static_cast<T>(9));

#else
		// Get capacity from user
	std::cout << "Enter initial capacity: ";
	std::cin >> i;
	system("cls");

	// Create array
	CircularQueue<Value> circularQueue{ i };
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		circularQueue.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kCircularQueue);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter enqueue value: ";
			std::cin >> value;
			circularQueue.Enqueue(value);
			break;

		case '1':
			circularQueue.Clear();
			break;

		case '2':
			circularQueue.Dequeue();
			break;

		case '3':
			std::cout << "Head value is: " << circularQueue.Front() << std::endl;
			system("pause");
			break;

		case '4':
			std::cout << "Tail value is: " << circularQueue.Rear() << std::endl;
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}

template<class T>
inline void CircularQueue<T>::Destroy()
{
	// If the type of elements is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<T>)
	{
		// Tracking current index to destroy
		size_t current = m_frontIndex;
		for (size_t i = 0; i < m_size; ++i)
		{
			m_pCircularQueue[current].~T();
			++current;
			if (current >= kInitialCapacity)
				current = 0;
		}
	}

	// If m_pArray is not nullptr, deallocate it and set it to nullptr
	if (m_pCircularQueue != nullptr)
	{
		delete[] m_pCircularQueue;
		m_pCircularQueue = nullptr;
	}
}

