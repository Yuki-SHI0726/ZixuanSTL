#pragma once

#include <numeric>
#include <vector>
#include <unordered_map>

// width & height, in tiles
static constexpr size_t kWorldWidth = 6;
static constexpr size_t kWorldHeight = 4;
static constexpr size_t kNumTiles = kWorldWidth * kWorldHeight;

// Alias
using NodeId = size_t;
using Dist = float;
using Edges = std::unordered_map<NodeId, Dist>;
using SearchData = std::pair<NodeId, Dist>;		// NodeId as prev, Dist as distance to start node.
using AdjacencyList = std::vector<Edges>;
static constexpr NodeId kInvalidNodeId = std::numeric_limits<NodeId>::max();
static constexpr Dist kStartDist = std::numeric_limits<Dist>::max();

// Weights
static constexpr Dist kGroundWeight = 1.0f;
static constexpr Dist kGrassWeight	= 2.0f;
static constexpr Dist kWaterWeight	= 8.0f;
static constexpr Dist kFireWeight	= 10.0f;
static constexpr Dist kStartWeight	= 0.0f;
static constexpr Dist kDestWeight	= 0.0f;