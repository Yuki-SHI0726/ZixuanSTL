#pragma once

// 1 = shortest
// 2 = longest
#define PATH_CHOICE 1

#include "Utils/StructureManager.h"
#include "Utils/Vector2.h"

#include <vector>
#include <map>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>

//--------------------------------------------------------------------------------------------------------------------
// Custom Graph class supporting the following operations
//	- BFS
//	- DFS
//	- Dijkstra's Algorithm
//	- A*
//	- Transpose
//--------------------------------------------------------------------------------------------------------------------
template <class Type>
class Graph
{
public:
	// Alias
	using NodeId = size_t;
	using Dist = float;
	using AdjacencyList = std::vector<std::map<NodeId, Dist>>;
	static constexpr NodeId kInvalidNodeId = std::numeric_limits<NodeId>::max();
	static constexpr Dist kStartDist = std::numeric_limits<Dist>::max();

private:
	struct GraphVertex
	{
		Type m_data;
		NodeId m_id;

		// Graph search data
		NodeId m_prev;
		Dist m_distance;
		bool m_closed;

		GraphVertex(const Type& data, NodeId id)
			: m_data(data)
			, m_prev{ kInvalidNodeId }
			, m_id{ id }
			, m_distance{ kStartDist }
			, m_closed{ false }
		{
		}

		constexpr void ResetSearchData()
		{
			m_prev = kInvalidNodeId;
			m_distance = kStartDist;
			m_closed = false;
		}
	};

	std::vector<GraphVertex> m_vertices;
	AdjacencyList m_adjacencyList;
	inline static NodeId s_id = 0;

public:
	// Adding
	constexpr NodeId AddNode(const Type& data);
	constexpr void AddEdge(NodeId fromId, NodeId toId, Dist weight = 1.0f);

	// Compute degree
	constexpr size_t ComputeInDegree(NodeId nodeId);
	constexpr size_t ComputeOutDegree(NodeId nodeId);

	// Transpose
	constexpr void TransposeGraph();

	// Search
	constexpr bool BreadthFirstSearchFind(NodeId startNodeId, NodeId endNodeId);	
	template <class Func> constexpr void BreadthFirstSearch(NodeId startNodeId, Func&& func);
	template <class Func> constexpr void DepthFirstSearchIter(NodeId startNodeId, Func&& func);
	template <class Func> constexpr void DepthFirstSearchRecur(NodeId startNodeId, Func&& func);

	// Path-finding
	template <class Func> constexpr void RunDijkstraSearch(NodeId startNodeId, Func&& func);
	template <class Func> constexpr std::vector<NodeId> RunDijkstraFind(NodeId startNodeId, NodeId endNodeId, Func&& func);
	template <class Func> constexpr void AStar(NodeId startNodeId, NodeId destNodeId, Func&& func);
	 
	// Printing
	constexpr void PrintShortestPath(NodeId node);
	constexpr void PrintGraph();

	// Getters
	constexpr Dist GetDist(NodeId fromId, NodeId toId) const;

	// tests
	constexpr void BuildUndirectedUnweightedGraph();
	constexpr void BuildDirectedWeightedGraph();
	constexpr void BuildDirectedUnweightedGraph();
	void static Test();

private:
	constexpr void DestroyGraph();
	template <class Func> constexpr void InternalDepthFirstSearch(NodeId nodeId, Func&& func);
	constexpr bool Relax(NodeId sourceNodeId, NodeId destNodeId, Dist weight);
	Dist Heuristic(NodeId sourceNodeId, NodeId destNodeId) const;
	constexpr Vector2 GetXYFromIndex(NodeId id) const;	// Only works if it's a grid-like graph
};

//--------------------------------------------------------------------------------------------------------------------
// Clear current graph
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void Graph<Type>::DestroyGraph()
{
	m_vertices.clear();
	m_adjacencyList.clear();
}

template<class Type>
inline constexpr bool Graph<Type>::Relax(NodeId sourceNodeId, NodeId destNodeId, Dist weight)
{
#if PATH_CHOICE == 1
	if (m_vertices[destNodeId].m_distance > m_vertices[sourceNodeId].m_distance + weight)
	{
		m_vertices[destNodeId].m_distance = m_vertices[sourceNodeId].m_distance + weight;
		m_vertices[destNodeId].m_prev = sourceNodeId;
		return true;
	}
#elif PATH_CHOICE == 2
	if (m_vertices[destNodeId].m_distance == kStartDist || m_vertices[destNodeId].m_distance < m_vertices[sourceNodeId].m_distance + weight)
	{
		m_vertices[destNodeId].m_distance = m_vertices[sourceNodeId].m_distance + weight;
		m_vertices[destNodeId].m_prev = sourceNodeId;
		return true;
	}
#endif

	return false;
}

//--------------------------------------------------------------------------------------------------------------------
// calculates the distance between the two nodes' world coordinates
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline typename Graph<Type>::Dist Graph<Type>::Heuristic(NodeId sourceNodeId, NodeId destNodeId) const
{
	Vector2 startPos = GetXYFromIndex(sourceNodeId);
	float startX = startPos.x;
	float startY = startPos.y;

	Vector2 destPos = GetXYFromIndex(destNodeId);
	float endX = destPos.x;
	float endY = destPos.y;

	return Dist(std::fabs(startX - endX) + std::fabs(startY - endY));
}

//--------------------------------------------------------------------------------------------------------------------
// Only works if it's a grid-like graph
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr Vector2 Graph<Type>::GetXYFromIndex(NodeId id) const
{
	auto kWorldWidth = 1;
	auto kWorldWidth = 2;
	float y = static_cast<float>(id / kWorldWidth);
	float x = static_cast<float>(id % kWorldWidth);
	return Vector2(x, y);
}

//--------------------------------------------------------------------------------------------------------------------
// Add data as node to vertices vector and adjacency list, return added node's index
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr typename Graph<Type>::NodeId Graph<Type>::AddNode(const Type& data)
{
	m_vertices.emplace_back(data, s_id);
	++s_id;
	m_adjacencyList.emplace_back();
	assert(m_vertices.size() == m_adjacencyList.size());
	return m_vertices.size() - 1;
}

//--------------------------------------------------------------------------------------------------------------------
// Add an edge between two nodes in one direction with the weight
// weight is default to 1
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
constexpr void Graph<Type>::AddEdge(NodeId fromId, NodeId toId, Dist weight)
{
	assert(fromId < m_vertices.size() && toId < m_vertices.size());
	m_adjacencyList[fromId].emplace(toId, weight);
}

//--------------------------------------------------------------------------------------------------------------------
// Get total in degree count from a vertex
// Time: O(V + log(E))
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr size_t Graph<Type>::ComputeInDegree(NodeId nodeId)
{
	assert(nodeId < m_vertices.size());

	size_t count = 0;

	// Loop through the adjacency list
	// O(V)
	for (size_t i = 0; i < m_vertices.size(); ++i)
	{
		// Collect all nodes that pointing to the target node
		// O(log(E))
		if (m_adjacencyList[i].find(nodeId) != m_adjacencyList[i].end())
			++count;
	}

	return count;
}

//--------------------------------------------------------------------------------------------------------------------
// Get total out degree count from a vertext
// O(1)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr size_t Graph<Type>::ComputeOutDegree(NodeId nodeId)
{
	assert(nodeId < m_vertices.size());
	return m_adjacencyList[nodeId].size();
}

//--------------------------------------------------------------------------------------------------------------------
// Transpose this graph, reverse the edges
// Time: O(V + E)
// Space: O(E)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::TransposeGraph()
{
	assert(!m_vertices.empty());

	// Grab an empty adjacencylist as the result 
	// Space: O(E)
	AdjacencyList transposedAdjacencyList;

	// Resize it to current adjacencylist
	// O(V)
	transposedAdjacencyList.resize(m_adjacencyList.size());

	// Walk through the adjacency list
	// O(E)
	for (NodeId nodeId = 0; nodeId < m_adjacencyList.size(); ++nodeId)
	{
		// For each adjacent node's id in the list, add the current node id to adjacent node's list
		for (const auto& [kAdjacentNodeId, kWeight] : m_adjacencyList[nodeId])
			transposedAdjacencyList[kAdjacentNodeId].emplace(nodeId, kWeight);
	}

	// O(1)
	m_adjacencyList.swap(transposedAdjacencyList);
}

//--------------------------------------------------------------------------------------------------------------------
// Assignment 12.2 breadth first search finding
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr bool Graph<Type>::BreadthFirstSearchFind(NodeId startNodeId, NodeId endNodeId)
{
	// reset all the search data
	// O(|V|)
	for (GraphVertex& vertex : m_vertices)
		vertex.ResetSearchData();

	// set the search data for the starting vertex
	m_vertices[startNodeId].m_distance = 0;
	m_vertices[startNodeId].m_closed = true;

	// Declare the open set, which is the queue of nodes we need to visit, and push our starting node there.
	std::queue<NodeId> openSet;
	openSet.push(startNodeId);

	// Search begins here, quit search if openSet is empty
	while (!openSet.empty())
	{
		// Pop the next vertex off of the queue
		NodeId currentNodeId = openSet.front();
		openSet.pop();

		// Return if found end node id
		if (currentNodeId == endNodeId)
			return true;

		// For each neighbor of the current node
		// O(E)
		for (const auto& [kNeighborId, kWeight] : m_adjacencyList[currentNodeId])
		{
			// If we haven't seen this neighbor before
			if (!m_vertices[kNeighborId].m_closed)
			{
				// Set the distance and previous node
				m_vertices[kNeighborId].m_distance = m_vertices[currentNodeId].m_distance + 1;
				m_vertices[kNeighborId].m_prev = currentNodeId;

				// close the node and add to the open set
				m_vertices[kNeighborId].m_closed = true;
				openSet.push(kNeighborId);
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------
// Recursively print shortest path to the target node
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::PrintShortestPath(NodeId nodeId)
{
	if (nodeId == kInvalidNodeId)
		return;

	assert(nodeId < m_vertices.size());
	PrintShortestPath(m_vertices[nodeId].m_prev);
	std::cout << m_vertices[nodeId].m_data << " -> ";
}

//--------------------------------------------------------------------------------------------------------------------
// Print current graph's adjacency list
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::PrintGraph()
{
	std::cout << "Graph Adjacency list: " << std::endl;
	for (NodeId nodeId = 0; nodeId < m_vertices.size(); ++nodeId)
	{
		std::cout << nodeId << "->";
		for (NodeId adjacentNodeId = 0; adjacentNodeId < m_adjacencyList[nodeId].size(); ++adjacentNodeId)
		{
			auto itr = m_adjacencyList[nodeId].begin();
			std::advance(itr, adjacentNodeId);
			std::cout << itr->first << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------
// Return Distance fromId to toId
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr typename Graph<Type>::Dist Graph<Type>::GetDist(NodeId fromId, NodeId toId) const
{
	assert(m_adjacencyList[fromId].find(toId) != m_adjacencyList[fromId].end());
	return m_adjacencyList[fromId].at(toId);
}

//--------------------------------------------------------------------------------------------------------------------
// Build an undirected test graph from "Introduction to algorithms-3rd", page 590
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::BuildUndirectedUnweightedGraph()
{
	DestroyGraph();
	m_vertices.reserve(5);

	AddNode(static_cast<Type>(1));
	AddNode(static_cast<Type>(2));
	AddNode(static_cast<Type>(3));
	AddNode(static_cast<Type>(4));
	AddNode(static_cast<Type>(5));

	// Node 1 is connected to nodes 2 and 5
	AddEdge(0, 1);
	AddEdge(0, 4);

	// Node 2 is connected to nodes 1, 5, 4, and 3
	AddEdge(1, 0);
	AddEdge(1, 4);
	AddEdge(1, 3);
	AddEdge(1, 2);

	// Node 3 is connected to nodes 2 and 4
	AddEdge(2, 1);
	AddEdge(2, 3);

	// Node 4 is connected to nodes 5, 2, and 3
	AddEdge(3, 4);
	AddEdge(3, 1);
	AddEdge(3, 2);

	// Node 5 is connected to nodes 1, 2, and 4
	AddEdge(4, 0);
	AddEdge(4, 1);
	AddEdge(4, 3);
}

//--------------------------------------------------------------------------------------------------------------------
// Build an directed test graph from Assets/DijkstraGraph.PNG
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::BuildDirectedWeightedGraph()
{
	DestroyGraph();
	m_vertices.reserve(6);

	AddNode(static_cast<Type>('S'));
	AddNode(static_cast<Type>('T'));
	AddNode(static_cast<Type>('Y'));
	AddNode(static_cast<Type>('X'));
	AddNode(static_cast<Type>('Z'));

	// Node S is connected to nodes T and Y
	AddEdge(0, 1, 0.9f);
	AddEdge(0, 2, 0.6f);

	// Node T is connected to node Y, X
	AddEdge(1, 2, 0.7f);
	AddEdge(1, 3, 0.7f);

	// Node Y is connected to node T, X, Z
	AddEdge(2, 1, 0.5f);
	AddEdge(2, 3, 0.9f);
	AddEdge(2, 4, 0.8f);

	// Node X is connected to node Z
	AddEdge(3, 4, 0.4f);

	// Node Z is connected to nodes S, X
	AddEdge(4, 0, 0.6f);
	AddEdge(4, 3, 0.2f);
}

//--------------------------------------------------------------------------------------------------------------------
// Build an undirected test graph from "Introduction to algorithms-3rd", page 590
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
inline constexpr void Graph<Type>::BuildDirectedUnweightedGraph()
{
	DestroyGraph();
	m_vertices.reserve(6);

	AddNode(static_cast<Type>(1));
	AddNode(static_cast<Type>(2));
	AddNode(static_cast<Type>(3));
	AddNode(static_cast<Type>(4));
	AddNode(static_cast<Type>(5));
	AddNode(static_cast<Type>(6));

	// Node 1 is connected to nodes 2 and 4
	AddEdge(0, 1);
	AddEdge(0, 3);

	// Node 2 is connected to node 5
	AddEdge(1, 4);

	// Node 3 is connected to node 5 and 6
	AddEdge(2, 4);
	AddEdge(2, 5);

	// Node 4 is connected to node 2
	AddEdge(3, 1);

	// Node 5 is connected to nodes 4
	AddEdge(4, 3);

	// Node 6 is connected to itself
	AddEdge(5, 5);
}

template<class Type>
inline void Graph<Type>::Test()
{
	// Variables for testing
	bool shouldQuit = false;
	NodeId nodeId = kInvalidNodeId;

	Graph<Type> graph;

#if _DEBUG
	graph.BuildDirectedWeightedGraph();
#else

#endif

	// Loop work
	while (!shouldQuit)
	{
		// Print array
		graph.PrintGraph();

		// Get input
		char operationInput = StructureManager::Get().GetOperation(DataStructure::kGraph);

		// Do work
		switch (operationInput)
		{
		case '0':
			graph.BuildDirectedWeightedGraph();
			break;

		case '1':
			graph.BuildUndirectedUnweightedGraph();
			break;

		case '2':
			std::cout << "Enter the node index you want to compute: ";
			std::cin >> nodeId;
			std::cout << "Node " << nodeId << "'s Out-Degree is " << graph.ComputeOutDegree(nodeId) << std::endl << std::endl;
			system("pause");
			break;

		case '3':
			std::cout << "Enter the node index you want to compute: ";
			std::cin >> nodeId;
			std::cout << "Node " << nodeId << "'s In-Degree is " << graph.ComputeInDegree(nodeId) << std::endl << std::endl;
			system("pause");
			break;

		case '4':
		{
			std::cout << "Select the start vertex: ";
			NodeId startVertexId = kInvalidNodeId;
			std::cin >> startVertexId;

			std::cout << "Select the end vertex: ";
			NodeId endVertexId = kInvalidNodeId;
			std::cin >> endVertexId;

			auto findLambda = [endVertexId](NodeId nodeId, const Type& data)
			{
				if (nodeId == (endVertexId - 1))
				{
					std::cout << "Found!" << std::endl;
					return;
				}
			};

			graph.BreadthFirstSearch(startVertexId - 1, findLambda);

			system("pause");
			break;
		}
		case '5':
		{
			std::cout << "Select the start vertex: ";
			std::cin >> nodeId;
			graph.DepthFirstSearchRecur(nodeId, [](const Type& data) {std::cout << data << " -> "; });
			system("pause");
			break;
		}
		case '6':
			graph.TransposeGraph();
			break;

		case '7':
		{
			std::cout << "Select the start vertex: ";
			NodeId startVertexId = kInvalidNodeId;
			std::cin >> startVertexId;

			std::cout << "Select the end vertex: ";
			NodeId endVertexId = kInvalidNodeId;
			std::cin >> endVertexId;

			std::cout << graph.GetDist(startVertexId, endVertexId) << std::endl;
			system("pause");
			break;
		}
		case '8':
		{
			std::cout << "Select the end vertex index:\n";
			std::cin >> nodeId;
			graph.PrintShortestPath(nodeId);
			system("pause");
			break;
		}
		case '9':
			std::cout << "Select the start vertex: ";
			std::cin >> nodeId;
			graph.RunDijkstraSearch(nodeId, [](NodeId id, const Type& data) {std::cout << "Id: " << id << ", Data: " << data << " -> "; });
			system("pause");
			break;

		case 'q':
			shouldQuit = true;
			break;
		}

		system("cls");
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Run breadth first search with start node
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr void Graph<Type>::BreadthFirstSearch(NodeId startNodeId, Func&& func)
{
	// reset all the search data
	// O(|V|)
	for (GraphVertex& vertex : m_vertices)
		vertex.ResetSearchData();

	// set the search data for the starting vertex
	m_vertices[startNodeId].m_distance = 0;
	m_vertices[startNodeId].m_closed = true;

	// Declare the open set, which is the queue of nodes we need to visit, and push our starting node there.
	std::queue<NodeId> openSet;
	openSet.emplace(startNodeId);

	// Search begins here, quit search if openSet is empty
	while (!openSet.empty())
	{
		// Pop the next vertex off of the queue
		const NodeId kCurrentNodeId = openSet.front();
		openSet.pop();

		// Perform lambda function here
		func(kCurrentNodeId, m_vertices[kCurrentNodeId].m_data);

		// For each neighbor of the current node
		// O(E)
		for (const auto& [kNeighbor, kWeight] : m_adjacencyList[kCurrentNodeId])
		{
			// If we haven't seen this neighbor before
			if (!m_vertices[kNeighbor].m_closed)
			{
				// Set the distance and previous node
				m_vertices[kNeighbor].m_distance = m_vertices[kCurrentNodeId].m_distance + 1;
				m_vertices[kNeighbor].m_prev = kCurrentNodeId;

				// close the node and add to the open set
				m_vertices[kNeighbor].m_closed = true;
				openSet.emplace(kNeighbor);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Iterative depth first search with a stack
// O(V + E)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr void Graph<Type>::DepthFirstSearchIter(NodeId startNodeId, Func&& func)
{
	// O(V)
	for (GraphVertex& node : m_vertices)
		node.ResetSearchData();

	// set the search data for the starting vertex
	m_vertices[startNodeId].m_distance = 0;
	m_vertices[startNodeId].m_closed = true;

	// Declare the open stack, which is the stack of nodes we need to visit, and push our starting node there.
	std::stack<NodeId> openSet;
	openSet.emplace(startNodeId);

	// Search begins here, quit search if openSet is empty
	while (!openSet.empty())
	{
		// Pop the next vertex off of the stack
		const NodeId kCurrentNodeId = openSet.top();
		openSet.pop();

		// Perfrom func here
		func(m_vertices[kCurrentNodeId].m_data);

		// For each neighbor of the current node
		// O(E)
		for (const auto& [kTargetId, kWeight] : m_adjacencyList[kCurrentNodeId])
		{
			if (!m_vertices[kTargetId].m_closed)
			{
				// set the distance and previous node
				m_vertices[kTargetId].m_distance = m_vertices[kCurrentNodeId].m_distance + 1;
				m_vertices[kTargetId].m_prev = kCurrentNodeId;

				// close the node and add to the open set
				m_vertices[kTargetId].m_closed = true;
				openSet.emplace(kTargetId);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Recursive Depth First Search interface
// O(V + E)
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr void Graph<Type>::DepthFirstSearchRecur(NodeId startNodeId, Func&& func)
{
	// reset all the search data
	// O(V)
	for (GraphVertex& node : m_vertices)
		node.ResetSearchData();

	// Internal search
	InternalDepthFirstSearch(startNodeId, std::forward<Func>(func));
}

//--------------------------------------------------------------------------------------------------------------------
// Dijkstra Algorithm searching
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr void Graph<Type>::RunDijkstraSearch(NodeId startNodeId, Func&& func)
{
	// initialize single source
	for (GraphVertex& node : m_vertices)
		node.ResetSearchData();

	// The lambda that priority queue is using
	auto openSetPriorityFunc = [this](NodeId left, NodeId right) -> bool
	{
#if PATH_CHOICE == 1
		return m_vertices[left].m_distance > m_vertices[right].m_distance;
#elif PATH_CHOICE == 2
		return m_vertices[left].m_distance < m_vertices[right].m_distance;
#endif
	};

	// Declare the open set, which is all the nodes we have yet to expand.
	using OpenSet = std::priority_queue<NodeId, std::vector<NodeId>, decltype(openSetPriorityFunc)>;
	OpenSet openSet(openSetPriorityFunc);

	// add the start vertex and set it's dist to 0
	m_vertices[startNodeId].m_distance = 0.0f;
	openSet.emplace(startNodeId);

	// keep going as long as there's anything in the open set
	while (!openSet.empty())
	{
		// grab the best weight
		NodeId nodeId = openSet.top();
		openSet.pop();

		// Perform func
		func(nodeId, m_vertices[nodeId].m_data);

		// Add this vertex to the closed set.  
		// We only use it to ask whether or not a node is in the closed set, so as an optimization we just store its membership as a bool.
		m_vertices[nodeId].m_closed = true;

		// for each neighbor
		for (const auto& [kNeighborNodeId, kWeight] : m_adjacencyList[nodeId])
		{
			// Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
			if (m_vertices[kNeighborNodeId].m_closed)
				continue;

			// Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
			// to be the case the first time the node is seen because the distance is set to infinity, so this path is 
			// guaranteed to be better.
			if (Relax(nodeId, kNeighborNodeId, kWeight))
				openSet.emplace(kNeighborNodeId);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Dijkstra Algorithm finding
// Returns a vector of nodeId that passed through 
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr std::vector<typename Graph<Type>::NodeId> Graph<Type>::RunDijkstraFind(NodeId startNodeId, NodeId endNodeId, Func&& func)
{
	// Data
	std::vector<NodeId> path;
	bool hasFoundTarget = false;

	// initialize single source
	for (GraphVertex& node : m_vertices)
		node.ResetSearchData();

	// The lambda that priority queue is using
	auto openSetPriorityFunc = [this](NodeId left, NodeId right) -> bool
	{
#if PATH_CHOICE == 1
		return m_vertices[left].m_distance > m_vertices[right].m_distance;
#elif PATH_CHOICE == 2
		return m_vertices[left].m_distance < m_vertices[right].m_distance;
#endif
	};

	// Declare the open set, which is all the nodes we have yet to expand.
	using OpenSet = std::priority_queue<NodeId, std::vector<NodeId>, decltype(openSetPriorityFunc)>;
	OpenSet openSet(openSetPriorityFunc);

	// add the start vertex and set it's dist to 0
	m_vertices[startNodeId].m_distance = 0.0f;
	openSet.emplace(startNodeId);

	// keep going as long as there's anything in the open set
	while (!openSet.empty())
	{
		// grab the best weight
		NodeId nodeId = openSet.top();
		openSet.pop();

		// Perform func
		func(nodeId, m_vertices[nodeId].m_data);

		// If this is the node we are looking for. Succeeded found it
		if (nodeId == endNodeId)
		{
			hasFoundTarget = true;
			break;
		}

		// Add this vertex to the closed set.  
		// We only use it to ask whether or not a node is in the closed set, so as an optimization we just store its membership as a bool.
		m_vertices[nodeId].m_closed = true;

		// for each neighbor
		for (const auto& [kNeighborNodeId, kWeight] : m_adjacencyList[nodeId])
		{
			// Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
			if (m_vertices[kNeighborNodeId].m_closed)
				continue;

			// Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
			// to be the case the first time the node is seen because the distance is set to infinity, so this path is 
			// guaranteed to be better.
			if (Relax(nodeId, kNeighborNodeId, kWeight))
				openSet.emplace(kNeighborNodeId);
		}
	}

	if (hasFoundTarget)
	{
		GraphVertex* pTarget = m_vertices[endNodeId];
		while (pTarget->m_pPrev->m_id != startNodeId)
		{
			path.emplace_back(pTarget->m_id);
			pTarget = pTarget->m_pPrev;
		}
	}

	return path;
}

template<class Type>
template<class Func>
inline constexpr void Graph<Type>::AStar(NodeId startNodeId, NodeId destNodeId, Func&& func)
{
	// initialize single source
	for (GraphVertex& node : m_vertices)
		node.ResetSearchData();

	// The lambda that priority queue is using
	auto openSetPriorityFunc = [this](NodeId left, NodeId right) -> bool
	{
#if PATH_CHOICE == 1
		return m_vertices[left].m_distance > m_vertices[right].m_distance;
#elif PATH_CHOICE == 2
		return m_vertices[left].m_distance < m_vertices[right].m_distance;
#endif
	};

	// Declare the open set, which is all the nodes we have yet to expand.
	using OpenSet = std::priority_queue<NodeId, std::vector<NodeId>, decltype(openSetPriorityFunc)>;
	OpenSet openSet(openSetPriorityFunc);

	// add the start vertex and set it's dist to 0
	m_vertices[startNodeId].m_distance = 0.0f;
	openSet.emplace(startNodeId);

	// keep going as long as there's anything in the open set
	while (!openSet.empty())
	{
		// grab the best weight
		NodeId currentNodeId = openSet.top();
		openSet.pop();

		// Perform func
		func(currentNodeId, m_vertices[currentNodeId].m_data);

		// Add this vertex to the closed set.  
		// We only use it to ask whether or not a node is in the closed set, so as an optimization we just store its membership as a bool.
		m_vertices[currentNodeId].m_closed = true;

		// for each neighbor
		for (const auto& [kNeighborNodeId, kWeight] : m_adjacencyList[currentNodeId])
		{
			// Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
			if (m_vertices[kNeighborNodeId].m_closed)
				continue;

			// Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
			// to be the case the first time the node is seen because the distance is set to infinity, so this path is 
			// guaranteed to be better.
			if (m_vertices[kNeighborNodeId].m_distance > m_vertices[currentNodeId].m_distance + kWeight)
			{
				m_vertices[kNeighborNodeId].m_distance = m_vertices[currentNodeId].m_distance + kWeight + Heuristic(destNodeId, kNeighborNodeId);
				m_vertices[kNeighborNodeId].m_prev = currentNodeId;
				openSet.emplace(kNeighborNodeId);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Internal recursive depth first search
//--------------------------------------------------------------------------------------------------------------------
template<class Type>
template<class Func>
inline constexpr void Graph<Type>::InternalDepthFirstSearch(NodeId nodeId, Func&& func)
{
	// set the search data for the starting vertex
	m_vertices[nodeId].m_distance = 0;
	m_vertices[nodeId].m_closed = true;
	func(m_vertices[nodeId].m_data);

	for (const auto& [kTargetId, kWeight] : m_adjacencyList[nodeId])
	{
		if (!m_vertices[kTargetId].m_closed)
		{
			// set the distance and previous node
			m_vertices[kTargetId].m_distance = m_vertices[nodeId].m_distance + 1;
			m_vertices[kTargetId].m_prev = nodeId;

			// Recursive call 
			InternalDepthFirstSearch(kTargetId, std::forward<Func>(func));
		}
	}
}
