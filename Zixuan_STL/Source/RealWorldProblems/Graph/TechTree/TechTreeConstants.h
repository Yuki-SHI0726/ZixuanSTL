#pragma once

#include <numeric>
#include <unordered_map>
#include <vector>

// Alias
using NodeId = size_t;
using Dist = int;
static constexpr Dist kStartDistance = std::numeric_limits<Dist>::max();

// Tech search data
static constexpr size_t kTotalTechCount = 13;
