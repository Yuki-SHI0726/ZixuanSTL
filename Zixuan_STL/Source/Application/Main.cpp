#include "Utils/StructureManager.h"

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

// STL

bool Run()
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
		vector<int>::InteractiveTest();
	else if (input == "1")
		OrderedArray<int>::Test();
	else if (input == "2")
		StackArray<char>::Test();
	else if (input == "3")
		QueueArray<int>::Test();
	else if (input == "4")
		list<int>::Test();
	else if (input == "5")
		StackList<int>::Test();
	else if (input == "6")
		QueueList<int>::Test();
	else if (input == "7")
		CircularQueue<int>::Test();
	else if (input == "8")
		unordered_map<int, int>::Test();
	else if (input == "9")
		BinarySearchTree<int, int>::Test();
	else if (input == "10")
		RedBlackTree<int, int>::Test();
	else if (input == "11")
		Graph<char>::Test();

	return false;
}

void Func(int data, int* pPtr)
{
	std::cout << "Foo" << std::endl;
}

int main()
{
	// stl
	{
		std::vector<int> stlVec;
		stlVec.emplace_back(0);
		stlVec.emplace_back(1);
		stlVec.emplace_back(2);

		for (std::vector<int>::iterator iterator = stlVec.begin(); iterator != stlVec.end(); ++iterator)
			std::cout << *iterator << std::endl;
	}
	std::cout << "---------------------------------------------\n";
	// zxstl
	{
		vector<int> zxVec;
		zxVec.emplace_back(0);
		zxVec.emplace_back(1);
		zxVec.emplace_back(2);

		for (vector<int>::iterator iterator = zxVec.begin(); iterator != zxVec.end(); ++iterator)
			std::cout << *iterator << std::endl;
	}

	return 0;
}

/*
* TODO list:
* - deque
* - Priority Queue
* - Graph: Jump Point Search
* - Vector3: refactor
*/



//bool shouldQuit = false;
//StructureManager::Get().Init();

//while (!shouldQuit)
//{
//	StructureManager::Get().ShowStructures();
//	shouldQuit = Run();
//}
//Actor* pActor = new Actor(10);