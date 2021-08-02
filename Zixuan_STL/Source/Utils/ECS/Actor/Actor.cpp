#include "Actor.h"

#define LOG 1


Actor::Actor()
	: m_data{ 0 }
	, m_pPtr{ nullptr }
{
#if LOG
	std::cout << "Default constructed an Actor: " << std::endl;
#endif
}

Actor::Actor(int data)
	: m_data{ data }
	, m_pPtr{ nullptr }
{
#if LOG
	std::cout << "Overloaded constructed an Actor: " << std::endl;
#endif
}

Actor::~Actor()
{
	delete m_pPtr;
	m_pPtr = nullptr;

#if LOG
	std::cout << "Destructed an Actor: " << std::endl;
#endif
}

Actor::Actor(int data, int* pPtr)
	: m_data{ data }
	, m_pPtr{ pPtr }
{
#if LOG
	std::cout << "Overloaded constructed an Actor: " << std::endl;
#endif
}

Actor::Actor(const Actor& other)
	: m_data{ other.m_data }
	, m_pPtr{ nullptr }
{
	if (other.m_pPtr)
		m_pPtr = new int(*other.m_pPtr);

#if LOG
	std::cout << "Copy constructed an Actor: " << std::endl;
#endif
}

Actor& Actor::operator=(const Actor& other)
{
	if (this == &other)
		return *this;

	delete m_pPtr;

	m_data = other.m_data;
	if (other.m_pPtr)
		m_pPtr = new int(*other.m_pPtr);

#if LOG
	std::cout << "Copy operator= an Actor: " << std::endl;
#endif

	return *this;
}

Actor::Actor(Actor&& other) noexcept
	: m_data{ other.m_data }
	, m_pPtr{ nullptr }
{
	delete m_pPtr;
	m_pPtr = other.m_pPtr;

	other.m_data = 0;
	other.m_pPtr = nullptr;

#if LOG
	std::cout << "Move constructed an Actor: " << std::endl;
#endif
}

Actor& Actor::operator=(Actor&& other) noexcept
{
	if (this == &other)
		return *this;

	delete m_pPtr;
	m_pPtr = other.m_pPtr;

	other.m_data = 0;
	other.m_pPtr = nullptr;

#if LOG
	std::cout << "Move operator= an Actor: " << std::endl;
#endif

	return *this;
}
