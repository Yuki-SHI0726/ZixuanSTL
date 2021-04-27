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
#include "Actor/Actor.h"

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

	UnorderedArray<Actor<int, int>> unorderedArray;
	unorderedArray.EmplaceBack();
	unorderedArray.EmplaceBack();
	unorderedArray.EmplaceBack();
	unorderedArray.EmplaceBack();
	UnorderedArray<Actor<int, int>> otherUnorderedArray;
	otherUnorderedArray = std::move(unorderedArray);
	otherUnorderedArray.Print();

	return 0;
}