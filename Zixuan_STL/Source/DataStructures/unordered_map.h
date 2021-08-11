#pragma once

#include "Utils/StructureManager.h"
#include "list.h"
#include "vector.h"

//--------------------------------------------------------------------------------------------------------------------
// Custom templated chained hash table implementation
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
class unordered_map
{
	using Table = std::pair<size_t, list<Type>*>;

private:
	Table* m_pTable;
	size_t m_capacity;
	// Using m_pValues's size

public:
	unordered_map();
	unordered_map(size_t capacity);
	~unordered_map();

	void Insert(Key key, const Type& data);
	void Delete(Key key);
	//bool Find(Key key, const Value& outData) const;
	void Print() const;
    void Clear();

	static void Test();

private:
	size_t Hash(Key key) const;
	void Destroy();
};

//--------------------------------------------------------------------------------------------------------------------
// Ctor
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline unordered_map<Key, Type>::unordered_map()
	: m_capacity{ kInitialCapacity }
{
	m_pTable = new Table[m_capacity];
	for (size_t i = 0; i < m_capacity; ++i)
	{
		m_pTable[i].second = new list<Type>;
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Ctor with initial capacity
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline unordered_map<Key, Type>::unordered_map(size_t capacity)
	: m_capacity{ capacity }
{
	m_pTable = new Table[m_capacity];
	for (size_t i = 0; i < m_capacity; ++i)
	{
		m_pTable[i].second = new list<Type>;
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline unordered_map<Key, Type>::~unordered_map()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------------------------------
// insert x at the head of list T[Hash(key)]
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Insert(Key key, const Type& data)
{
	size_t hashKey = Hash(key);
	m_pTable[hashKey].first = hashKey;
	m_pTable[hashKey].second->PushBack(data);
}

//--------------------------------------------------------------------------------------------------------------------
// delete x from the list T[Hash(key)]
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Delete(Key key)
{
	size_t hashKey = Hash(key);
	m_pTable[hashKey].second->PopBack();
	if (m_pTable[hashKey].second->Empty())
	{
		delete m_pTable[hashKey].second;
		m_pTable[hashKey].second = nullptr;
	}
}

//template<typename Key, typename Value>
//inline bool HashTable<Key, Value>::Find(Key key, const Value& outData) const
//{
//	return m_pTable[Hash(key)].second->SearchByVal(outData);
//}

//--------------------------------------------------------------------------------------------------------------------
// Print key value pair
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Print() const
{
	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (!m_pTable[i].second->Empty())
		{
			std::cout << "Key: " << m_pTable[i].first << ", Value: ";
			m_pTable[i].second->Print();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Delete current content
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Clear()
{
	Destroy();
}

template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	size_t i = 0;       // Used as capacity and index
	Key key = 0;		// Key in hash table
	Type value = 0;		// Be used as value

	// Create list

	unordered_map<Key, Type> hashTable(9);
#if _DEBUG
	hashTable.Insert(5,  0);	// Hashed 5
	hashTable.Insert(28, 0);	// Hashed 1
	hashTable.Insert(19, 0);	// Hashed 1, Collision occurs at 1, value added by chainning
	hashTable.Insert(15, 0);	// Hashed 6
	hashTable.Insert(20, 0);	// Hashed 2
	hashTable.Insert(33, 0);	// Hashed 6, Collision occurs at 6, value added by chainning
	hashTable.Insert(12, 0);	// Hashed 3
	hashTable.Insert(17, 0);	// Hashed 8
	hashTable.Insert(10, 0);	// Hashed 1, Collision occurs at 1, value added by chainning
	
#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		hashTable.Print();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kHashTable);

		// Do work
		switch (operationInput)
		{
		case '0':
			std::cout << "Enter key: ";
			std::cin >> key;
			std::cout << "Enter inserting value: ";
			std::cin >> value;
			hashTable.Insert(key, value);
			break;

		case '1':
			hashTable.Clear();
			break;

		case '2':
			std::cout << "Enter key: ";
			std::cin >> key;
			hashTable.Delete(key);
			break;

		//case '4':
		//	std::cout << "Enter key: ";
		//	std::cin >> key;
		//	std::cout << "Enter inserting value: ";
		//	std::cin >> value;
		//	hashTable.Find(key, value);
		//	break;

		case 'q':            shouldQuit = true;            break;
		}

		system("cls");
	}
}

//--------------------------------------------------------------------------------------------------------------------
// The hash function
//--------------------------------------------------------------------------------------------------------------------
template<typename Key, typename Type>
inline size_t unordered_map<Key, Type>::Hash(Key key) const
{
	return static_cast<size_t>(key % m_capacity);
}

template<typename Key, typename Type>
inline void unordered_map<Key, Type>::Destroy()
{
	for (size_t i = 0; i < m_capacity; ++i)
	{
		if (m_pTable[i].second)
		{
			delete m_pTable[i].second;
			m_pTable[i].second = nullptr;
		}
	}

	if (m_pTable)
	{
		delete[] m_pTable;
		m_pTable = nullptr;
	}
}
