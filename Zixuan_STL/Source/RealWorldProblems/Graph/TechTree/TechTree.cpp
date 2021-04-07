// TechTree.cpp
#include "TechTree.h"
#include "Tech.h"

#include <iostream>
#include <queue>

struct SearchData
{
    const Tech* m_pPrev = nullptr;
    Dist m_distance = kStartDistance;
    bool m_closed = false;
};

//---------------------------------------------------------------------------------------------------------------------
// This function finds the best path to the tech we want.
//      * goalTech: The index of the tech we're looking for.  Call GetTechByIndex() to get the actual Tech instance.
//      * bestPath: The best path to the goal tech.  This is an array sorted in the order of the best path from the 
//                  start to the goal.  This is an output variable; it's what you need to populate with this function.
//---------------------------------------------------------------------------------------------------------------------
TechTree::Path TechTree::FindBestPath(size_t goalTech)
{
    // Reset path
    Path bestPath;

    // If goal tech is not valid or it's already owned, exit early
    if (goalTech > kTotalTechCount || GetTechByIndex((unsigned int)goalTech)->HasTech())
        return bestPath;

    // Graph search map
    std::unordered_map<const Tech*, SearchData> searchMap;

    // Calculate Cost lambda
    auto calcCost = [](const Tech* pTech)
    {
        return pTech->HasTech() ? 0 : pTech->GetCost();
    };

    // Priority queue func
    auto frontierPriorityFunc = [&searchMap](const Tech* pLeft, const Tech* pRight) -> bool
    {
        return searchMap[pLeft].m_distance > searchMap[pRight].m_distance;
    };

    // Get frontier priority queue
    using OpenSet = std::priority_queue<const Tech*, std::vector<const Tech*>, decltype(frontierPriorityFunc)>;
    OpenSet openSet(frontierPriorityFunc);

    // Setup and push three basic techs to the queue
    // Social
    searchMap[GetBasicSocialTech()].m_distance = calcCost(GetBasicSocialTech());
    openSet.emplace(GetBasicSocialTech());

    // Military
    searchMap[GetBasicMilitaryTech()].m_distance = calcCost(GetBasicMilitaryTech());
    openSet.emplace(GetBasicMilitaryTech());

    // Science
    searchMap[GetBasicScienceTech()].m_distance = calcCost(GetBasicScienceTech());
    openSet.emplace(GetBasicScienceTech());

    // Search in the frontier
    while (!openSet.empty())
    {
        // Get the top tech
        const Tech* pCurrentTech = openSet.top();
        openSet.pop();

        // Exits early if found target tech
        if (pCurrentTech == GetTechByIndex((unsigned int)goalTech))
            break;

        // set this tech has been seen
        searchMap[pCurrentTech].m_closed = true;

        // Get neighbors
        for (const Tech* pNextTech : pCurrentTech->GetAllUnlocks())
        {
            // Grab the neighbor.  If it's in the closed set, skip it.  This keeps us from processing cycles.
            if (searchMap[pNextTech].m_closed)
                continue;

            // Relax the tech
            // Get cost to the target tech
            int newCost = calcCost(pNextTech);
            if (searchMap[pNextTech].m_distance > searchMap[pCurrentTech].m_distance + newCost)
            {
                searchMap[pNextTech].m_distance = searchMap[pCurrentTech].m_distance + newCost;
                searchMap[pNextTech].m_pPrev = pCurrentTech;
                openSet.emplace(pNextTech);
            }
        }
    }

    // Get path from goal to start
    const Tech* pCurrentTech = GetTechByIndex((unsigned int)goalTech);
    while (pCurrentTech && !pCurrentTech->HasTech())
    {
        bestPath.emplace_back(pCurrentTech);
        pCurrentTech = searchMap[pCurrentTech].m_pPrev;
    }

    // Reverse the path to match the result
    std::reverse(bestPath.begin(), bestPath.end());

    // Return the path
    return bestPath;
}
