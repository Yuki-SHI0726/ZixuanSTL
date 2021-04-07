#pragma once

#include <string>
#include <vector>

class Tech
{
    friend class TechTreeBase;

    std::string m_techName;
    int m_cost;
    bool m_hasTech;

    std::vector<Tech*> m_unlocks;

public:
    // Returns the name of the tech.
    const std::string& GetTechName() const { return m_techName; }

    // Returns the cost of purchasing this tech.
    int GetCost() const { return m_cost; }

    // Returns true if the player has this tech, false if they don't.
    bool HasTech() const { return m_hasTech; }

    // Returns the vector of all techs that this tech will unlock once it's been learned.  It's is a const reference, 
    // so it cannot be modified.
    const std::vector<Tech*>& GetAllUnlocks() const { return m_unlocks; }

private:
    // These are private functions that I would typically make public, but you are not allowed to call them so I 
    // have made them private.
    Tech(const std::string& techName, int cost);
    void AddUnlock(Tech* pTech);
    void LearnTech();
};

