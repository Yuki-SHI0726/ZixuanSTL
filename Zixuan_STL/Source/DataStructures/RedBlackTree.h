#pragma once
#include "Utils/StructureManager.h"

#include <iostream>
#include <assert.h>
#include <string>
#include <optional>

namespace zxstl
{
//---------------------------------------------------------------------------------------------------------------------
// Red black tree implementation
//  - Every node is either red or black.
//  - The root is black.
//  - Every leaf(nil) is black.
//  - If the node is red, then both its children are black.
//  - For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.
//---------------------------------------------------------------------------------------------------------------------
template <class _KeyType, class _DataType>
class RedBlackTree
{
public:
	using RBT = RedBlackTree<_KeyType, _DataType>;
	using KeyType = _KeyType;
	using DataType = _DataType;

private:
	enum class NodeColor : uint8_t
	{
		kRed,
		kBlack,
	};

	struct Node
	{
		Node* m_pParent;
		Node* m_pLeft;
		Node* m_pRight;

		KeyType m_key;
		DataType m_data;
		NodeColor m_color;

		Node(const KeyType& key, const DataType& data)
			: m_pParent(nullptr)
			, m_pLeft(nullptr)
			, m_pRight(nullptr)
			, m_key(key)
			, m_data(data)
			, m_color(NodeColor::kRed)
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

		void PrintNode(int depth) const
		{
			// print out spaces for each level
			for (int i = 0; i < depth; ++i)
			{
				std::cout << "  ";
			}

			// print out this node
			std::cout << m_key << " -> " << m_data << ((m_color == NodeColor::kBlack) ? (" (B)") : (" (R)")) << std::endl;

			// print the left node
			if (m_pLeft)
				m_pLeft->PrintNode(depth + 1);

			// print the right node
			if (m_pRight)
				m_pRight->PrintNode(depth + 1);
		}
	};

private:
	Node* m_pRoot;
	size_t m_size;

public:
	RedBlackTree();
	~RedBlackTree();

	// Modifiers
	void Insert(const KeyType& key, const DataType& data);
	void Clear();
	void Delete(const KeyType& key);

	// Max and minimum
	std::optional<DataType> FindMinIter() const;
	std::optional<DataType> FindMaxIter() const;
	std::optional<DataType> FindMinRecur() const;
	std::optional<DataType> FindMaxRecur() const;

	// Changing Binary Tree
	void ChangeKey(const KeyType& keyToFind, const KeyType& keyToChange);

	// Traversal
	template <class Func> void PreOrderWalkRecursive(Func&& func);
	template <class Func> void PostOrderWalkRecursive(Func&& func);
	template <class Func> void InOrderWalkRecursive(Func&& func);
	template <class Func> void InOrderWalkIterative(Func&& func);

	// Print
	void PrintNodesInOrder() const;
	void PrintTree() const;

	// Look up
	std::optional<DataType> Search(const KeyType& key) const;
	std::optional<DataType> operator[](const KeyType& key) { return Search(key); }
	const std::optional<DataType> operator[](const KeyType& key) const { return Search(key); }
	int GetHeight(Node* pNode) const;
	size_t GetSize() const { return m_size; }
	std::optional<DataType> GetRootData() const;

	// Test
	static void Test();

private:
	// Modifier
	void Destroy();
	void RedBlackTransplant(Node* pNodeToReplace, Node* pReplacingNode);

	// Max and minimum
	Node* InteralFindMinRecur(Node* pParent) const;
	Node* InternalFindMaxRecur(Node* pParent) const;

	// Traversal
	template <class Func> void RecursivePreOrderWalk(Node* pNode, Func&& func);
	template <class Func> void RecursivePostOrderWalk(Node* pNode, Func&& func);
	template <class Func> void RecursiveInOrderWalk(Node* pNode, Func&& func);

	// Print
	void RecursivePrintNodesInOrder(Node* pNode) const;

	// Lookup
	Node* InternalFindNode(const KeyType& key) const;

	//-------------------------------------------------------------
	// red / black tre
	//-------------------------------------------------------------
	void FixupAfterInsert(Node* pNodeInserted);
	void FixupAfterDelete(Node* pNodeFix);

	// Node rotation.  In this example, pNode == x.
	// 
	//     y  <-- RL  x
	//    /   RR -->   \
    //   x              y
	void RotateLeft(Node* pNode);
	void RotateRight(Node* pNode);

	NodeColor GetNodeColor(Node* pNode) const;
};

template<class _KeyType, class _DataType>
inline RedBlackTree<_KeyType, _DataType>::RedBlackTree()
	: m_pRoot(nullptr)
	, m_size{ 0 }
{
}

template<class _KeyType, class _DataType>
inline RedBlackTree<_KeyType, _DataType>::~RedBlackTree()
{
	Destroy();
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::Insert(const KeyType& key, const DataType& data)
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

	// Red black tree fix
	FixupAfterInsert(pNewNode);

	// Success added node
	++m_size;
}

//---------------------------------------------------------------------------------------------------------------------
// Delete all nodes in the tree
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::Clear()
{
	Destroy();
}

//---------------------------------------------------------------------------------------------------------------------
// Search and delete input key node
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::Delete(const KeyType& key)
{
	// Find node to delete, return if not exists
	Node* pNodeToDelete = InternalFindNode(key);
	if (!pNodeToDelete)
		return;

	//  sets y to point to node z when z has fewer than two children and is therefore removed.
	Node* pSuccessor = pNodeToDelete;

	// Because node y¡¯s color might change, the variable y-original-color stores y¡¯s color before any changes occur.
	NodeColor successorOriginalColor = pSuccessor->m_color;

	// Node to fix
	Node* pNodeToFix = nullptr;

	// Case 1: Node has no left children. Shift right node up.
	if (!pNodeToDelete->m_pLeft)
	{
		pNodeToFix = pNodeToDelete->m_pRight;
		RedBlackTransplant(pNodeToDelete, pNodeToFix);
	}
	// Case 2: Node has no right children. Shift left node up.
	else if (!pNodeToDelete->m_pRight)
	{
		pNodeToFix = pNodeToDelete->m_pLeft;
		RedBlackTransplant(pNodeToDelete, pNodeToFix);
	}
	// Case 3: Both children are valid.
	else
	{
		// The successor is guaranteed to be the node with the smallest value in our node's right branch.  
		// We need to sugically remove the successor and replace our node with it.
		pSuccessor = InteralFindMinRecur(pNodeToDelete->m_pRight);
		assert(pSuccessor);

		// Reset successor's color
		successorOriginalColor = pSuccessor->m_color;
		pNodeToFix = pSuccessor->m_pRight;

		// If the successor is not the direct child of the node we want to delete, 
		if (pSuccessor->m_pParent == pNodeToDelete)
		{
			pNodeToFix->m_pParent = pSuccessor;
		}
		else
		{
			// Link right children
			RedBlackTransplant(pSuccessor, pNodeToFix);
			pSuccessor->m_pRight = pNodeToDelete->m_pRight;
			pSuccessor->m_pRight->m_pParent = pSuccessor;
		}

		// Link deleted left children to successor
		RedBlackTransplant(pNodeToDelete, pSuccessor);
		pSuccessor->m_pLeft = pNodeToDelete->m_pLeft;
		pSuccessor->m_pLeft->m_pParent = pSuccessor;
		pSuccessor->m_color = pNodeToDelete->m_color;
	}

	// Fix up after delete
	if (successorOriginalColor == NodeColor::kBlack)
		FixupAfterDelete(pNodeToFix);

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
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::FindMinIter() const
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
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::FindMaxIter() const
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
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::FindMinRecur() const
{
	// Return nothing if the tree is empty
	if (!m_pRoot)
		return {};

	Node* pNode = InteralFindMinRecur(m_pRoot);
	return pNode->m_data;
}

template<class _KeyType, class _DataType>
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::FindMaxRecur() const
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
inline void RedBlackTree<_KeyType, _DataType>::ChangeKey(const KeyType& keyToFind, const KeyType& keyToChange)
{	
	// Find node to change, return if not exists
	Node* pNodeToChange = InternalFindNode(keyToFind);
	if (!pNodeToChange)
		return;

	// Retrieve data
	DataType data = pNodeToChange->m_data;

	// Remove it from the tree
	Delete(keyToFind);

	// Re insert it
	Insert(keyToChange, data);
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::PrintNodesInOrder() const
{
	RecursivePrintNodesInOrder(m_pRoot);
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::PrintTree() const
{
	if (m_pRoot)
		m_pRoot->PrintNode(0);
}

template<class _KeyType, class _DataType>
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::Search(const KeyType& key) const
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
inline int RedBlackTree<_KeyType, _DataType>::GetHeight(Node* pNode) const
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
inline std::optional<_DataType> RedBlackTree<_KeyType, _DataType>::GetRootData() const
{
	if (m_pRoot)
		return m_pRoot->m_data;
	return {};
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	_KeyType key = 0;			// Used as capacity and index
	_DataType value = 0;		// Be used as value

	RBT rbt;
#if _DEBUG
	rbt.Insert(static_cast<_KeyType>(10), static_cast<_DataType>(10));
	rbt.Insert(static_cast<_KeyType>(1), static_cast<_DataType>(1));
	rbt.Insert(static_cast<_KeyType>(5), static_cast<_DataType>(5));
	rbt.Insert(static_cast<_KeyType>(6), static_cast<_DataType>(6));
	rbt.Insert(static_cast<_KeyType>(9), static_cast<_DataType>(9));
	rbt.Insert(static_cast<_KeyType>(7), static_cast<_DataType>(7));
	rbt.Insert(static_cast<_KeyType>(12), static_cast<_DataType>(12));
#else

#endif
	// Loop work
	while (!shouldQuit)
	{
		// Print
		rbt.PrintTree();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kBinarySearchTree);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter inserting value: ";
			std::cin >> value;

			rbt.Insert(static_cast<_KeyType>(value), value);
			break;

		case '1':
			rbt.InOrderWalkIterative([](const RBT::KeyType& key, const RBT::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;


		case '2':
			rbt.PreOrderWalkRecursive([](const RBT::KeyType& key, const RBT::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;

		case '3':
			rbt.PostOrderWalkRecursive([](const RBT::KeyType& key, const RBT::DataType& data)
			{
				std::cout << key << " => " << data << "\n";
			});
			system("pause");
			break;

		case '4':
		{
			std::optional<_DataType> minData = rbt.FindMinRecur();
			if (minData.has_value())
				std::cout << "Min value: " << minData.value() << std::endl;
			else
				std::cout << "Empty tree" << std::endl;
			system("pause");
			break;
		}
		case '5':
		{
			std::optional<_DataType> maxData = rbt.FindMaxRecur();
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

			std::optional<DataType> result = rbt.Search(key);
			if (result.has_value())
				std::cout << result.value() << std::endl;
			else
				std::cout << "Value is not found." << std::endl;

			system("pause");
			break;
		}

		case '7':
			rbt.Clear();
			break;

		case '8':
			std::cout << "Enter deleting key: ";
			std::cin >> key;
			rbt.Delete(key);
			break;

		case '9':
		{
			std::cout << "Enter the key you want to find: ";
			std::cin >> key;

			_KeyType keyToChange = static_cast<_KeyType>(0);
			std::cout << "\nEnter the key you want to change: ";
			std::cin >> keyToChange;
			rbt.ChangeKey(key, keyToChange);

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
inline void RedBlackTree<_KeyType, _DataType>::Destroy()
{
	delete m_pRoot;
	m_pRoot = nullptr;

	m_size = 0;
}

//---------------------------------------------------------------------------------------------------------------------
// Transplants one subtree with another.
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::RedBlackTransplant(Node* pNodeToReplace, Node* pReplacingNode)
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
	else if (pNodeToReplace == pNodeToReplace->m_pParent->m_pRight)
		pNodeToReplace->m_pParent->m_pRight = pReplacingNode;

	// Update the parent of the replacing node
	pReplacingNode->m_pParent = pNodeToReplace->m_pParent;
}

//---------------------------------------------------------------------------------------------------------------------
// Return the smallest starting the input parent node Recursively
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline typename RedBlackTree<_KeyType, _DataType>::Node* RedBlackTree<_KeyType, _DataType>::InteralFindMinRecur(Node* pParent) const
{
	if (pParent->m_pLeft)
		return InteralFindMinRecur(pParent->m_pLeft);

	return pParent;
}

//---------------------------------------------------------------------------------------------------------------------
// Return the largest starting the input parent node in the tree Recursively
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
inline typename RedBlackTree<_KeyType, _DataType>::Node* RedBlackTree<_KeyType, _DataType>::InternalFindMaxRecur(Node* pParent) const
{
	if (pParent->m_pRight)
		return InternalFindMaxRecur(pParent->m_pRight);

	return pParent;
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::RecursivePrintNodesInOrder(Node* pNode) const
{
	if (pNode)
	{
		RecursivePrintNodesInOrder(pNode->m_pLeft);
		std::cout << pNode->m_key << " => " << pNode->m_data << "\n";
		RecursivePrintNodesInOrder(pNode->m_pRight);
	}
}

template<class _KeyType, class _DataType>
inline typename RedBlackTree<_KeyType, _DataType>::Node* RedBlackTree<_KeyType, _DataType>::InternalFindNode(const KeyType& key) const
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

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::FixupAfterInsert(Node* pNodeInserted)
{
	// pNodeInserted is z in the algorithm
	// pUncle is y in the algorithm

	while (pNodeInserted != m_pRoot && GetNodeColor(pNodeInserted->m_pParent) == NodeColor::kRed)
	{
		// if our parent is the left child
		if (pNodeInserted->m_pParent == pNodeInserted->m_pParent->m_pParent->m_pLeft)
		{
			// get uncle
			Node* pUncle = pNodeInserted->m_pParent->m_pParent->m_pRight;

			// Case 1: Uncle is red
			if (GetNodeColor(pUncle) == NodeColor::kRed)
			{
				pNodeInserted->m_pParent->m_color = NodeColor::kBlack;
				if (pUncle)
					pUncle->m_color = NodeColor::kBlack;
				pNodeInserted->m_pParent->m_pParent->m_color = NodeColor::kRed;
				pNodeInserted = pNodeInserted->m_pParent->m_pParent;
			}

			else  // uncle is black
			{
				// Case 2: Node's uncle is black and node is a right child
				if (pNodeInserted == pNodeInserted->m_pParent->m_pRight)
				{
					pNodeInserted = pNodeInserted->m_pParent;
					RotateLeft(pNodeInserted);
				}

				// Case 3: Node's uncle is black and node is a left child.
				// Note that case 2 falls into case 3.
				pNodeInserted->m_pParent->m_color = NodeColor::kBlack;
				pNodeInserted->m_pParent->m_pParent->m_color = NodeColor::kRed;
				RotateRight(pNodeInserted->m_pParent->m_pParent);
			}
		}

		// parent is the right child
		else
		{
			// get node's uncle
			Node* pUncle = pNodeInserted->m_pParent->m_pParent->m_pLeft;

			// Case 1: Node's uncle is red
			if (GetNodeColor(pUncle) == NodeColor::kRed)
			{
				pNodeInserted->m_pParent->m_color = NodeColor::kBlack;
				pUncle->m_color = NodeColor::kBlack;
				pNodeInserted->m_pParent->m_pParent->m_color = NodeColor::kRed;
				pNodeInserted = pNodeInserted->m_pParent->m_pParent;
			}

			else
			{
				// Case 2: Node's uncle is black and node is a right child
				if (pNodeInserted == pNodeInserted->m_pParent->m_pLeft)
				{
					pNodeInserted = pNodeInserted->m_pParent;
					RotateRight(pNodeInserted);
				}

				// Case 3: Node's uncle is black and node is a left child.
				// Note that case 2 falls into case 3.
				pNodeInserted->m_pParent->m_color = NodeColor::kBlack;
				pNodeInserted->m_pParent->m_pParent->m_color = NodeColor::kRed;
				RotateLeft(pNodeInserted->m_pParent->m_pParent);
			}
		}
	}

	// force root node to be black
	m_pRoot->m_color = NodeColor::kBlack;
}

// Node rotation.  In this example, pNode == x.
// 
//     y  <-- RL  x
//    /   RR -->   \
//   x              y

// Before Left Rotation:
//       15
//         \
//          20
//          /
//         17

// After Left Rotation:
//       20
//      /
//     15
//      \
//       17
template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::RotateLeft(Node* pNode)
{
	assert(pNode);
	assert(pNode->m_pRight);

	// Set the other node.  This is 'y' in the algorithm.
	// y = x.right
	Node* pOther = pNode->m_pRight;

// turn the other node's left subtree into our node's right subtree
// x.right = y.left
pNode->m_pRight = pOther->m_pLeft;

// if y.left != T.nil
if (pOther->m_pLeft)
pOther->m_pLeft->m_pParent = pNode;

// link out node's parent to the other node's parent
pOther->m_pParent = pNode->m_pParent;

if (!pNode->m_pParent)  // check to see if this is the root node
m_pRoot = pOther;
else if (pNode == pNode->m_pParent->m_pLeft)
pNode->m_pParent->m_pLeft = pOther;
else
pNode->m_pParent->m_pRight = pOther;

// put our node on the other's left node
pOther->m_pLeft = pNode;
pNode->m_pParent = pOther;
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::RotateRight(Node* pNode)
{
	// these nodes must be valid
	assert(pNode);
	assert(pNode->m_pLeft);

	// Set the other node.  This is 'y' in the algorthm definition.
	Node* pOther = pNode->m_pLeft;

	// turn the other node's right subtree into our node's left subtree.
	pNode->m_pLeft = pOther->m_pRight;
	if (pOther->m_pRight)
		pOther->m_pRight->m_pParent = pNode;

	// link our node's parent to the other node's parent
	pOther->m_pParent = pNode->m_pParent;

	if (pNode->m_pParent == nullptr)  // check to see if this is the root node
		m_pRoot = pOther;
	else if (pNode == pNode->m_pParent->m_pRight)
		pNode->m_pParent->m_pRight = pOther;
	else
		pNode->m_pParent->m_pLeft = pOther;

	// put our node on the other's right node
	pOther->m_pRight = pNode;
	pNode->m_pParent = pOther;
}

template<class _KeyType, class _DataType>
inline typename RedBlackTree<_KeyType, _DataType>::NodeColor RedBlackTree<_KeyType, _DataType>::GetNodeColor(Node* pNode) const
{
	if (pNode)
		return pNode->m_color;
	return NodeColor::kBlack;
}

template<class _KeyType, class _DataType>
inline void RedBlackTree<_KeyType, _DataType>::FixupAfterDelete(Node* pNodeToFix)
{
	// pNodeToFix is x in the book

	if (!pNodeToFix)
		return;

	while (pNodeToFix != m_pRoot && GetNodeColor(pNodeToFix->m_pParent) == NodeColor::kBlack)
	{
		// If fixing node is a left child
		if (pNodeToFix == pNodeToFix->m_pParent->m_pLeft)
		{
			// Get sibling
			Node* pSibling = pNodeToFix->m_pParent->m_pRight;
			assert(pSibling);

			// Case 1: Sibling is red
			if (pSibling->m_color == NodeColor::kRed)
			{
				pSibling->m_color = NodeColor::kBlack;
				pNodeToFix->m_pParent->m_color = NodeColor::kRed;
				RotateLeft(pNodeToFix->m_pParent);
				pSibling = pNodeToFix->m_pParent->m_pRight;
			}

			if (pSibling) // I don't know if brother is guaranteed to exist
			{
				// Case 2: sibling is black, and both of sibling¡¯s children are black
				if ((pSibling->m_color == NodeColor::kBlack) &&
					(pSibling->m_pLeft->m_color == NodeColor::kBlack) &&
					(pSibling->m_pRight->m_color == NodeColor::kBlack))
				{
					pSibling->m_color = NodeColor::kRed;
					pNodeToFix = pNodeToFix->m_pParent;
				}
				else
				{
					// Case 3: sibling is black, sibling¡¯s left child is red, and sibling¡¯s right child is black
					if (pSibling->m_pRight->m_color == NodeColor::kBlack)
					{
						if (pSibling->m_pLeft)
							pSibling->m_pLeft->m_color = NodeColor::kBlack;

						pSibling->m_color = NodeColor::kRed;
						RotateRight(pSibling);
						pSibling = pNodeToFix->m_pParent->m_pRight;
					}

					// Case 4: sibling is black, and sibling¡¯s right child is red
					pSibling->m_color = pNodeToFix->m_pParent->m_color;
					pNodeToFix->m_pParent->m_color = NodeColor::kBlack;
					pSibling->m_pRight->m_color = NodeColor::kBlack;
					RotateLeft(pNodeToFix->m_pParent);
					pNodeToFix = m_pRoot;
				}
			}
		}

		// Fixing node is a right child
		else
		{
			// If fixing node is a right child
			if (pNodeToFix == pNodeToFix->m_pParent->m_pRight)
			{
				// Get sibling
				Node* pSibling = pNodeToFix->m_pParent->m_pLeft;
				assert(pSibling);

				// Case 1: Sibling is red
				if (pSibling->m_color == NodeColor::kRed)
				{
					pSibling->m_color = NodeColor::kBlack;
					pNodeToFix->m_pParent->m_color = NodeColor::kRed;
					RotateRight(pNodeToFix->m_pParent);
					pSibling = pNodeToFix->m_pParent->m_pLeft;
				}
				
				if (pSibling) // I don't know if brother is guaranteed to exist
				{
					// Case 2: sibling is black, and both of sibling¡¯s children are black
					if ((pSibling->m_color == NodeColor::kBlack) &&
						(pSibling->m_pLeft->m_color == NodeColor::kBlack) &&
						(pSibling->m_pRight->m_color == NodeColor::kBlack))
					{
						pSibling->m_color = NodeColor::kRed;
						pNodeToFix = pNodeToFix->m_pParent;
					}
					else
					{
						// Case 3: sibling is black, sibling¡¯s right child is red, and sibling¡¯s left child is black
						if (pSibling->m_pLeft->m_color == NodeColor::kBlack)
						{
							if (pSibling->m_pRight)
								pSibling->m_pRight->m_color = NodeColor::kBlack;

							pSibling->m_color = NodeColor::kRed;
							RotateLeft(pSibling);
							pSibling = pNodeToFix->m_pParent->m_pLeft;
						}

						// Case 4: sibling is black, and sibling¡¯s left child is red
						pSibling->m_color = pNodeToFix->m_pParent->m_color;
						pNodeToFix->m_pParent->m_color = NodeColor::kBlack;

						if (pSibling->m_pRight)
						{
							pSibling->m_pRight->m_color = NodeColor::kBlack;
							RotateRight(pNodeToFix->m_pParent);
							pNodeToFix = m_pRoot;
						}
					}
				}
			}
		}
	}

	// Force fixed node's color to black
	pNodeToFix->m_color = NodeColor::kBlack;
}

template<class _KeyType, class _DataType>
template<class Func>
inline void RedBlackTree<_KeyType, _DataType>::PreOrderWalkRecursive(Func&& func)
{
	RecursivePreOrderWalk(m_pRoot, std::forward<Func>(func));
}

template<class _KeyType, class _DataType>
template<class Func>
inline void RedBlackTree<_KeyType, _DataType>::PostOrderWalkRecursive(Func&& func)
{
	RecursivePostOrderWalk(m_pRoot, std::forward<Func>(func));
}

template<class _KeyType, class _DataType>
template<class Func>
inline void RedBlackTree<_KeyType, _DataType>::InOrderWalkRecursive(Func&& func)
{
	RecursiveInOrderWalk(m_pRoot, std::forward<Func>(func));
}

//---------------------------------------------------------------------------------------------------------------------
// Iterative Inorder tree walk
//---------------------------------------------------------------------------------------------------------------------
template<class _KeyType, class _DataType>
template<class Func>
inline void RedBlackTree<_KeyType, _DataType>::InOrderWalkIterative(Func&& func)
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
inline void RedBlackTree<_KeyType, _DataType>::RecursivePreOrderWalk(Node* pNode, Func&& func)
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
inline void RedBlackTree<_KeyType, _DataType>::RecursivePostOrderWalk(Node* pNode, Func&& func)
{
	if (pNode)
	{
		RecursivePostOrderWalk(pNode->m_pLeft, std::forward<Func>(func));
		RecursivePostOrderWalk(pNode->m_pRight, std::forward<Func>(func));
		func(const_cast<const KeyType&>(pNode->m_key), pNode->m_data);
	}
}

template<class _KeyType, class _DataType>
template<class Func>
inline void RedBlackTree<_KeyType, _DataType>::RecursiveInOrderWalk(Node* pNode, Func&& func)
{
	if (pNode)
	{
		// Go to find the smallest element
		RecursiveInOrderWalk(pNode->m_pLeft, std::forward<Func>(func));
		func(const_cast<const KeyType&>(pNode->m_key), pNode->m_data);
		RecursiveInOrderWalk(pNode->m_pRight, std::forward<Func>(func));
	}
}

}