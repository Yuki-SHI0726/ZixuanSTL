#pragma once

#include "FindTileWorldConstants.h"
#include "Utils/Vector2.h"
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------
// World for path-finding, which is a graph
//----------------------------------------------------------------------------------------------------------
class FindTileWorld
{
private:
	enum class Type : char
	{
		kStart = '@',
		kDest = 'X',
		kWater = '&',
		kGrass = '+',
		kGround = '.',
		kFire = '^',
		kNum
	};

	// Representing graph nodes
	struct GraphVertex
	{
		NodeId m_id;
		const Dist kWeight;
		char m_data;
		Type m_type;

		// Graph search data
		//NodeId m_prev;
		//Dist m_distance;
		bool m_closed;

		GraphVertex(char data, Dist weight, Type type, NodeId id)
			: m_data(data)
			//, m_prev{ kInvalidNodeId }
			//, m_distance{ kStartDist }
			, m_id{ id }
			, m_closed{ false }
			, kWeight{ weight }
			, m_type{ type }
		{
		}

		constexpr void ResetSearchData()
		{
			//m_prev = kInvalidNodeId;
			//m_distance = kStartDist;
			m_closed = false;
		}
	};

	// Graph structures
	std::vector<GraphVertex> m_vertices;		
	std::unordered_map<NodeId, SearchData> m_searchMap;		// first as Prev, second as distance
	AdjacencyList m_adjacencyList;

	// Nodes
	inline static NodeId s_id = 0;
	NodeId m_startNodeId = kInvalidNodeId;
	NodeId m_destNodeId = kInvalidNodeId;

public:
	void Init();
	void Run();

private:
	// Init
	NodeId AddNode(char data, Type type);
	Edges GetNeighbors(NodeId centerNodeId) const;

	// Path finding
	void Reset();

	// A*
	 void AStar();
	 Dist Heuristic(NodeId destNodeId, NodeId currentNodeId) const;

	// Dijkstra
	void Dijkstra();

	// Utils
	void PrintShortestPath(NodeId nodeId);
	constexpr NodeId GetIndexFromXY(size_t x, size_t y) const { return (y * kWorldWidth) + x; }
	Vector2 GetXYFromIndex(NodeId id) const;
	constexpr void PrintMap() const;
};
