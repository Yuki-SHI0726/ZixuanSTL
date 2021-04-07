#include "TechTree.h"
#include "Tech.h"

#include <iostream>
#include <conio.h>
#include <vld.h>

using std::cout;

void TestTechTree();

int TechtreeMain()
{
    TestTechTree();

    cout << "\nAll tests complete\n";
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
// Test cases for the tech tree problem.
//---------------------------------------------------------------------------------------------------------------------
void _PrintExpectedAndActualPath(const std::vector<std::string>& expectedPath, const TechTree::Path& path)
{
    cout << "Expected Path: \n";
    for (const auto& str : expectedPath)
    {
        cout << "    " << str << "\n";
    }

    cout << "\nActual Path: \n";
    for (const auto* pTech : path)
    {
        cout << "    " << pTech->GetTechName() << "\n";
    }

    cout << "\n";
}

void TestTechTree()
{
    TechTree techTree;
    TechTree::Path path;
    std::vector<std::string> expectedPath;

    // individual test
    auto runSingleTest = [&](size_t testNum, size_t goalIndex) -> bool
    {
        path = techTree.FindBestPath(goalIndex);

        // make sure the sizes match up
        if (path.size() != expectedPath.size())
        {
            cout << "Path and expected path sizes don't match for test #" << testNum << "\n";
            _PrintExpectedAndActualPath(expectedPath, path);
            return false;
        }

        // check to see if the path is correct
        for (size_t i = 0; i < path.size(); ++i)
        {
            if (path[i]->GetTechName() != expectedPath[i])
            {
                cout << "Invalid path for test #" << testNum << "; found " << path[i] << " and expected " << expectedPath[i] << "\n";
                _PrintExpectedAndActualPath(expectedPath, path);
                return false;
            }
        }

        return true;
    };

    techTree.SetupForTest1();

    // Test #1: Throwing Parties
    expectedPath = { "Being Nice to People", "Throwing Parties" };
    runSingleTest(1, 2);

    // Test #2: Training
    expectedPath = { "Being Nice to People", "Being Nice to Animals", "Attack Animal Training" };
    runSingleTest(2, 5);

    // Test #3: Inhumane Human Experiments
    expectedPath = { "Being Nice to Books", "Being Alone", "Drug Experiments", "Inhumane Human Experiments" };
    runSingleTest(3, 12);

    // Test #4: Cost to initial tech
    expectedPath = { "Being Mean to People" };
    runSingleTest(4, 4);

    techTree.SetupForTest2();

    // Test #5: Cost to something I already have
    expectedPath.clear();
    runSingleTest(5, 4);

    // Test #6: Training
    expectedPath = { "Attack Animal Training" };
    runSingleTest(6, 5);
}
