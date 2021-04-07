// TechTreeBase.h
#pragma once

#include <vector>
#include <assert.h>

class Tech;

class TechTreeBase
{
    static constexpr int kBasicSocialTechIndex = 0;
    static constexpr int kBasicMilitaryTechIndex = 4;
    static constexpr int kBasicScienceTechIndex = 8;

    std::vector<Tech*> m_techs;

public:
    // These functions are called test code.  You shouldn't need to call them yourself unless you're rewriting the 
    // test function.
    void SetupForTest1();
    void SetupForTest2();

    // Prints out all the techs.  This function is also called by the test code, so you shouldn't need to directly
    // call it, but you can if you want.
    void PrintAllTechs();

protected:
    // Protected constructor & destructor, which prevents you from instantiating this class.  It must be inherited 
    // from.  Note that the destructor is virtual, allowing you to override it if you want.
    TechTreeBase();
    virtual ~TechTreeBase();

    // Returns a const tech pointer by index.  This is how you can get the goal tech when the user enters the index 
    // of the tech to find.  Note that since it returns a const pointer, techs cannot be modified.
    const Tech* GetTechByIndex(unsigned int index) const { assert(index >= 0 && index < m_techs.size()); return m_techs[index]; }

    // These functions return the base techs the player can always learn.  Note that since they return a const 
    // pointer, techs cannot be modified.
    const Tech* GetBasicSocialTech()    const { return m_techs[kBasicSocialTechIndex]; }
    const Tech* GetBasicMilitaryTech()  const { return m_techs[kBasicMilitaryTechIndex]; }
    const Tech* GetBasicScienceTech()   const { return m_techs[kBasicScienceTechIndex]; }

private:
    void ResetTest();
};

