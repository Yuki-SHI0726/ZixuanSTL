#include "TechTreeBase.h"

#include "Tech.h"
#include "TechTreeConstants.h"

#include <iostream>

void TechTreeBase::SetupForTest1()
{
    ResetTest();
}

//---------------------------------------------------------------------------------------------------------------------
// Basic military tech should be learned here because of the context in main
//---------------------------------------------------------------------------------------------------------------------
void TechTreeBase::SetupForTest2()
{
    ResetTest();

    // we know military already
    m_techs[kBasicMilitaryTechIndex]->LearnTech();
}

void TechTreeBase::PrintAllTechs()
{
    for (unsigned int i = 0; i < m_techs.size(); ++i)
    {
        std::cout << i << ") " << (i < 10 ? " " : "") << m_techs[i]->GetTechName() << "\n";
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Build up graph in TechTree.png from left to right, top to bottom
//---------------------------------------------------------------------------------------------------------------------
TechTreeBase::TechTreeBase()
{
	m_techs.reserve(kTotalTechCount);

    // Add nodes/techs
	 // social
    m_techs.emplace_back(new Tech("Being Nice to People",           1));        // 0
    m_techs.emplace_back(new Tech("Making Friends",                 6));        // 1
    m_techs.emplace_back(new Tech("Throwing Parties",               10));       // 2
    m_techs.emplace_back(new Tech("Being Nice to Animals",          5));        // 3

    // military
    m_techs.emplace_back(new Tech("Being Mean to People",           10));       // 4
    m_techs.emplace_back(new Tech("Attack Animal Training",         5));        // 5
    m_techs.emplace_back(new Tech("Being Mean to Animals",          2));        // 6
    m_techs.emplace_back(new Tech("Inhumane Animal Experiments",    10));       // 7

    // science
    m_techs.emplace_back(new Tech("Being Nice to Books",            5));        // 8
    m_techs.emplace_back(new Tech("Being Alone",                    1));        // 9
    m_techs.emplace_back(new Tech("Drug Experiments",               4));        // 10
    m_techs.emplace_back(new Tech("Mysteries of the Human Mind",    15));       // 11
    m_techs.emplace_back(new Tech("Inhumane Human Experiments",     15));       // 12

    // Add edges
    // link the social tree
    m_techs[0]->AddUnlock(m_techs[1]);
    m_techs[0]->AddUnlock(m_techs[2]);
    m_techs[0]->AddUnlock(m_techs[3]);
    m_techs[3]->AddUnlock(m_techs[5]);

    // link the military tree
    m_techs[4]->AddUnlock(m_techs[5]);
    m_techs[4]->AddUnlock(m_techs[6]);
    m_techs[6]->AddUnlock(m_techs[7]);

    // link the science tree
    m_techs[8]->AddUnlock(m_techs[9]);
    m_techs[9]->AddUnlock(m_techs[10]);
    m_techs[9]->AddUnlock(m_techs[11]);
    m_techs[10]->AddUnlock(m_techs[7]);
    m_techs[10]->AddUnlock(m_techs[12]);
    m_techs[11]->AddUnlock(m_techs[12]);
}

//---------------------------------------------------------------------------------------------------------------------
// Clean tech memory
//---------------------------------------------------------------------------------------------------------------------
TechTreeBase::~TechTreeBase()
{
    for (Tech* pTech : m_techs)
    {
        delete pTech;
        pTech = nullptr;
    }
    m_techs.clear();
}

//---------------------------------------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------------------------------------
void TechTreeBase::ResetTest()
{
    // Set every tech has not been unlocked
    for (Tech* pTech : m_techs)
        pTech->m_hasTech = false;
}
