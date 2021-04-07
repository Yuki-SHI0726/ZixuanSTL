#pragma once

#include <assert.h>
#include <cstring>
#include <optional>

//--------------------------------------------------------------------------------------------------------------------
// Queue implemented by T array
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
class QueueArray
{
private:
	Value* m_pQueue;

	size_t m_capacity; // was m_maxSize  
	size_t m_size;     // was m_numElements

	// Used for tracking head and tail in the array
	size_t m_headIndex;
	size_t m_tailIndex;

public:
	QueueArray();
	QueueArray(size_t capacity);
	~QueueArray();

	void Enqueue(const Value& val);
	Value& Dequeue();
	void Print() const;
	void Clear();
	Value& Tail() const;
	Value& Head() const;
	bool Empty() const { return m_size <= 0; }
	size_t GetSize() const { return m_size; }
	size_t GetCapacity() const { return m_capacity; }

	// Test
	static void Test();

private:
	void Destroy();
};

template<class Value>
inline QueueArray<Value>::QueueArray()
	: m_pQueue(nullptr)
	, m_capacity(kInitialCapacity)
	, m_size(0)
	, m_headIndex{ 0 }
	, m_tailIndex{ 0 }
{
	m_pQueue = new Value[kInitialCapacity];
}

template<class Value>
inline QueueArray<Value>::QueueArray(size_t capacity)
	: m_pQueue(nullptr)
	, m_capacity(capacity)
	, m_size(0)
	, m_headIndex{ 0 }
	, m_tailIndex{ 0 }
{
	m_pQueue = new Value[m_capacity];
}

//--------------------------------------------------------------------------------------------------------------------
// Default ctor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline QueueArray<Value>::~QueueArray()
{
	Destroy();
}

template<class Value>
inline void QueueArray<Value>::Enqueue(const Value& val)
{
	// Overflow checking
	// If size is equal to the capacity, it's full
	assert(m_size <= m_capacity);

	// Enqueue the data
	m_pQueue[m_tailIndex] = val;

	// Increment tail index
	++m_tailIndex;

	// Wrap around if tail hit max size
	if (m_tailIndex >= kInitialCapacity)
		m_tailIndex = 0;

	// Increment size
	++m_size;
}

template<class Value>
inline Value& QueueArray<Value>::Dequeue()
{
	// Underflow checking
	// If size is less than 0, it's empty
	assert(m_size > 0);

	// Get return value
	Value& pVal = m_pQueue[m_headIndex];

	// If the element is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<Value>)
		m_pQueue[m_headIndex].~T();

	// Increment head
	++m_headIndex;

	// Wrap around if head hit max size
	if (m_headIndex >= kInitialCapacity)
		m_headIndex = 0;

	// Decrement size
	--m_size;

	return pVal;
}

template<class Value>
inline void QueueArray<Value>::Print() const
{
	std::cout << "Queue: { ";

	// Tracking current index to print
	size_t current = m_headIndex;
	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << m_pQueue[current] << ", ";
		++current;
		if (current >= kInitialCapacity)
			current = 0;
	}

	std::cout << "} " << std::endl;
}

template<class Value>
inline void QueueArray<Value>::Clear()
{
	std::memset(m_pQueue, 0, sizeof(Value) * m_size);
	m_headIndex = 0;
	m_tailIndex = 0;
	m_size = 0;
}

template<class Value>
inline Value& QueueArray<Value>::Tail() const
{
	assert(m_size > 0);
	return m_pQueue[(m_tailIndex == 0) ? (m_capacity - 1) : (m_tailIndex - 1)];
}

template<class Value>
inline Value& QueueArray<Value>::Head() const
{
	assert(m_size > 0);
	return m_pQueue[m_headIndex];
}

template<class Value>
inline void QueueArray<Value>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Value value = 0;		// Be used value

	// I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
	// Create queue
	QueueArray<Value> queueArray{ kInitialCapacity };
	queueArray.Enqueue(static_cast<Value>(13));
	queueArray.Enqueue(static_cast<Value>(19));

#else
		// Get capacity from user
	std::cout << "Enter initial capacity: ";
	std::cin >> i;
	system("cls");

	// Create array
	QueueArray<Value> queueArray{ i };
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		queueArray.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kQueueArray);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter enqueue value: ";
			std::cin >> value;
			queueArray.Enqueue(value);
			break;

		case '1':
			queueArray.Clear();
			break;

		case '2':
			queueArray.Dequeue();
			break;

		case '3':
			std::cout << "Head value is: " << queueArray.Head() << std::endl;
			system("pause");
			break;

		case '4':
			std::cout << "Tail value is: " << queueArray.Tail() << std::endl;
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Delete pArray and set it to nullptr
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void QueueArray<Value>::Destroy()
{
	// If the type of elements is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<Value>)
	{
		// Tracking current index to destroy
		size_t current = m_headIndex;
		for (size_t i = 0; i < m_size; ++i)
		{
			m_pQueue[current].~T();
			++current;
			if (current >= kInitialCapacity)
				current = 0;
		}
	}

	// If m_pArray is not nullptr, deallocate it and set it to nullptr
	if (m_pQueue != nullptr)
	{
		delete[] m_pQueue;
		m_pQueue = nullptr;
	}
}
