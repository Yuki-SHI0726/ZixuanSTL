#pragma once

#include <assert.h>
#include <cstring>
#include <optional>

namespace zxstl
{
//--------------------------------------------------------------------------------------------------------------------
// Queue implemented by T array
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
class QueueArray
{
private:
	std::byte* m_pBuffer;

	size_t m_capacity;
	size_t m_size;   

	// Used for tracking head and tail in the array
	size_t m_headIndex;
	size_t m_tailIndex;

public:
	QueueArray(size_t capacity = kInitialCapacity);
	~QueueArray();

	void Enqueue(const Type& val);
	Type Dequeue();
	void Print() const;
	void Clear();
	Type& Tail() const;
	Type& Head() const;
	bool Empty() const { return m_size <= 0; }
	size_t GetSize() const { return m_size; }
	size_t GetCapacity() const { return m_capacity; }

	// Test
	static void Test();

private:
	void Destroy();
};

template<class Type>
inline QueueArray<Type>::QueueArray(size_t capacity /*= kInitialCapacity*/)
	: m_pBuffer(nullptr)
	, m_capacity(capacity)
	, m_size(0)
	, m_headIndex{ 0 }
	, m_tailIndex{ 0 }
{
	m_pBuffer = new std::byte[m_capacity * sizeof(Type)];
}

//--------------------------------------------------------------------------------------------------------------------
// Default ctor
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline QueueArray<Type>::~QueueArray()
{
	Destroy();
}

template<class Type>
inline void QueueArray<Type>::Enqueue(const Type& val)
{
	// Overflow checking
	// If size is equal to the capacity, it's full
	assert(m_size <= m_capacity);

	// Enqueue the data
	Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
	pTypeArray[m_tailIndex] = val;

	// Increment tail index
	++m_tailIndex;

	// Wrap around if tail hit max size
	if (m_tailIndex >= m_capacity)
		m_tailIndex = 0;

	// Increment size
	++m_size;
}

template<class Type>
inline Type QueueArray<Type>::Dequeue()
{
	// Underflow checking
	// If size is less than 0, it's empty
	assert(m_size > 0);

	// Get return value
	Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
	Type pVal = pTypeArray[m_headIndex];

	// If the element is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<Type>)
		pTypeArray[m_headIndex].~Type();

	// Increment head
	++m_headIndex;

	// Wrap around if head hit max size
	if (m_headIndex >= m_capacity)
		m_headIndex = 0;

	// Decrement size
	--m_size;

	return pVal;
}

template<class Type>
inline void QueueArray<Type>::Print() const
{
	std::cout << "Queue: { ";

	// Tracking current index to print
	size_t current = m_headIndex;
	Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << pTypeArray[current] << ", ";
		++current;
		if (current >= m_capacity)
			current = 0;
	}

	std::cout << "} " << std::endl;
}

template<class Type>
inline void QueueArray<Type>::Clear()
{
	Destroy();
	m_headIndex = 0;
	m_tailIndex = 0;
	m_size = 0;
}

template<class Type>
inline Type& QueueArray<Type>::Tail() const
{
	assert(m_size > 0);
	Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
	return pTypeArray[(m_tailIndex == 0) ? (m_capacity - 1) : (m_tailIndex - 1)];
}

template<class Type>
inline Type& QueueArray<Type>::Head() const
{
	assert(m_size > 0);
	Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);
	return pTypeArray[m_headIndex];
}

template<class Type>
inline void QueueArray<Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Type value = 0;		// Be used value

	// I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
	// Create queue
	QueueArray<Type> queueArray{ kInitialCapacity };
	queueArray.Enqueue(static_cast<Type>(13));
	queueArray.Enqueue(static_cast<Type>(19));

#else
		// Get capacity from user
	std::cout << "Enter initial capacity: ";
	std::cin >> i;
	system("cls");

	// Create array
	QueueArray<Type> queueArray{ i };
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
template<class Type>
inline void QueueArray<Type>::Destroy()
{
	// If the type of elements is not trivially destructible, call it's destructor
	if constexpr (!std::is_trivially_destructible_v<Type>)
	{
		Type* pTypeArray = reinterpret_cast<Type*>(m_pBuffer);

		// Tracking current index to destroy
		size_t current = m_headIndex;
		for (size_t i = 0; i < m_size; ++i)
		{
			pTypeArray[current].~Type();
			++current;
			if (current >= m_capacity)
				current = 0;
		}
	}

	delete[] m_pBuffer;
	m_pBuffer = nullptr;
}
}