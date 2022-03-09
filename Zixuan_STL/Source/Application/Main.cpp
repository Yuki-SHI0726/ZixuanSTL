#include "Tests/StructureManager.h"

#include "vector.h"
#include "OrderedArray.h"
#include "StackArray.h"
#include "QueueArray.h"
#include "list.h"
#include "StackList.h"
#include "QueueList.h"
#include "CircularQueue.h"
#include "unordered_map.h"
#include "BinarySearchTree.h"
#include "RedBlackTree.h"
#include "Graph.h"
#include "SmartPointers/unique_ptr.h"
#include "SmartPointers/shared_ptr.h"
#include "SmartPointers/weak_ptr.h"
#include "Helpers.h"
#include "Math/Vector3.h"
#include "Actor/Actor.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <vld.h>
#include <memory>
#include <functional>

bool RunInteractiveUnitTests()
{
	// Get array choice input
	std::string input{ "" };
	std::cin >> input;
	system("cls");

	// Check quit
	if (input == "q")
		return true;

	// Do work
	if (input == "0")
		zxstl::vector<int>::InteractiveTest();
	else if (input == "1")
		zxstl::OrderedArray<int>::Test();
	else if (input == "2")
		zxstl::StackArray<char>::Test();
	else if (input == "3")
		zxstl::QueueArray<int>::Test();
	else if (input == "4")
		zxstl::list<int>::Test();
	else if (input == "5")
		zxstl::StackList<int>::Test();
	else if (input == "6")
		zxstl::QueueList<int>::Test();
	else if (input == "7")
		zxstl::CircularQueue<int>::Test();
	else if (input == "8")
		zxstl::unordered_map<int, int>::Test();
	else if (input == "9")
		zxstl::BinarySearchTree<int, int>::Test();
	else if (input == "10")
		zxstl::RedBlackTree<int, int>::Test();
	else if (input == "11")
		zxstl::Graph<char>::Test();

	return false;
}

void Func(int data, int* pPtr)
{
	std::cout << "Foo" << std::endl;
}

int appmain()
{
	bool shouldQuit = false;
	StructureManager::Get().Init();

	while (!shouldQuit)
	{
		StructureManager::Get().ShowStructures();
		shouldQuit = RunInteractiveUnitTests();
	}

	return 0;
}

/*
* TODO list:
* - iterator_base
* - deque
* - Priority Queue
* - Graph: Jump Point Search
* - Vector3: refactor
*/



