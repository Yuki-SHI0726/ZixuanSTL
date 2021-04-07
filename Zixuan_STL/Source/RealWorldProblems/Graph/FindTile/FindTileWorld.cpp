#include "FindTileWorld.h"
#include "Utils/Log.h"

#include <assert.h>
#include <queue>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------------------------------------
// Set up graph nodes
//----------------------------------------------------------------------------------------------------------
void FindTileWorld::Init()
{
	// Init structures
	m_adjacencyList.reserve(kNumTiles);
	m_vertices.reserve(kNumTiles);

	// Init map
	Type map[kNumTiles] = 
	{
		Type::kStart,	Type::kWater,	Type::kFire,	Type::kGround,	Type::kGround,	Type::kGround,
		Type::kGround,	Type::kWater,	Type::kFire,	Type::kWater,	Type::kGround,	Type::kGround,
		Type::kGround,	Type::kWater,	Type::kGround,	Type::kWater,	Type::kGround,	Type::kDest,
		Type::kGrass,	Type::kGrass,	Type::kGrass,	Type::kWater,	Type::kGround,	Type::kGround,
	};

	// Init graph nodes
	size_t data = static_cast<size_t>('A');
	for (size_t i = 0; i < kNumTiles; ++i)
	{
		AddNode(static_cast<char>(data), map[i]);
		++data;
	}

	// Assign neighbors
	for (size_t i = 0; i < kNumTiles; ++i)
	{
		Edges neighbors = GetNeighbors(i);
		m_adjacencyList[i] = neighbors;
	}
}

//----------------------------------------------------------------------------------------------------------
// Find best path to the target
//----------------------------------------------------------------------------------------------------------
void FindTileWorld::Run()
{
	PrintMap();
	AStar();
	PrintShortestPath(m_destNodeId);
}

//----------------------------------------------------------------------------------------------------------
// Add node to the graph
//----------------------------------------------------------------------------------------------------------
NodeId FindTileWorld::AddNode(char data, Type type)
{
	// Check if current node is a start or dest
	if (m_startNodeId == kInvalidNodeId && type == Type::kStart)
		m_startNodeId = s_id;

	if (m_destNodeId == kInvalidNodeId && type == Type::kDest)
		m_destNodeId = s_id;

	// Get weight
	Dist weight = [type]() -> Dist
	{
		switch (type)
		{
		case FindTileWorld::Type::kStart:	return kStartWeight;
		case FindTileWorld::Type::kDest:	return kDestWeight;
		case FindTileWorld::Type::kWater:   return kWaterWeight;
		case FindTileWorld::Type::kGrass:   return kGrassWeight;
		case FindTileWorld::Type::kGround:  return kGroundWeight;
		case FindTileWorld::Type::kFire:    return kFireWeight;
		default: assert(false);		return 0;	// Something must went wrong if it goes here
		}
	}();

	// Insert node to structures
	m_vertices.emplace_back(data, weight, type, s_id);
	m_adjacencyList.emplace_back();

	// Update id
	++s_id;

	assert(m_vertices.size() == m_adjacencyList.size());
	return m_vertices.size() - 1;
}

//----------------------------------------------------------------------------------------------------------
// Get four surrounding neighbors around the center node
//----------------------------------------------------------------------------------------------------------
Edges FindTileWorld::GetNeighbors(NodeId centerNodeId) const
{
	Edges outNeighbors;

	// I'm using int for x and y pos because I will use substraction to check if they are less than 0
	int y = (int)centerNodeId / kWorldWidth;
	int x = (int)centerNodeId % kWorldWidth;

	// Left neighbor
	--x;
	if (x >= 0)
	{
		NodeId leftNeighborNodeId = GetIndexFromXY((size_t)x, (size_t)y);
		outNeighbors.insert({ leftNeighborNodeId, m_vertices[leftNeighborNodeId].kWeight });
	}

	// right neighbor
	x += 2;
	if (x < kWorldWidth)
	{
		NodeId rightNeighborNodeId = GetIndexFromXY((size_t)x, (size_t)y);
		outNeighbors.insert({ rightNeighborNodeId, m_vertices[rightNeighborNodeId].kWeight });
	}

	// reset x
	--x;

	// upper neighbor
	--y;
	if (y >= 0)
	{
		NodeId upperNeighborNodeId = GetIndexFromXY((size_t)x, (size_t)y);
		outNeighbors.insert({ upperNeighborNodeId, m_vertices[upperNeighborNodeId].kWeight });
	}

	// bottom neighbor
	y += 2;
	if (y < kWorldHeight)
	{
		NodeId bottomNeighborNodeId = GetIndexFromXY((size_t)x, (size_t)y);
		outNeighbors.insert({ bottomNeighborNodeId, m_vertices[bottomNeighborNodeId].kWeight });
	}

	return outNeighbors;
}

void FindTileWorld::Reset()
{
	for (GraphVertex& node : m_vertices)
	{
		node.m_closed = false;
		m_searchMap[node.m_id].first = kInvalidNodeId;
		m_searchMap[node.m_id].second = kStartDist;
	}
}

//----------------------------------------------------------------------------------------------------------
// Find the best path from start to dest
//----------------------------------------------------------------------------------------------------------
void FindTileWorld::AStar()
{
	// initialize single source
	Reset();

	// Priority queue func
	auto frontierPriorityFunc = [this](NodeId left, NodeId right) -> bool
	{
		return m_searchMap[left].second > m_searchMap[right].second;
	};

	// Declare the open set, which is all the nodes we have yet to expand.
	using Frontier = std::priority_queue<NodeId, std::vector<NodeId>, decltype(frontierPriorityFunc)>;
	Frontier frontier(frontierPriorityFunc);

	// add the start vertex and set it's dist to 0
	m_searchMap[m_startNodeId].second = m_vertices[m_startNodeId].kWeight;
	frontier.push(m_startNodeId);

	// keep going as long as there's anything in the open set
	while (!frontier.empty())
	{
		// grab the best weight
		NodeId currentNodeId = frontier.top();
		frontier.pop();

		// Exit early if current node is the dest
		if (currentNodeId == m_destNodeId)
			break;

		// Add this vertex to the closed set.  
		// We only use it to ask whether or not a node is in the closed set, so as an optimization we just store its membership as a bool.
		m_vertices[currentNodeId].m_closed = true;

		// for each neighbor
		for (const auto& [kNextNodeId, kWeight] : m_adjacencyList[currentNodeId])
		{
			// Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
			if (m_vertices[kNextNodeId].m_closed)
				continue;

			// Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
			// to be the case the first time the node is seen because the distance is set to infinity, so this path is 
			// guaranteed to be better.
			if (m_searchMap[kNextNodeId].second > m_searchMap[currentNodeId].second + kWeight)
			{
				Dist dist = Heuristic(m_destNodeId, kNextNodeId);
				m_searchMap[kNextNodeId].second = m_searchMap[currentNodeId].second + kWeight + dist;
				m_searchMap[kNextNodeId].first = currentNodeId;
				frontier.push(kNextNodeId);
			}
		}
	}
}

void FindTileWorld::Dijkstra()
{
	// initialize single source
	Reset();

	// Priority queue func
	auto frontierPriorityFunc = [this](NodeId left, NodeId right) -> bool
	{
		return m_searchMap[left].second > m_searchMap[right].second;
	};

	// Declare the open set, which is all the nodes we have yet to expand.
	using Frontier = std::priority_queue<NodeId, std::vector<NodeId>, decltype(frontierPriorityFunc)>;
	Frontier frontier(frontierPriorityFunc);

	// add the start vertex and set it's dist to 0
	m_searchMap[m_startNodeId].second = m_vertices[m_startNodeId].kWeight;
	frontier.push(m_startNodeId);

	// keep going as long as there's anything in the open set
	while (!frontier.empty())
	{
		// grab the best weight
		NodeId currentNodeId = frontier.top();
		frontier.pop();

		// Exit early if current node is the dest
		if (currentNodeId == m_destNodeId)
			break;

		// Add this vertex to the closed set.  
		// We only use it to ask whether or not a node is in the closed set, so as an optimization we just store its membership as a bool.
		m_vertices[currentNodeId].m_closed = true;

		// for each neighbor
		for (const auto& [kNextNodeId, kWeight] : m_adjacencyList[currentNodeId])
		{
			// Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
			if (m_vertices[kNextNodeId].m_closed)
				continue;

			// Relax the node.  If this path is better, we insert it into the open set.  Note that this is guaranteed 
			// to be the case the first time the node is seen because the distance is set to infinity, so this path is 
			// guaranteed to be better.
			if (m_searchMap[kNextNodeId].second > m_searchMap[currentNodeId].second + kWeight)
			{
				m_searchMap[kNextNodeId].second = m_searchMap[currentNodeId].second + kWeight;
				m_searchMap[kNextNodeId].first = currentNodeId;
				frontier.push(kNextNodeId);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------
// calculates the distance between the two nodes' world coordinates
//----------------------------------------------------------------------------------------------------------
Dist FindTileWorld::Heuristic(NodeId destNodeId, NodeId currentNodeId) const
{
	Vector2 startPos = GetXYFromIndex(destNodeId);
	float startX = startPos.x;
	float startY = startPos.y;

	Vector2 destPos = GetXYFromIndex(currentNodeId);
	Dist endX = destPos.x;
	Dist endY = destPos.y;

	return Dist(std::fabs(startX - endX) + std::fabs(startY - endY));
}

//----------------------------------------------------------------------------------------------------------
// Print the shortest path from start to dest node
//----------------------------------------------------------------------------------------------------------
void FindTileWorld::PrintShortestPath(NodeId nodeId)
{
	if (nodeId == kInvalidNodeId)
		return;

	assert(nodeId < m_vertices.size());
	PrintShortestPath(m_searchMap[nodeId].first);

	// Get color
	Log::Color color = [nodeId, this]()
	{
		switch (m_vertices[nodeId].m_type)
		{
		case FindTileWorld::Type::kStart:	return Log::Color::kYellow;
		case FindTileWorld::Type::kDest:	return Log::Color::kMagenta;
		case FindTileWorld::Type::kWater:   return Log::Color::kCyan;
		case FindTileWorld::Type::kGrass:   return Log::Color::kGreen;
		case FindTileWorld::Type::kGround:  return Log::Color::kBrown;
		case FindTileWorld::Type::kFire:    return Log::Color::kRed;
		default: assert(false);		return Log::Color::kBlack;	// Something must went wrong if it goes here
		}
	}();

	Log::Get().PrintInColor(color, "%c", m_vertices[nodeId].m_data);
	std::cout << " -> ";
}

//----------------------------------------------------------------------------------------------------------
// Return vec2 of xy from index
//----------------------------------------------------------------------------------------------------------
Vector2 FindTileWorld::GetXYFromIndex(NodeId id) const
{
	Dist y = static_cast<Dist>(id / kWorldWidth);
	Dist x = static_cast<Dist>(id % kWorldWidth);
	return Vector2(x, y);
}

//----------------------------------------------------------------------------------------------------------
// Print map
//----------------------------------------------------------------------------------------------------------
constexpr void FindTileWorld::PrintMap() const
{
	for (size_t y = 0; y < kWorldHeight; ++y)
	{
		for (size_t x = 0; x < kWorldWidth; ++x)
		{
			NodeId id = GetIndexFromXY(x, y);
			Log::Color color = [id, this]()
			{
				switch (m_vertices[id].m_type)
				{
				case FindTileWorld::Type::kStart:	return Log::Color::kYellow;
				case FindTileWorld::Type::kDest:	return Log::Color::kMagenta;
				case FindTileWorld::Type::kWater:   return Log::Color::kCyan;
				case FindTileWorld::Type::kGrass:   return Log::Color::kGreen;
				case FindTileWorld::Type::kGround:  return Log::Color::kBrown;
				case FindTileWorld::Type::kFire:    return Log::Color::kRed;
				default: assert(false);		return Log::Color::kBlack;	// Something must went wrong if it goes here
				}
			}();

			Log::Get().PrintInColor(color, "%c", m_vertices[id].m_data);
			std::cout << " ";
		}

		std::cout << std::endl;
	}
}

