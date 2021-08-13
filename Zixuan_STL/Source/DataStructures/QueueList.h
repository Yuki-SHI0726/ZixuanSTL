#pragma once

#include "list.h"

//--------------------------------------------------------------------------------------------------------------------
// Queue implemented by Linkedlist
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
class QueueList
{
private:
	list<Type> m_queueList;

	// Head is always list's Head
	// Tail is always list's tail

public:
	QueueList();

	void Enqueue(const Type& val) { m_queueList.PushBack(val); }
	Type Dequeue() { return m_queueList.PopFront(); }
	void Print() const;
	void Clear() { m_queueList.Clear(); }
	Type& Head() { return m_queueList.front(); }
	Type& Tail() { return m_queueList.back(); }
	bool Empty() const { return m_queueList.Empty(); }
	size_t GetSize() const { return m_queueList.GetSize(); }

	static void Test();
};

//--------------------------------------------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline QueueList<Type>::QueueList()
	: m_queueList{}
{
}

//--------------------------------------------------------------------------------------------------------------------
// Print queue, mark where head and tail is
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void QueueList<Type>::Print() const
{
	m_queueList.Print("Head", "Tail");
}

template<class Type>
inline void QueueList<Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Type value = 0;		// Be used value

	// Create queue
	QueueList<Type> queueList{ };
#if _DEBUG
	queueList.Enqueue(static_cast<Type>(0));
	queueList.Enqueue(static_cast<Type>(1));
	queueList.Enqueue(static_cast<Type>(2));
	queueList.Enqueue(static_cast<Type>(3));
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		queueList.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kQueueList);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter enqueue value: ";
			std::cin >> value;
			queueList.Enqueue(value);
			break;

		case '1':
			queueList.Clear();
			break;

		case '2':
			queueList.Dequeue();
			break;

		case '3':
			std::cout << "Head value is: " << queueList.Head() << std::endl;
			system("pause");
			break;

		case '4':
			std::cout << "Tail value is: " << queueList.Tail() << std::endl;
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}
