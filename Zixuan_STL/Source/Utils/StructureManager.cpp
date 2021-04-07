#include "StructureManager.h"

#include "UnorderedArray.h"
#include "OrderedArray.h"
#include "StackArray.h"
#include "QueueArray.h"

#include <iostream>
#include <conio.h>

void StructureManager::ShowStructures()
{
	system("cls");
	std::cout << "Choose an data structure to test" << std::endl;

	for(size_t i = 0; i < m_structures.size(); ++i)
	{
		std::cout << i << ") Test " << m_structures[i] << std::endl;
	}
	std::cout << "q) Quit" << std::endl;
}

char StructureManager::GetOperation(DataStructure structure)
{
	assert(m_operationMap[structure].size() > 0);

	std::cout << "\nChoose an operation" << std::endl;

	for (size_t i = 0; i < m_operationMap[structure].size(); ++i)
	{
		std::cout << i << ") " << m_operationMap[structure][i] << std::endl;
	}

	std::cout << "q) Quit" << std::endl << std::endl;

	char input = _getch();
	return input;
}

void StructureManager::Init()
{
	InitUnorderedArray();
	InitOrderedArray();
	InitStackArray();
	InitQueueArray();
	InitList();
	InitStackList();
	InitQueueList();
	InitCircularQueue();
	InitHashTable();
	InitBinarySearchTree();
	InitRedBlackTree();
	InitGraph();
}

void StructureManager::InitUnorderedArray()
{
	// Register structures
	m_structures.push_back("Unordered Array");

	// Init operation
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Push Back");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Clear");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Pop");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Erase");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Insertion Sort");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Selection Sort");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Bubble Sort");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Quick Sort");
	m_operationMap[DataStructure::kUnorderedArray].emplace_back("Shuffle");
}

void StructureManager::InitOrderedArray()
{
	// Register structures
	m_structures.push_back("Ordered Array");
								   
	// Init operation			   
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Push");
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Clear");
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Pop");
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Remove");
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Switch order");
	m_operationMap[DataStructure::kOrderedArray].emplace_back("Search");
}

void StructureManager::InitList()
{
	// Register
	m_structures.push_back("Linked list");

	// Init Operations
	m_operationMap[DataStructure::kList].emplace_back("Push front");
	m_operationMap[DataStructure::kList].emplace_back("Push back");
	m_operationMap[DataStructure::kList].emplace_back("Pop front");
	m_operationMap[DataStructure::kList].emplace_back("Pop back");
	m_operationMap[DataStructure::kList].emplace_back("Insert");
	m_operationMap[DataStructure::kList].emplace_back("Delete node by value");
	m_operationMap[DataStructure::kList].emplace_back("Reverse");
	m_operationMap[DataStructure::kList].emplace_back("Clear");
	m_operationMap[DataStructure::kList].emplace_back("Get Size");
	m_operationMap[DataStructure::kList].emplace_back("Delete node by index");
}

void StructureManager::InitStackArray()
{
	// Register structures
	m_structures.push_back("Stack Array");

	// Init operation			   
	m_operationMap[DataStructure::kStackArray].emplace_back("Push");
	m_operationMap[DataStructure::kStackArray].emplace_back("Clear");
	m_operationMap[DataStructure::kStackArray].emplace_back("Pop");
	m_operationMap[DataStructure::kStackArray].emplace_back("Top");
}

void StructureManager::InitStackList()
{
	// Register
	m_structures.push_back("Stack List");

	// Init operation	
	m_operationMap[DataStructure::kStackList].emplace_back("Push");
	m_operationMap[DataStructure::kStackList].emplace_back("Clear");
	m_operationMap[DataStructure::kStackList].emplace_back("Pop");
	m_operationMap[DataStructure::kStackList].emplace_back("Top");
}

void StructureManager::InitQueueArray()
{
	// Register structures
	m_structures.push_back("Queue Array");

	// Init operation			   
	m_operationMap[DataStructure::kQueueArray].emplace_back("Enqueue");
	m_operationMap[DataStructure::kQueueArray].emplace_back("Clear");
	m_operationMap[DataStructure::kQueueArray].emplace_back("Dequeue");
	m_operationMap[DataStructure::kQueueArray].emplace_back("Head");
	m_operationMap[DataStructure::kQueueArray].emplace_back("Tail");
}

void StructureManager::InitQueueList()
{
	// Register structures
	m_structures.push_back("Queue List");

	// Init operation			   
	m_operationMap[DataStructure::kQueueList].emplace_back("Enqueue");
	m_operationMap[DataStructure::kQueueList].emplace_back("Clear");
	m_operationMap[DataStructure::kQueueList].emplace_back("Dequeue");
	m_operationMap[DataStructure::kQueueList].emplace_back("Head");
	m_operationMap[DataStructure::kQueueList].emplace_back("Tail");
}

void StructureManager::InitCircularQueue()
{
	// Register structures
	m_structures.push_back("Circular queue");

	// Init operation			   
	m_operationMap[DataStructure::kCircularQueue].emplace_back("Enqueue");
	m_operationMap[DataStructure::kCircularQueue].emplace_back("Clear");
	m_operationMap[DataStructure::kCircularQueue].emplace_back("Dequeue");
	m_operationMap[DataStructure::kCircularQueue].emplace_back("Front");
	m_operationMap[DataStructure::kCircularQueue].emplace_back("Rear");
}

void StructureManager::InitHashTable()
{
	// Register structures
	m_structures.push_back("Hash Table");

	// Init operation			   
	m_operationMap[DataStructure::kHashTable].emplace_back("Insert");
	m_operationMap[DataStructure::kHashTable].emplace_back("Clear");
	m_operationMap[DataStructure::kHashTable].emplace_back("Delete");
	// m_operationMap[DataStructure::kHashTable].emplace_back("Find");
}

void StructureManager::InitBinarySearchTree()
{
	// Register structures
	m_structures.push_back("Binary Search Tree");

	// Init operation			   
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Insert");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("InOrder Walk");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("PreOrder Walk");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("PostOrder Walk");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Find Minimum");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Find Maximum");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Search");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Clear");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Delete");
	m_operationMap[DataStructure::kBinarySearchTree].emplace_back("Change key");
}

void StructureManager::InitRedBlackTree()
{	
	// Register structures
	m_structures.push_back("Red Black Tree");

	// Init operation			   
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Insert");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("InOrder Walk");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("PreOrder Walk");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("PostOrder Walk");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Find Minimum");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Find Maximum");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Search");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Clear");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Delete");
	m_operationMap[DataStructure::kRedBlackTree].emplace_back("Change key");
}

void StructureManager::InitGraph()
{
	// Register structures
	m_structures.push_back("Graph");

	// Init operation			   
	m_operationMap[DataStructure::kGraph].emplace_back("Build Directed Test Graph from \"Introduction to algorithms - 3rd\", page 590");
	m_operationMap[DataStructure::kGraph].emplace_back("Build Undirected Test Graph \"Introduction to algorithms - 3rd\", page 590");
	m_operationMap[DataStructure::kGraph].emplace_back("Get out degree");
	m_operationMap[DataStructure::kGraph].emplace_back("Get in degree");
	m_operationMap[DataStructure::kGraph].emplace_back("Breadth First Search (Remember to build a graph first)");
	m_operationMap[DataStructure::kGraph].emplace_back("Depth First Search (Remember to build a graph first)");
	m_operationMap[DataStructure::kGraph].emplace_back("Transpose Graph");
	m_operationMap[DataStructure::kGraph].emplace_back("Get Distance between two nodes");
	m_operationMap[DataStructure::kGraph].emplace_back("Print Shortest Path (Remember to run a search first)");
	m_operationMap[DataStructure::kGraph].emplace_back("Dijkstra (Remember to run a search first)");
}
