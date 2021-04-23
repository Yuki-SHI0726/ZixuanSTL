#pragma once

#include "LinkedList.h"

//--------------------------------------------------------------------------------------------------------------------
// Queue implemented by Linkedlist
//--------------------------------------------------------------------------------------------------------------------
template<class T>
class QueueList
{
private:
	LinkedList<T> m_queueList;

	// Head is always list's Head
	// Tail is always list's tail

public:
	QueueList();

	void Enqueue(const T& val) { m_queueList.PushBack(val); }
	T Dequeue() { return m_queueList.PopFront(); }
	void Print() const;
	void Clear() { m_queueList.Clear(); }
	T Head() const { return m_queueList.Head()->GetValue(); }
	T Tail() const { return m_queueList.Tail()->GetValue(); }
	bool Empty() const { return m_queueList.Empty(); }
	size_t GetSize() const { return m_queueList.GetSize(); }

	static void Test();
};

//--------------------------------------------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline QueueList<T>::QueueList()
	: m_queueList{}
{
}

//--------------------------------------------------------------------------------------------------------------------
// Print queue, mark where head and tail is
//--------------------------------------------------------------------------------------------------------------------
template<class T>
inline void QueueList<T>::Print() const
{
	m_queueList.Print("Head", "Tail");
}

template<class T>
inline void QueueList<T>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	T value = 0;		// Be used value

	// Create queue
	QueueList<T> queueList{ };
#if _DEBUG
	queueList.Enqueue(static_cast<T>(0));
	queueList.Enqueue(static_cast<T>(1));
	queueList.Enqueue(static_cast<T>(2));
	queueList.Enqueue(static_cast<T>(3));
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
