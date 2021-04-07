#pragma once
#include "TechTreeBase.h"

#include "TechTreeConstants.h"

//---------------------------------------------------------------------------------------------------------------------
// Your comments go here:
//  The TechTree is a map, a directed, weighted map.
//      1. Set up the graph first.
//      2. Set up graph search data.
//      3. Build the path finding algorithm
//
//---------------------------------------------------------------------------------------------------------------------
// 1. Set up the graph first.
//---------------------------------------------------------------------------------------------------------------------
//      - Create Teches and add edges to them using AddUnlock().
//      - I see SetupForTest() functions, I assume that they should be called at some point, 
//          so I digged through the TestTechTree() in Main.cpp, I noticed that     
//              ( Test #5: Cost to something I already have ) comment. 
//              So I should set BasicMilitaryTech as learned in SetupForTest2().
//
//---------------------------------------------------------------------------------------------------------------------
//      2. Set up graph search data.
//---------------------------------------------------------------------------------------------------------------------
//      - Since I'm not allowed to change anything inside Tech.h and TechTreeBase.h,
//          I will have to previous nodes info and distance info inside TechTreeBase 
//
//---------------------------------------------------------------------------------------------------------------------
//      3. Build the path finding algorithm
//---------------------------------------------------------------------------------------------------------------------
//      - Since the task explicitly asked for the "Best" path, therefore I ought to use Dijkstra instead of A* because Dijkatra is guaranteed the best path
//      - The difference between this task and the task we did before is I have three available starting points here.
//          Which tells me I should add those three tech to the priority queue before the search starts
//  
//---------------------------------------------------------------------------------------------------------------------
//  Feedback from me
//---------------------------------------------------------------------------------------------------------------------
//      - The tech names in the test function does not exactly match the name in the png :(
//          i.e. Drug Experimentation vs Drug Experiments
//               Being a Dick to People vs Being Mean to People
class TechTree : public TechTreeBase
{
public:
    using Path = std::vector<const Tech*>;

    Path FindBestPath(size_t goalTech);
};
