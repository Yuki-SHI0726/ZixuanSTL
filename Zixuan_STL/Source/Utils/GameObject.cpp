#include "GameObject.h"
#include <utility>

//====================================================================================================================
// GameObject function implementations
//====================================================================================================================

//--------------------------------------------------------------------------------------------------------------------
// GameObject default Ctor
//--------------------------------------------------------------------------------------------------------------------
GameObject::GameObject()
	: m_age{0}
	, m_pName{"Unnamed"}
	, m_pData{new float(10.0f) }
{
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject parameter Ctor
//--------------------------------------------------------------------------------------------------------------------
GameObject::GameObject(int age, const char* pName, float* pData)
	: m_age{ age }
	, m_pName{ pName }
	, m_pData{ pData }
{
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject copy ctor
//--------------------------------------------------------------------------------------------------------------------
GameObject::GameObject(const GameObject& other)
	: m_age{ other.m_age }
	, m_pName{ other.m_pName }
	, m_pData{ other.m_pData }
{
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject copy assign operator
//--------------------------------------------------------------------------------------------------------------------
GameObject& GameObject::operator=(const GameObject& other)
{
	if (this == &other)
		return *this;

	m_age = other.m_age;
	m_pName = other.m_pName;
	m_pData = other.m_pData;
	return *this;
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject move ctor
//--------------------------------------------------------------------------------------------------------------------
GameObject::GameObject(GameObject&& other) noexcept
	: m_age{ other.m_age }
	, m_pName{ std::move(other.m_pName) }
	, m_pData{ nullptr }
{
	if (other.m_pData)
	{
		m_pData = new float(*other.m_pData);
		other.m_pData = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject move assign operator
//--------------------------------------------------------------------------------------------------------------------
GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	if (this == &other)
		return *this;

	m_age = other.m_age;
	m_pName = std::move(other.m_pName);

	delete m_pData;
	m_pData = other.m_pData;
	other.m_pData = nullptr;

	return *this;
}

//--------------------------------------------------------------------------------------------------------------------
// GameObject Dtor
//--------------------------------------------------------------------------------------------------------------------
GameObject::~GameObject()
{
	std::cout << "Delete GameObject: " << m_pName << std::endl;
	delete m_pData;
	m_pData = nullptr;
}

void GameObject::DoWork()
{
	std::cout << "GameObject: " << m_pName << " is working" << std::endl;
}

bool GameObject::IsAdult() const
{
	return m_age > 10;
}

//====================================================================================================================
// Person function implementations
//====================================================================================================================

//--------------------------------------------------------------------------------------------------------------------
// Person default Ctor
//--------------------------------------------------------------------------------------------------------------------
Person::Person()
	: GameObject{}
	, m_id{ -1 }
{
}

//--------------------------------------------------------------------------------------------------------------------
// Person parameter ctor
//--------------------------------------------------------------------------------------------------------------------
Person::Person(int age, const char* pName, int id, float* pData)
	: GameObject(age, pName, pData)
	, m_id{ id }
{
}

//--------------------------------------------------------------------------------------------------------------------
// Person copy ctor
//--------------------------------------------------------------------------------------------------------------------
Person::Person(const Person& other)
	: GameObject{ other }
	, m_id{ other.m_id }
{
}

//--------------------------------------------------------------------------------------------------------------------
// Person copy assign operator
//--------------------------------------------------------------------------------------------------------------------
Person& Person::operator=(const Person& other)
{
	if (this == &other)
		return *this;

	m_age = other.m_age;
	m_pName = other.m_pName;
	m_id = other.m_id;

	return *this;
}

//--------------------------------------------------------------------------------------------------------------------
// Person move ctor
//--------------------------------------------------------------------------------------------------------------------
Person::Person(Person&& other) noexcept
	: GameObject{ std::move(other) }
	, m_id{ other.m_id }
{
}

//--------------------------------------------------------------------------------------------------------------------
// Person move assign operator
//--------------------------------------------------------------------------------------------------------------------
Person& Person::operator=(Person&& other) noexcept
{
	if (this == &other)
		return *this;

	m_age = other.m_age;
	m_pName = std::move(other.m_pName);
	m_id = other.m_id;

	return *this;
}

//--------------------------------------------------------------------------------------------------------------------
// Person dtor
//--------------------------------------------------------------------------------------------------------------------
Person::~Person()
{
	std::cout << "Delete Person: " << m_pName << std::endl;
}

void Person::DoWork()
{
	std::cout << "Person: " << m_pName << " is working" << std::endl;
}

bool Person::IsAdult() const
{
	return m_age >= 18;
}


