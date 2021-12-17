#pragma once

#include "list.h"

namespace zxstl
{
//--------------------------------------------------------------------------------------------------------------------
// Stack implemented by Linkedlist
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
class StackList
{
private:
	list<Type> m_stackList;
	
	// Top is alwasy list's Head

public:
	StackList();

	void Push(const Type& val) { m_stackList.PushFront(val); }
	void Print() const;
	void Clear() { m_stackList.Clear(); }
	Type Pop() { return m_stackList.PopFront(); }
	Type& Top() { return m_stackList.front(); }
	bool Empty() const { return m_stackList.Empty(); }
	size_t GetSize() const { return m_stackList.GetSize(); }
	
	static void Test();
};

//--------------------------------------------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline StackList<Type>::StackList()
	: m_stackList{}
{
}

//--------------------------------------------------------------------------------------------------------------------
// Print stack, mark where top is
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void StackList<Type>::Print() const
{
	m_stackList.Print("Top");
}

//--------------------------------------------------------------------------------------------------------------------
// Test
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void StackList<Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Type value = 0;		// Be used as value

	// I'm tired typing in initial size and elements to test sorting algorithms
	StackList<Type> stackList{  };
#if _DEBUG
	stackList.Push(static_cast<Type>(76));
	stackList.Push(static_cast<Type>(68));

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

}