#pragma once

#include "UnorderedArray.h"

//--------------------------------------------------------------------------------------------------------------------
// Stack implemented by UnorderedArray
//--------------------------------------------------------------------------------------------------------------------
template<class T>
class StackArray
{
private:
	UnorderedArray<T> m_unorderedArray;
	// Top in stack is trivial, it's just m_unorderedArray[size-1]

public:
	StackArray();
	StackArray(size_t capacity);

	void Push(const T& val);
	void Print() const;
	void Clear();
	T Pop();
	T& Top();
	const T& Top() const;
	bool Empty() const { return m_unorderedArray.Empty(); }
	size_t GetSize() const { return m_unorderedArray.GetSize(); }
	size_t GetCapacity() const { return m_unorderedArray.GetCapacity(); }

	// Test
	static void Test();
};

template<class T>
inline StackArray<T>::StackArray()
	: m_unorderedArray{ kInitialCapacity }
{
}

template<class T>
inline StackArray<T>::StackArray(size_t capacity)
	: m_unorderedArray{ capacity }
{
}

template<class T>
inline void StackArray<T>::Push(const T& val)
{
	assert(m_unorderedArray.GetSize() < kInitialCapacity && "Error: Stack Overflow!");
	m_unorderedArray.PushBack(val);
}

template<class T>
inline void StackArray<T>::Print() const
{
	m_unorderedArray.Print();
}

template<class T>
inline void StackArray<T>::Clear()
{
	m_unorderedArray.Clear();
}

template<class T>
inline T StackArray<T>::Pop()
{
	assert(!Empty() && "Error: Stack Underflow!");
	return m_unorderedArray.PopBack();
}

template<class T>
inline T& StackArray<T>::Top()
{
	assert(!Empty() && "Error: Stack has no element!");
	return m_unorderedArray[m_unorderedArray.GetSize() - 1];
}

template<class T>
inline const T& StackArray<T>::Top() const
{
	assert(!Empty() && "Error: Stack has no element!");
	return m_unorderedArray[m_unorderedArray.GetSize() - 1];
}

template<class T>
inline void StackArray<T>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	T value = 0;		// Be used as value

	// I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
	// Create stack
	StackArray<T> stackArray{ kInitialCapacity };
	stackArray.Push(static_cast<T>(76));
	stackArray.Push(static_cast<T>(68));

#else
		// Get capacity from user
	std::cout << "Enter initial capacity: ";
	std::cin >> i;
	system("cls");

	// Create array
	StackArray<Value> stackArray{ i };
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		stackArray.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kStackArray);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter pushing value: ";
			std::cin >> value;
			stackArray.Push(value);
			break;

		case '1':
			stackArray.Clear();
			break;

		case '2':
			stackArray.Pop();
			break;

		case '3':
			std::cout << "Top value is: " << stackArray.Top() << std::endl;
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}
