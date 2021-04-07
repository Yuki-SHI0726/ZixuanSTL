#pragma once

#include "LinkedList.h"

//--------------------------------------------------------------------------------------------------------------------
// Stack implemented by Linkedlist
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
class StackList
{
private:
	LinkedList<Value> m_stackList;
	
	// Top is alwasy list's Head

public:
	StackList();

	void Push(const Value& val) { m_stackList.PushFront(val); }
	void Print() const;
	void Clear() { m_stackList.Clear(); }
	Value Pop() { return m_stackList.PopFront(); }
	Value Top() const { return m_stackList.Head()->GetValue(); }
	bool Empty() const { return m_stackList.Empty(); }
	size_t GetSize() const { return m_stackList.GetSize(); }
	
	static void Test();
};

//--------------------------------------------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline StackList<Value>::StackList()
	: m_stackList{}
{
}

//--------------------------------------------------------------------------------------------------------------------
// Print stack, mark where top is
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void StackList<Value>::Print() const
{
	m_stackList.Print("Top");
}

//--------------------------------------------------------------------------------------------------------------------
// Test
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void StackList<Value>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Value value = 0;		// Be used as value

	// I'm tired typing in initial size and elements to test sorting algorithms
	StackList<Value> stackList{  };
#if _DEBUG
	stackList.Push(static_cast<Value>(76));
	stackList.Push(static_cast<Value>(68));

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
		stackList.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kStackList);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter pushing value: ";
			std::cin >> value;
			stackList.Push(value);
			break;

		case '1':
			stackList.Clear();
			break;

		case '2':
			stackList.Pop();
			break;

		case '3':
			std::cout << "Top value is: " << stackList.Top() << std::endl;
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}
