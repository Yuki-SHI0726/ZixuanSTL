#pragma once

#include "vector.h"


//--------------------------------------------------------------------------------------------------------------------
// Stack implemented by UnorderedArray
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
class StackArray
{
private:
	vector<Type> m_unorderedArray;

public:
	StackArray();
	StackArray(size_t capacity);

	void Push(const Type& val);
	void Print() const;
	void Clear();
	Type Pop();
	Type& Top();
	const Type& Top() const;
	bool Empty() const { return m_unorderedArray.Empty(); }
	size_t GetSize() const { return m_unorderedArray.GetSize(); }
	size_t GetCapacity() const { return m_unorderedArray.GetCapacity(); }

	// Test
	static void Test();
};

template<class Type>
inline StackArray<Type>::StackArray()
	: m_unorderedArray{ kInitialCapacity }
{
}

template<class Type>
inline StackArray<Type>::StackArray(size_t capacity)
	: m_unorderedArray{ capacity }
{
}

template<class Type>
inline void StackArray<Type>::Push(const Type& val)
{
	assert(m_unorderedArray.GetSize() < kInitialCapacity && "Error: Stack Overflow!");
	m_unorderedArray.push_back(val);
}

template<class Type>
inline void StackArray<Type>::Print() const
{
	m_unorderedArray.Print();
}

template<class Type>
inline void StackArray<Type>::Clear()
{
	m_unorderedArray.clear();
}

template<class Type>
inline Type StackArray<Type>::Pop()
{
	assert(!Empty() && "Error: Stack Underflow!");
	return m_unorderedArray.PopBack();
}

template<class Type>
inline Type& StackArray<Type>::Top()
{
	assert(!Empty() && "Error: Stack has no element!");
	return m_unorderedArray[m_unorderedArray.GetSize() - 1];
}

template<class Type>
inline const Type& StackArray<Type>::Top() const
{
	assert(!Empty() && "Error: Stack has no element!");
	return m_unorderedArray[m_unorderedArray.GetSize() - 1];
}

template<class Type>
inline void StackArray<Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Type value = 0;		// Be used as value

	// I'm tired typing in initial size and elements to test sorting algorithms
#if _DEBUG
	// Create stack
	StackArray<Type> stackArray{ kInitialCapacity };
	stackArray.Push(static_cast<Type>(76));
	stackArray.Push(static_cast<Type>(68));

#else
		// Get capacity from user
	std::cout << "Enter initial capacity: ";
	std::cin >> i;
	system("cls");

	// Create array
	StackArray<Type> stackArray{ i };
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

