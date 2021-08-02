#pragma once

class HealthComponent
{
    int m_hp;

public:
    HealthComponent(int hp);
    HealthComponent& operator=(const HealthComponent& right);

    void Kill() { --m_hp; }
    int Get() const { return m_hp; }
};