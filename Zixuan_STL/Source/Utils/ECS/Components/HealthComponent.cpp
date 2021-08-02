#include "HealthComponent.h"

HealthComponent::HealthComponent(int hp)
	: m_hp(hp)
{
}

HealthComponent& HealthComponent::operator=(const HealthComponent& right)
{
	m_hp = right.m_hp; 
	return (*this);
}
