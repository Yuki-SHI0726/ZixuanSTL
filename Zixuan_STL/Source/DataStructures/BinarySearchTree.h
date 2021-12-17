#pragma once
#include "Utils/StructureManager.h"

#include <iostream>
#include <assert.h>
#include <string>
#include <optional>

namespace zxstl
{
//---------------------------------------------------------------------------------------------------------------------
// Binary Search Tree Declaration
//---------------------------------------------------------------------------------------------------------------------
template <class _KeyType, class _DataType>
class BinarySearchTree
{
public:
	using BST = BinarySearchTree<_KeyType, _DataType>;
	using KeyType = _KeyType;
	using DataType = _DataType;

private:
	struct Node
	{
		Node* m_pParent;
		Node* m_pLeft;
		Node* m_pRight;

		KeyType m_key;
		DataType m_data;

		Node(const KeyType& key, const DataType& data)
			: m_pParent(nullptr)
			, m_pLeft(nullptr)
			, m_pRight(nullptr)
			, m_key(key)
			, m_data(data)
		{
		}

		~Node()
		{
			delete m_pLeft;
			delete m_pRight;
			ClearPointers();
		}

		void ClearPointers()
		{
			m_pParent = nullptr;
			m_pLeft = nullptr;
			m_pRight = nullptr;
		}
	};

private:
	Node* m_pRoot;
	size_t m_size;

public:
	BinarySearchTree();
	~BinarySearchTree();

	// Modifiers
	void Insert(const KeyType& key, const DataType& data);
	void Clear();
	void DeleteBySuccessor(const KeyType& key);
	void DeleteByPredecessor(const KeyType& key);
	void ChangeKey(const KeyType& keyToFind, const KeyType& keyToChange);

	// Accessors
	std::optional<DataType> FindMinIter() const;
	std::optional<DataType> FindMaxIter() const;
	std::optional<DataType> FindMinRecur() const;
	std::optional<DataType> FindMaxRecur() const;
	int GetHeight(Node* pNode) const;
	size_t GetSize() const { return m_size; }
	std::optional<DataType> GetRootData() const;

	// Traversal
	template <class Func> void InOrderWalkIterative(Func&& func);
	template <class Func> void InOrderWalkRecursive(Func&& func);
	template <class Func> void PreOrderWalkRecursive(Func&& func);
	template <class Func> void PostOrderWalkRecursive(Func&& func);
	void PrintNodesInOrder() const;

	// Look up
	std::optional<DataType> Search(const KeyType& key) const;
	std::optional<DataType> operator[](const KeyType& key) { return Search(key); }
	const std::optional<DataType> operator[](const KeyType& key) const { return Search(key); }

	// Test
	static void Test();

private:
	// Modifier
	void Destroy();
	void Transplant(Node* pNodeToReplace, Node* pReplacingNode);

	// Accessors
	Node* InteralFindMinRecur(Node* pParent) const;
	Node* InternalFindMaxRecur(Node* pParent) const;

	// Traversal
	template <class Func> void RecursiveInOrderWalk(Node* pNode, Func&& func);
	void RecursivePrintNodesInOrder(Node* pNode) const;
	template <class Func> void RecursivePreOrderWalk(Node* pNode, Func&& func);
	template <class Func> void RecursivePostOrderWalk(Node* pNode, Func&& func);

	// Lookup
	Node* InternalFindNode(const KeyType& key) const;
};

template<class KeyType, class DataType>
BinarySearchTree<KeyType, DataType>::BinarySearchTree()
	: m_pRoot(nullptr)
	, m_size{ 0 }
{
}

template<class KeyType, class DataType>
BinarySearchTree<KeyType, DataType>::~BinarySearchTree()
{
	Destroy();
}

template<class KeyType, class DataType>
void BinarySearchTree<KeyType, DataType>::Insert(const KeyType& key, const DataType& data)
{
	Node* pNewNode = new Node(key, data);

	Node* pParent = nullptr;
	Node* pCurrent = m_pRoot;

	// walk through the tree to find the spot to insert this node
	while (pCurrent)
	{
		// set the last node, aka the parent node
		pParent = pCurrent;

		// move to the next level
		if (key < pCurrent->m_key)
			pCurrent = pCurrent->m_pLeft;
		else
			pCurrent = pCurrent->m_pRight;
	}

	// Once we get here, pParent will be the root of where we need to insert the new node.  If it's nullptr, it
	// means the tree is empty.
	if (pParent == nullptr)
	{
		m_pRoot = pNewNode;
	}
	else if (key < pParent->m_key)
	{
		pParent->m_pLeft = pNewNode;
		pNewNode->m_pParent = pParent;
	}
	else
	{
		pParent->m_pRight = pNewNode;
		pNewNode->m_pParent = pParent;
	}

	// Success added node
	++m_size;
}

//---------------------------------------------------------------------------------------------------------------------
// Delete all nodes in the tree
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::Clear()
{
	Destroy();
}

//---------------------------------------------------------------------------------------------------------------------
// Search and delete input key node
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::DeleteBySuccessor(const KeyType& key)
{
	// Find node to delete, return if not exists
	Node* pNodeToDelete = InternalFindNode(key);
	if (!pNodeToDelete)
		return;

	// Case 1: Node has no left children. Shift right node up.
	if (!pNodeToDelete->m_pLeft)
		Transplant(pNodeToDelete, pNodeToDelete->m_pRight);

	// Case 2: Node has no right children. Shift left node up.
	else if (!pNodeToDelete->m_pRight)
		Transplant(pNodeToDelete, pNodeToDelete->m_pLeft);

	// Case 3: Both children are valid.
	else
	{
		// The successor is guaranteed to be the node with the smallest value in our node's right branch.  
		// We need to sugically remove the successor and replace our node with it.
		Node* pSuccessor = InteralFindMinRecur(pNodeToDelete->m_pRight);
		assert(pSuccessor);

		// If the successor is not the direct child of the node we want to delete, 
		if (pSuccessor->m_pParent != pNodeToDelete)
		{
			// Link right children
			Transplant(pSuccessor, pSuccessor->m_pRight);
			pSuccessor->m_pRight = pNodeToDelete->m_pRight;
			pSuccessor->m_pRight->m_pParent = pSuccessor;
		}

		// Link deleted left children to successor
		Transplant(pNodeToDelete, pSuccessor);
		pSuccessor->m_pLeft = pNodeToDelete->m_pLeft;
		pSuccessor->m_pLeft->m_pParent = pSuccessor;
	}

	// Destroy node
	pNodeToDelete->ClearPointers();
	delete pNodeToDelete;
	pNodeToDelete = nullptr;

	// Success deleted node
	--m_size;
}

//---------------------------------------------------------------------------------------------------------------------
// Delete node using predecessor
//	1. Find max value on the deleting node's left subtree
//	2. Transplant predecessor's left subtree if it's not the direct child of the node we want to delete, 
//	3. Link deleted right children to predecessor
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::DeleteByPredecessor(const KeyType& key)
{	
	// Find node to delete, return if not exists
	Node* pNodeToDelete = InternalFindNode(key);
	if (!pNodeToDelete)
		return;

	// Case 1: Node has no left children. Shift right node up.
	if (!pNodeToDelete->m_pLeft)
		Transplant(pNodeToDelete, pNodeToDelete->m_pRight);

	// Case 2: Node has no right children. Shift left node up.
	else if (!pNodeToDelete->m_pRight)
		Transplant(pNodeToDelete, pNodeToDelete->m_pLeft);

	// Case 3: Both children are valid.
	else
	{
		// The predecessor is guaranteed to be the node with the largest value in our node's left branch.  
		// We need to sugically remove the predecessor and replace our node with it.
		Node* pPredecessor = InternalFindMaxRecur(pNodeToDelete->m_pLeft);
		assert(pPredecessor);

		// If the predecessor is not the direct child of the node we want to delete, 
		if (pPredecessor->m_pParent != pNodeToDelete)
		{
			// Link m_pLeft children
			Transplant(pPredecessor, pPredecessor->m_pLeft);
			pPredecessor->m_pLeft = pNodeToDelete->m_pLeft;
			pPredecessor->m_pLeft->m_pParent = pPredecessor;
		}

		// Link deleted right children to predecessor
		Transplant(pNodeToDelete, pPredecessor);
		pPredecessor->m_pRight = pNodeToDelete->m_pRight;
		pPredecessor->m_pRight->m_pParent = pPredecessor;
	}

	// Destroy node
	pNodeToDelete->ClearPointers();
	delete pNodeToDelete;
	pNodeToDelete = nullptr;

	// Success deleted node
	--m_size;
}

//--------------------------------------------------------------------------------------------------------------------
// Return minimum value in this tree
//--------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
typename std::optional<_DataType> BinarySearchTree<_KeyType, _DataType>::FindMinIter() const
{
	// Return nothing if the tree is empty
	if (!m_pRoot)
		return {};

	// Dive left until got the end
	Node* pCurrent = m_pRoot;
	while (pCurrent->m_pLeft)
		pCurrent = pCurrent->m_pLeft;

	return pCurrent->m_data;
}

//--------------------------------------------------------------------------------------------------------------------
// Return maximum value in this tree
//--------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
typename std::optional<_DataType> BinarySearchTree<_KeyType, _DataType>::FindMaxIter() const
{
	// Return nothing if the tree is empty
	if (!m_pRoot)
		return {};

	// Dive right until got the end
	Node* pCurrent = m_pRoot;
	while (pCurrent->m_pRight)
		pCurrent = pCurrent->m_pRight;

	return pCurrent->m_data;
}

template<class _KeyType, class _DataType>
typename std::optional<_DataType> BinarySearchTree<_KeyType, _DataType>::FindMinRecur() const
{
	// Return nothing if the tree is empty
	if (!m_pRoot)
		return {};

	Node* pNode = InteralFindMinRecur(m_pRoot);
	return pNode->m_data;
}

template<class _KeyType, class _DataType>
typename std::optional<_DataType> BinarySearchTree<_KeyType, _DataType>::FindMaxRecur() const
{
	// Return nothing if the tree is empty
	if (!m_pRoot)
		return {};

	Node* pNode = InternalFindMaxRecur(m_pRoot);
	return pNode->m_data;
}

//---------------------------------------------------------------------------------------------------------------------
// Allows you to change the key of a node. When a key is changed, it will need to be removed from the tree and reinserted.
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::ChangeKey(const KeyType& keyToFind, const KeyType& keyToChange)
{
	// Find node to change, return if not exists
	Node* pNodeToChange = InternalFindNode(keyToFind);
	if (!pNodeToChange)
		return;

	// Retrieve data
	DataType data = pNodeToChange->m_data;

	// Remove it from the tree
	DeleteBySuccessor(keyToFind);

	// Re insert it
	Insert(keyToChange, data);
}

template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::PrintNodesInOrder() const
{
	RecursivePrintNodesInOrder(m_pRoot);
}

template<class KeyType, class DataType>
inline std::optional<DataType> BinarySearchTree<KeyType, DataType>::Search(const KeyType& key) const
{
	Node* pNode = InternalFindNode(key);
	if (pNode)
		return pNode->m_data;

	return {};
}

//---------------------------------------------------------------------------------------------------------------------
// Recursively get tree height
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline int BinarySearchTree<_KeyType, _DataType>::GetHeight(Node* pNode) const
{
	// Empty tree
	if (!m_pRoot)
		return 0;

	int leftSubTreeHeight = GetHeight(pNode->m_pLeft);
	int rightSubTreeHeight = GetHeight(pNode->m_pRight);

	// Return the greater subtree height
	return leftSubTreeHeight > rightSubTreeHeight ? leftSubTreeHeight + 1 : rightSubTreeHeight + 1;
}

//---------------------------------------------------------------------------------------------------------------------
// Return root's data if valid
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline std::optional<_DataType> BinarySearchTree<_KeyType, _DataType>::GetRootData() const
{
	if (m_pRoot)
		return m_pRoot->m_data;
	return {};
}

template<class KeyType, class DataType>
template<class Func>
void BinarySearchTree<KeyType, DataType>::InOrderWalkRecursive(Func&& func)
{
	RecursiveInOrderWalk(m_pRoot, std::forward<Func>(func));
}

//---------------------------------------------------------------------------------------------------------------------
// Iterative Inorder tree walk
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
template<class Func>
void BinarySearchTree<_KeyType, _DataType>::InOrderWalkIterative(Func&& func)
{
	// Data
	Node* pCurrent = m_pRoot;
	Node* pPrevious = nullptr;		// Testing two pointers for equality

	// While current is not nullptr
	while (pCurrent)
	{
		// If the current node does not has left child, means it's the smallest node and we are at the left most node
		if (!pCurrent->m_pLeft)
		{
			// Walk the function
			func(const_cast<const KeyType&>(pCurrent->m_key), pCurrent->m_data);

			// Go to right
			pCurrent = pCurrent->m_pRight;
		}
		// Else if we have a left child
		else
		{
			// Make current as the right child of the rightmost node in current's left subtree
			// Find the rightmost node in current's children
			pPrevious = pCurrent->m_pLeft;
			while (pPrevious->m_pRight != nullptr && pPrevious->m_pRight != pCurrent)
			{
				pPrevious = pPrevious->m_pRight;
			}

			// Make current as the pPrevious's right child
			if (!pPrevious->m_pRight)
			{
				pPrevious->m_pRight = pCurrent;

				// Go to this left child.
				pCurrent = pCurrent->m_pLeft;
			}
			else
			{
				pPrevious->m_pRight = nullptr;
				func(const_cast<const KeyType&>(pCurrent->m_key), pCurrent->m_data);
				pCurrent = pCurrent->m_pRight;
			}
		}
	}
}

template<class _KeyType, class _DataType>
template<class Func>
inline void BinarySearchTree<_KeyType, _DataType>::PreOrderWalkRecursive(Func&& func)
{
	RecursivePreOrderWalk(m_pRoot, std::forward<Func>(func));
}

template<class _KeyType, class _DataType>
template<class Func>
inline void BinarySearchTree<_KeyType, _DataType>::PostOrderWalkRecursive(Func&& func)
{
	RecursivePostOrderWalk(m_pRoot, std::forward<Func>(func));
}

template<class _KeyType, class _DataType>
template<class Func>
inline void BinarySearchTree<_KeyType, _DataType>::RecursivePreOrderWalk(Node* pNode, Func&& func)
{
	if (pNode)
	{
		func(const_cast<const KeyType&>(pNode->m_key), pNode->m_data);
		RecursivePreOrderWalk(pNode->m_pLeft, std::forward<Func>(func));
		RecursivePreOrderWalk(pNode->m_pRight, std::forward<Func>(func));
	}
}

template<class _KeyType, class _DataType>
template<class Func>
inline void BinarySearchTree<_KeyType, _DataType>::RecursivePostOrderWalk(Node* pNode, Func&& func)
{
	if (pNode)
	{
		RecursivePostOrderWalk(pNode->m_pLeft, std::forward<Func>(func));
		RecursivePostOrderWalk(pNode->m_pRight, std::forward<Func>(func));
		func(const_cast<const KeyType&>(pNode->m_key), pNode->m_data);
	}
}

template<class KeyType, class DataType>
template<class Func>
void BinarySearchTree<KeyType, DataType>::RecursiveInOrderWalk(Node* pNode, Func&& func)
{
	if (pNode)
	{
		// Go to find the smallest element
		RecursiveInOrderWalk(pNode->m_pLeft, std::forward<Func>(func));
		func(const_cast<const KeyType&>(pNode->m_key), pNode->m_data);
		RecursiveInOrderWalk(pNode->m_pRight, std::forward<Func>(func));
	}
}

// Thomas990726@

template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	_KeyType key = 0;			// Used as capacity and index
	_DataType value = 0;		// Be used as value

	BST bst;
#if _DEBUG
	bst.Insert(static_cast<_KeyType>(10), static_cast<_DataType>(10));
	bst.Insert(static_cast<_KeyType>(1), static_cast<_DataType>(1));
	bst.Insert(static_cast<_KeyType>(5), static_cast<_DataType>(5));
	bst.Insert(static_cast<_KeyType>(6), static_cast<_DataType>(6));
	bst.Insert(static_cast<_KeyType>(9), static_cast<_DataType>(9));
	bst.Insert(static_cast<_KeyType>(7), static_cast<_DataType>(7));
	bst.Insert(static_cast<_KeyType>(12), static_cast<_DataType>(12));
#else

#endif
	// Loop work
	while (!shouldQuit)
	{
		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kBinarySearchTree);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter inserting value: ";
			std::cin >> value;

			bst.Insert(static_cast<_KeyType>(value), value);
			break;

		case '1':
			bst.InOrderWalkIterative([](const BST::KeyType& key, const BST::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;


		case '2':
			bst.PreOrderWalkRecursive([](const BST::KeyType& key, const BST::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;

		case '3':
			bst.PostOrderWalkRecursive([](const BST::KeyType& key, const BST::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;

		case '4':
		{
			std::optional<_DataType> minData = bst.FindMinRecur();
			if (minData.has_value())
				std::cout << "Min value: " << minData.value() << std::endl;
			else
				std::cout << "Empty tree" << std::endl;
			system("pause");
			break;
		}
		case '5':
		{
			std::optional<_DataType> maxData = bst.FindMaxRecur();
			if (maxData.has_value())
				std::cout << "Maxq value: " << maxData.value() << std::endl;
			else
				std::cout << "Empty tree" << std::endl;
			system("pause");
			break;
		}

		case '6':
		{
			std::cout << "Enter searching key: ";
			std::cin >> key;

			std::optional<DataType> result = bst.Search(key);
			if (result.has_value())
				std::cout << result.value() << std::endl;
			else
				std::cout << "Value is not found." << std::endl;

			system("pause");
			break;
		}

		case '7':
			bst.Clear();
			break;

		case '8':
			std::cout << "Enter deleting key: ";
			std::cin >> key;
			bst.DeleteByPredecessor(key);
			break;

		case '9':
		{
			std::cout << "Enter the key you want to find: ";
			std::cin >> key;

			_KeyType keyToChange = static_cast<_KeyType>(0);
			std::cout << "\nEnter the key you want to change: ";
			std::cin >> keyToChange;
			bst.ChangeKey(key, keyToChange);

			break;
		}
		case 'q':            
			shouldQuit = true;     
			break;
		}

		system("cls");
	}
}

//---------------------------------------------------------------------------------------------------------------------
// Destroy this tree by deleting root node
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::Destroy()
{
	delete m_pRoot;
	m_pRoot = nullptr;

	m_size = 0;
}

//---------------------------------------------------------------------------------------------------------------------
// Transplants one subtree with another.
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void BinarySearchTree<_KeyType, _DataType>::Transplant(Node* pNodeToReplace, Node* pReplacingNode)
{
	// Check to see if we're replacing the root node
	// If so, set root node as pReplacing node
	if (!pNodeToReplace->m_pParent)
		m_pRoot = pReplacingNode;

	// If we get here, we're either a right or a left child node.
	// Handle each case by pointing those nodes to the replacing node.

	// Left child
	else if (pNodeToReplace == pNodeToReplace->m_pParent->m_pLeft)
		pNodeToReplace->m_pParent->m_pLeft = pReplacingNode;

	// Right child
	else if(pNodeToReplace == pNodeToReplace->m_pParent->m_pRight)
		pNodeToReplace->m_pParent->m_pRight = pReplacingNode;

	// Update the parent of the replacing node, assuming the replacing node is valid
	if (pReplacingNode)
		pReplacingNode->m_pParent = pNodeToReplace->m_pParent;
}

//---------------------------------------------------------------------------------------------------------------------
// Return the smallest starting the input parent node Recursively
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline typename BinarySearchTree<_KeyType, _DataType>::Node* BinarySearchTree<_KeyType, _DataType>::InteralFindMinRecur(Node* pParent) const
{
	if (pParent->m_pLeft)
		return InteralFindMinRecur(pParent->m_pLeft);

	return pParent;
}

//---------------------------------------------------------------------------------------------------------------------
// Return the largest starting the input parent node in the tree Recursively
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline typename BinarySearchTree<_KeyType, _DataType>::Node* BinarySearchTree<_KeyType, _DataType>::InternalFindMaxRecur(Node* pParent) const
{
	if (pParent->m_pRight)
		return InternalFindMaxRecur(pParent->m_pRight);

	return pParent;
}

template<class _KeyType, class _DataType>
inline typename BinarySearchTree<_KeyType, _DataType>::Node* BinarySearchTree<_KeyType, _DataType>::InternalFindNode(const KeyType& key) const
{
	Node* pCurrent = m_pRoot;
	while (pCurrent)
	{
		// if this is the node we're looking for, return it
		if (key == pCurrent->m_key)
			return pCurrent;

		if (key < pCurrent->m_key)
			pCurrent = pCurrent->m_pLeft;
		else
			pCurrent = pCurrent->m_pRight;
	}

	// if we get here, we didn't find the node
	return nullptr;
}


template<class KeyType, class DataType>
void BinarySearchTree<KeyType, DataType>::RecursivePrintNodesInOrder(Node* pNode) const
{
	if (pNode)
	{
		RecursivePrintNodesInOrder(pNode->m_pLeft);
		std::cout << pNode->m_key << " => " << pNode->m_data << "\n";
		RecursivePrintNodesInOrder(pNode->m_pRight);
	}
}

}
