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
#include "SmartPointers/UniquePtr.h"
#include "SmartPointers/SharedPtr.h"
#include "SmartPointers/WeakPtr.h"
#include "Helpers.h"
#include "Math/Vector3.h"
#include "Actor/Actor.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <vld.h>
#include <memory>
#include <functional>

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

void Func(int data, int* pPtr)
{
	std::cout << "Foo" << std::endl;
}

int main()
{
	// Weak pointer test
	WeakPtr<Actor> pWeakFoo;
	{
		std::cout << pWeakFoo.UseCount() << std::endl;
		{
			// Shared pointer test
			SharedPtr<Actor> pBar = SharedPtr<Actor>::Make(10);

			pWeakFoo = pBar;

			if (!pWeakFoo.Expired())
				pWeakFoo.Lock()->Work(Func);

			std::cout << pWeakFoo.UseCount() << std::endl;
		}

		std::cout << pWeakFoo.UseCount() << std::endl;
	}
	if (!pWeakFoo.Expired())
		pWeakFoo.Lock()->Work(Func);

	std::cout << pWeakFoo.UseCount() << std::endl;

	// Unique pointer test
	Actor* pActor = new Actor(10);
	UniquePtr<Actor> pUniqueActor = pActor;
	pUniqueActor->Work(Func);

	return 0;
}

/*
* TODO list:
* - UnorderedArray: Iterator
* - Graph: Jump Point Search
* - Vector3: refactor
* - Priority Queue
*/



//bool shouldQuit = false;
//StructureManager::Get().Init();

//while (!shouldQuit)
//{
//	StructureManager::Get().ShowStructures();
//	shouldQuit = Run();
//}
//Actor* pActor = new Actor(10);