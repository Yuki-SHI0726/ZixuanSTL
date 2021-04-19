#pragma once

#include <iostream>
#include <assert.h>
#include <utility>

// 0 = singly linkedlist
// 1 = doubly linkedlist
#define DOUBLY_LINKED 0

//--------------------------------------------------------------------------------------------------------------------
// LinkedList class
//--------------------------------------------------------------------------------------------------------------------
template<class Value> 
class LinkedList
{
private:
	struct Node
	{
		Node* m_pNext;

#if DOUBLY_LINKED
		Node* m_pPrev;
#endif
		// The value that this node holds
		Value m_value;

		Node(Value value)
			: m_pNext{ nullptr }
#if DOUBLY_LINKED
			, m_pPrev{ nullptr }
#endif
			, m_value{ value }
		{}

		void ClearPointers()
		{
			m_pNext = nullptr;
#if DOUBLY_LINKED
			m_pPrev = nullptr;
#endif
		}

		Value GetValue() const { return m_value; }
	};

	Node* m_pHead;
	Node* m_pTail;	// A slight hack for making inserting O(n/2) in worst case
	size_t m_size;
	
public:
	LinkedList();
	~LinkedList();

	void PushFront(const Value& val);
	void PushBack(const Value& val);
	void Insert(size_t index, const Value& val);
	void DeleteByValue(const Value& val);
	void DeleteByNode(Node* pNode);
	void DeleteByIndex(size_t index);
	void Print(const char* pPrefix = "", const char* pSuffix = "") const;
	void Clear();
	Value PopFront();
	Value PopBack();
	
	// API
	bool Empty() const { return m_size <= 0; }
	size_t GetSize() const { return m_size; }
	Node* Head() const { return m_pHead; }
	Value HeadValue() const { return m_pHead->GetValue(); }
	Node* Tail() const { return m_pTail; }
	Value TailValue() const { return m_pTail->GetValue(); }
	void Reverse(size_t begin = 0, size_t end = std::numeric_limits<size_t>::max());	
	void Sort();

	// Tests
	static void Test();
	
private:
	Node* SearchNodeByValue(const Value& val);
	Node* SearchNodeByNode(const Node* pNode);
	Node* SearchNodeByIndex(size_t index);
	std::pair<Node*, Node*> SearchPairByValue(const Value& val);	// Return the searched node and it's previous
	std::pair<Node*, Node*> SearchPairByNode(const Node* pNode);	// Return the searched node and it's previous
	std::pair<Node*, Node*> SearchPairByIndex(size_t index);		// Return the searched node and it's previous

	void Destroy();
	void SetNullptrIfEmpty();
	void Delete(Node* pNodeToDelete, Node* pPrevious);
	void DeleteNode(Node* pNodeToDelete);
	void AssignNodeAtIndex(size_t index, Node* pNode);
};

//--------------------------------------------------------------------------------------------------------------------
// Default ctor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline LinkedList<Value>::LinkedList()
	: m_pHead{ nullptr }
	, m_pTail{ nullptr }
	, m_size{ 0 }
{
}

//--------------------------------------------------------------------------------------------------------------------
// Default dtor
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline LinkedList<Value>::~LinkedList()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// Insert val to front
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::PushFront(const Value& val)
{
	// Create new node for pushing
	Node* pNewNode = new Node(val);

	// Set this new node's next to the head
	pNewNode->m_pNext = m_pHead;

	// If doubly linked, set head's prev to new node when head is not empty
#if DOUBLY_LINKED
	if (m_pHead)
		m_pHead->m_pPrev = pNewNode;
#endif

	// Set tail if it's the first element pushed into list
	if (Empty())
		m_pTail = pNewNode;

	// Now, new node is the head
	m_pHead = pNewNode;

	// Increment size
	++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Insert val to back
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::PushBack(const Value& val)
{
	// Create new node
	Node* pNewNode = new Node(val);

	// Set this new node's previous to tail if doubly linked
#if DOUBLY_LINKED
	pNewNode->m_pPrev = m_pTail;
#endif

	// Link this new node to tail's next
	if (m_pTail)
		m_pTail->m_pNext = pNewNode;

	// If empty, head is new node too
	if (Empty())
		m_pHead = pNewNode;

	// Now, new node is the tail
	m_pTail = pNewNode;

	// Increment size
	++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Insert val to the index
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Insert(size_t index, const Value& val)
{
	// Error checking
	assert(index <= m_size);

	// Create new node
	Node* pNewNode = new Node(val);

	// Find the previous
	Node* pPrevious = nullptr;

	// Find the place to insert
	Node* pNext = m_pHead;
	for (size_t i = 0; i < index; ++i)
	{
		pPrevious = pNext;
		pNext = pNext->m_pNext;
	}

	// Set the previous node's next to the new node
	if (pPrevious)
	{
		pPrevious->m_pNext = pNewNode;
#if DOUBLY_LINKED
		pNewNode->m_pPrev = pPrevious;
#endif
	}

	// If inserting to a empty list, set head and tail
	if (m_size == 0)
	{
		m_pHead = pNewNode;
		m_pTail = pNewNode;
	}
	// If index is 0, also set new node as head
	else if (index == 0)
	{
		m_pHead = pNewNode;
	}

	// If it's not the end of the list
	if (pNext)
	{
#if DOUBLY_LINKED
		pNext->m_pPrev = pNewNode;
#endif
		pNewNode->m_pNext = pNext;
	}

	// Increment size
	++m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Find the value and delete it
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::DeleteByValue(const Value& val)
{
	// Underflow checking
	assert(m_size > 0);

	// Get data
#if DOUBLY_LINKED
	Node* pNodeToDelete = SearchNodeByValue(val);
	Delete(pNodeToDelete, nullptr);
#else
	std::pair<Node*, Node*> pNodes = SearchPairByValue(val);	// The node to delete and it's previous
	Node* pPrevious = pNodes.first;
	Node* pNodeToDelete = pNodes.second;
	Delete(pNodeToDelete, pPrevious);
#endif
}

//--------------------------------------------------------------------------------------------------------------------
// Find the node and delete it
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::DeleteByNode(Node* pNode)
{
	// Underflow checking
	assert(m_size > 0);

	// Get data
#if DOUBLY_LINKED
	Node* pNodeToDelete = SearchNodeByNode(pNode);
	Delete(pNodeToDelete, nullptr);
#else
	std::pair<Node*, Node*> pNodes = SearchPairByNode(pNode);	// The node to delete and it's previous
	Node* pPrevious = pNodes.first;
	Node* pNodeToDelete = pNodes.second;
	Delete(pNodeToDelete, pPrevious);
#endif
}

//--------------------------------------------------------------------------------------------------------------------
// Find index and delete it
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::DeleteByIndex(size_t index)
{
	// Underflow checking
	assert(m_size > 0);

	// Get data
#if DOUBLY_LINKED
	Node* pNodeToDelete = SearchNodeByIndex(index);
	Delete(pNodeToDelete, nullptr);
#else
	std::pair<Node*, Node*> pNodes = SearchPairByIndex(index);	// The node to delete and it's previous
	Node* pPrevious = pNodes.first;
	Node* pNodeToDelete = pNodes.second;
	Delete(pNodeToDelete, pPrevious);
#endif
}

//--------------------------------------------------------------------------------------------------------------------
// Reverse the list of given begin and end, default to reverse the whole list
// Time:  O(n)
// Space: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Reverse(size_t begin/* = 0*/, size_t end/*= std::numeric_limits<size_t>::max()*/)
{
	assert(begin <= end);

	// Data for relinking after reversing
	Node* pStart = SearchNodeByIndex(begin);
	Node* pStartPrev = nullptr;
	if (begin > 0)
		pStartPrev = SearchNodeByIndex(begin - 1);

	Node* pEnd = nullptr;
	if (end == std::numeric_limits<size_t>::max())
	{
		end = m_size - 1;
		pEnd = m_pTail;
	}
	else
	{
		assert(end < m_size);
		pEnd = SearchNodeByIndex(end);
	}
	Node* pEndNext = nullptr;
	if (pEnd)
		pEndNext = pEnd->m_pNext;

	// Data for reversing
	Node* pCurrent = pStart;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
	if (begin != 0)
		pPrev = SearchNodeByIndex(begin - 1);

	// Walk through the list
	while (pCurrent)
	{
		// Store next
		pNext = pCurrent->m_pNext;

		// Relink current node's prev and next
		pCurrent->m_pNext = pPrev;
#if DOUBLY_LINKED
		pCurrent->m_pPrev = pNext;
#endif

		// Early quit if we reached the end
		if (pCurrent == pEnd)
			break;

		// Move forward
		pPrev = pCurrent;
		pCurrent = pNext;
	}

	// Relink head and tail
	if (pStartPrev)
		pStartPrev->m_pNext = pEnd;
	else
		m_pHead = pEnd;

	if (pEndNext)
		pStart->m_pNext = pEndNext;
	else
	{
		m_pTail = pStart;
		pStart->m_pNext = nullptr;
	}

#pragma region WithoutSpacialReversing
	//	Node* pCurrent = m_pHead;
//	Node* pNext = nullptr;
//	Node* pPrev = nullptr;
//
//	// Walk through the list
//	while (pCurrent)
//	{
//		// Store next
//		pNext = pCurrent->m_pNext;
//
//		// Relink current node's prev and next
//		pCurrent->m_pNext = pPrev;
//#if DOUBLY_LINKED
//		pCurrent->m_pPrev = pNext;
//#endif
//
//		// Move forward
//		pPrev = pCurrent;
//		pCurrent = pNext;
//	}
//
//	// Reset head and tail
//	m_pTail = m_pHead;
//	m_pHead = pPrev;
#pragma endregion
}

//--------------------------------------------------------------------------------------------------------------------
// Sort this list in numeric order
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Sort()
{
}

template<class Value>
inline void LinkedList<Value>::Print(const char* pPrefix, const char* pSuffix) const
{
	Node* pCurrent = m_pHead;

	std::cout << pPrefix << " { ";
	// Loop through from head
	while (pCurrent)
	{
#if DOUBLY_LINKED
		std::cout << pCurrent->m_value << "<=>";
#else
		std::cout << pCurrent->m_value << "->";
#endif
		pCurrent = pCurrent->m_pNext;
	}
	std::cout << "} " << pSuffix << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Clear()
{
	Destroy();
	m_size = 0;
}

//--------------------------------------------------------------------------------------------------------------------
// Pop first element and return it's value
// Time: O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value LinkedList<Value>::PopFront()
{
	// Underflow checking
	assert(m_size > 0);

	// Reset head
	Node* pFront = m_pHead;
	m_pHead = pFront->m_pNext;

	// Get return value
	Value val = pFront->m_value;

	// Delete pFront
	DeleteNode(pFront);

	// Return Front
	return val;
}

//--------------------------------------------------------------------------------------------------------------------
// Pop last element and return it's value
// Time: O(1) if doubly linked, O(n) if singly linked
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline Value LinkedList<Value>::PopBack()
{
	// Underflow checking
	assert(m_size > 0);

	// Reset tail
#if DOUBLY_LINKED
	Node* pBack = m_pTail;
	m_pTail = pBack->m_pPrev;
#else
	std::pair<Node*, Node*> pNodes = SearchPairByNode(m_pTail);	// O(n), n == m_size :(
	Node* pPrev = pNodes.first;
	Node* pBack = pNodes.second;
	m_pTail = pPrev;
#endif

	// Get return value
	Value val = pBack->m_value;

	// Delete pBack
	DeleteNode(pBack);

	// Set tail's next to nullptr
	if (m_pTail)
		m_pTail->m_pNext = nullptr;

	// Return Front
	return val;
}

//--------------------------------------------------------------------------------------------------------------------
// Find and return the node with the passed in value, return nullptr if not found
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline typename LinkedList<Value>::Node* LinkedList<Value>::SearchNodeByValue(const Value& val)
{
	Node* pResult = m_pHead;
	while (pResult && pResult->m_value != val)
		pResult = pResult->m_pNext;

	return pResult;
}

//--------------------------------------------------------------------------------------------------------------------
// Find and return the node with the passed in node, return nullptr if not found
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline typename LinkedList<Value>::Node* LinkedList<Value>::SearchNodeByNode(const Node* pNode)
{
	Node* pResult = m_pHead;
	while (pResult && pResult != pNode)
		pResult = pResult->m_pNext;

	return pResult;
}

//--------------------------------------------------------------------------------------------------------------------
// Find and return the node with the passed in index, return nullptr if not found
// Time: O(n), n = index
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline  typename LinkedList<Value>::Node* LinkedList<Value>::SearchNodeByIndex(size_t index)
{
	// Get node on index
	Node* pNodeToDelete = m_pHead;
	for (size_t i = 0; i < index; ++i)
		pNodeToDelete = pNodeToDelete->m_pNext;

	return pNodeToDelete;
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a value
// Find and the node with the passed in value, return a pair of result node and it's previous
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::pair<typename LinkedList<Value>::Node*, typename LinkedList<Value>::Node*> LinkedList<Value>::SearchPairByValue(const Value& val)
{
	Node* pPrevious = nullptr;
	Node* pResult = m_pHead;

	// Find the place to insert
	while (pResult && pResult->m_value != val)
	{
		pPrevious = pResult;
		pResult = pResult->m_pNext;
	}

	return { pPrevious , pResult};
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a node pointer
// Find and the node with the passed in value, return a pair of result node and it's previous
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::pair<typename LinkedList<Value>::Node*, typename LinkedList<Value>::Node*> LinkedList<Value>::SearchPairByNode(const Node* pNode)
{
	Node* pPrevious = nullptr;
	Node* pResult = m_pHead;

	// Find the place to insert
	while (pResult && pResult != pNode)
	{
		pPrevious = pResult;
		pResult = pResult->m_pNext;
	}

	return { pPrevious , pResult };
}

//--------------------------------------------------------------------------------------------------------------------
// Takes in a index
// Find and the node with the passed in index, return a pair of result node and it's previous
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline std::pair<typename LinkedList<Value>::Node*, typename LinkedList<Value>::Node*> LinkedList<Value>::SearchPairByIndex(size_t index)
{
	Node* pPrevious = nullptr;
	Node* pResult = m_pHead;

	for (size_t i = 0; i < index; ++i)
	{
		pPrevious = pResult;
		pResult = pResult->m_pNext;
	}

	return { pPrevious , pResult };
}

//--------------------------------------------------------------------------------------------------------------------
// Delete every node in the list
// Time: O(n)
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Destroy()
{
	Node* pCurrent = m_pHead;

	// Loop through from head
	while (pCurrent)
	{
		// Move head to next
		m_pHead = m_pHead->m_pNext;

		if constexpr (!std::is_trivially_destructible_v<Value>)
			pCurrent->m_pNext.~T();

		// Delete current
		delete pCurrent;

		// Set current to head
		pCurrent = m_pHead;
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Set head and tail to nullptr if the list is empty
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::SetNullptrIfEmpty()
{
	if (m_size <= 0)
	{
		m_pHead = nullptr;
		m_pTail = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Internally delete a node
//--------------------------------------------------------------------------------------------------------------------
template<class Value>
inline void LinkedList<Value>::Delete(Node* pNodeToDelete, Node* pPrevious)
{
	// If data is in the list, delete it
	if (pNodeToDelete)
	{
		// If we have a valid previous node or next node, relink them.  
#if DOUBLY_LINKED
		if (pNodeToDelete->m_pPrev)
			pNodeToDelete->m_pPrev->m_pNext = pNodeToDelete->m_pNext;

		if (pNodeToDelete->m_pNext)
			pNodeToDelete->m_pNext->m_pPrev = pNodeToDelete->m_pPrev;
#else
		if (pNodeToDelete->m_pNext && pPrevious)
			pPrevious->m_pNext = pNodeToDelete->m_pNext;
#endif

		// If head is the node to delete, we need to reset the m_pHead pointer.
		if (m_pHead == pNodeToDelete)
			m_pHead = pNodeToDelete->m_pNext;
		// same for tail
		if (m_pTail == pNodeToDelete)
		{
			m_pTail->m_pNext = nullptr;
#if DOUBLY_LINKED
			m_pTail = pNodeToDelete->m_pPrev;
#else
			m_pTail = pPrevious;
#endif
		}

		DeleteNode(pNodeToDelete);
	}
}

template<class Value>
inline void LinkedList<Value>::DeleteNode(Node* pNodeToDelete)
{
	pNodeToDelete->ClearPointers();
	delete pNodeToDelete;
	pNodeToDelete = nullptr;

	// Decrement size
	--m_size;

	// Reset head and tail if necessary
	SetNullptrIfEmpty();
}

template<class Value>
inline void LinkedList<Value>::AssignNodeAtIndex(size_t index, Node* pNode)
{
	Node* pSearchNode = m_pHead;
	for (size_t i = 0; i < index; ++i)
		pSearchNode = pSearchNode->m_pNext;
	pSearchNode = pNode;
}

template<class Value>
inline void LinkedList<Value>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Value value = 0;		// Be used as value

	// Create list
	LinkedList<Value> list;

#if _DEBUG
	list.PushBack(static_cast<Value>(13));
	list.PushBack(static_cast<Value>(19));
	list.PushBack(static_cast<Value>(24));
	list.PushBack(static_cast<Value>(39));
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		list.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kList);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter pushing value: ";
			std::cin >> value;
			list.PushFront(value);
			break;

		case '1':
			std::cout << "Enter pushing value: ";
			std::cin >> value;
			list.PushBack(value);
			break;

		case '2':
			list.PopFront();
			break;

		case '3':
			list.PopBack();
			break;

		case '4':
			std::cout << "Enter pushing value: ";
			std::cin >> value;

			std::cout << "\nEnter pushing index: ";
			std::cin >> i;
			list.Insert(i, value);
			break;

		case '5':
			std::cout << "Enter deleting value: ";
			std::cin >> value;
			list.DeleteByValue(value);
			break;

		case '6':
			list.Reverse();
			break;

		case '7':
			list.Clear();
			break;

		case '8':
			std::cout << "Size: " << list.m_size << std::endl;
			system("pause");
			break;

		case '9':
			std::cout << "Enter deleting index: ";
			std::cin >> i;
			list.DeleteByIndex(i);
			break;

		case 'q':            shouldQuit = true;            break;
		}

		system("cls");
	}
}