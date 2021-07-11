#include "Utils/StructureManager.h"

#include "UnorderedArray.h"
#include "OrderedArray.h"
#include "StackArray.h"
#include "QueueArray.h"
#include "LinkedList.h"
#include "StackList.h"
#include "QueueList.h"
#include "CircularQueue.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "Graph.h"
#include "Utils/Helpers.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <vld.h>

bool Run()
{
	// Get array choice input
	std::string input = "";
	std::cin >> input;
	system("cls");

	// Check quit
	if (input == "q")
		return true;

	// Do work
	if (input == "0")
		UnorderedArray<int>::Test();
	else if (input == "1")
		OrderedArray<int>::Test();
	else if (input == "2")
		StackArray<char>::Test();
	else if (input == "3")
		QueueArray<int>::Test();
	else if (input == "4")
		LinkedList<int>::Test();
	else if (input == "5")
		StackList<int>::Test();
	else if (input == "6")
		QueueList<int>::Test();
	else if (input == "7")
		CircularQueue<int>::Test();
	else if (input == "8")
		HashTable<int, int>::Test();
	else if (input == "9")
		BinarySearchTree<int, int>::Test();
	else if (input == "10")
		RedBlackTree<int, int>::Test();
	else if (input == "11")
		Graph<char>::Test();

	return false;
}

int main()
{
	//bool shouldQuit = false;
	//StructureManager::Get().Init();

	//while (!shouldQuit)
	//{
	//	StructureManager::Get().ShowStructures();
	//	shouldQuit = Run();
	//}

	UnorderedArray<int> testArr;
	testArr.EmplaceBack(1);
	testArr.EmplaceBack(2);
	testArr.EmplaceBack(3);
	testArr.EmplaceBack(4);
	testArr.EmplaceBack(5);

	testArr.Print();

	testArr.PushFront(0);
	testArr.PushFront(-1);

	testArr.Print();

	return 0;
}

/*
* TODO list:
* - UnorderedArray: Iterator
* - QueueArray: Switch from Type* pArray to std::byte*
* - Implement Smart Ptrs
*/