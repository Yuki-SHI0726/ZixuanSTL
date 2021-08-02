#pragma once

#include <unordered_map>
#include <vector>

static constexpr size_t kInitialCapacity = 10;   // Default capacity for default ctor
static constexpr size_t kExpandMultiplier = 2; // If the array is full, multiply the capacity to this number and create a new array

enum class DataStructure : size_t
{
	kUnorderedArray,
	kOrderedArray,
	kList,
	kStackArray,
	kStackList,
	kQueueArray,
	kQueueList,
	kCircularQueue,
	kHashTable,
	kBinarySearchTree,
	kRedBlackTree,
	kGraph,

	kNum
};

class StructureManager
{
private:
	std::unordered_map<DataStructure, std::vector<const char*>> m_operationMap;		// Operations
	std::vector<const char*> m_structures;					// Structures names

public:
	static StructureManager& Get()
	{
		static StructureManager instance;
		return instance;
	}

	void Init();
	void ShowStructures();
	char GetOperation(DataStructure structure);

private:
	StructureManager() = default;

	void InitUnorderedArray();
	void InitOrderedArray();
	void InitList();
	void InitStackArray();
	void InitStackList();
	void InitQueueArray();
	void InitQueueList();
	void InitCircularQueue();
	void InitHashTable();
	void InitBinarySearchTree();
	void InitRedBlackTree();
	void InitGraph();
};
