#include "DataStructures/Graph.h"
#include <array>

static constexpr size_t kRow = 5;
static constexpr size_t kColumn = 5;
static constexpr size_t kSize = kRow * kColumn;
static constexpr size_t kInvalid = std::numeric_limits<size_t>::max();

std::array<char, kSize> g_map =
{
	'G', 'G', 'G', 'G', 'G',
	'G', 'B', 'B', 'B', 'G',
	'G', 'G', 'G', 'B', 'G',
	'G', 'G', 'G', 'B', 'G',
	'G', 'G', 'G', 'G', 'G',
};

float GetWeightFromType(char node)
{
	switch (node)
	{
	case 'G': return 2.0f;
	case 'B': return 5.0f;
	case 'W': return 0.0f;
	case 'R': return 3.0f;
	default: return 0.0f;
	}
}

float GetWeightBetweenCells(size_t FromId, size_t ToId)
{
	const float FromCost = GetWeightFromType(g_map[FromId]);
	const float ToCost = GetWeightFromType(g_map[ToId]);
	const float TotalCost = (FromCost + ToCost) / 2.0f;
	return TotalCost;
}

size_t GetIndexFromXY(int x, int y)
{
	return ((y * kRow) + x);
}

bool IsPassable(int x, int y)
{
	const size_t Index = GetIndexFromXY(x, y);
	const float CellWeight = GetWeightFromType(g_map[Index]);
	return CellWeight > 0.0f;
}

std::array<size_t, 4> GetNeighbors(size_t Index)
{
	std::array<size_t, 4> neighbors{ kInvalid, kInvalid, kInvalid, kInvalid };

	int Y = static_cast<int>(Index / kRow);
	int X = static_cast<int>(Index % kRow);

	// Left
	--X;
	if (X >= 0 && IsPassable(X, Y))
	{
		neighbors[0] = GetIndexFromXY(X, Y);
	}

	// Right
	X += 2;
	if (X < kRow && IsPassable(X, Y))
	{
		neighbors[1] = GetIndexFromXY(X, Y);
	}

	// Reset X
	--X;

	// Upper
	--Y;
	if (Y >= 0 && IsPassable(X, Y))
	{
		neighbors[2] = GetIndexFromXY(X, Y);
	}

	// Bottom
	Y += 2;
	if (Y < kColumn && IsPassable(X, Y))
	{
		neighbors[3] = GetIndexFromXY(X, Y);
	}

	return neighbors;
}

zxstl::Graph<char> InitGraph()
{
	zxstl::Graph<char> graph;

	for (size_t i = 0; i < g_map.size(); ++i)
	{
		graph.AddNode(g_map[i]);
	}

	for (size_t i = 0; i < g_map.size(); ++i)
	{
		std::array<size_t, 4> neighbors = GetNeighbors(i);
		for (size_t neighbor : neighbors)
		{
			if (neighbor == kInvalid)
			{
				continue;
			}

			graph.AddEdge(i, neighbor, GetWeightFromType(g_map[neighbor]));
		}
	}

	return graph;
}

int main()
{
	zxstl::Graph<char> graph = InitGraph();

	graph.RunAStar(20, 4, [](size_t left, const char& right) {});
	graph.PrintShortestPath(4);

	return 0;
}