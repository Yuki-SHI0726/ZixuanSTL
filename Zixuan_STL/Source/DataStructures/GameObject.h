#pragma once
#include <iostream>

//-----------------------------------------------------------------------------------------------------------------------------
// Used for testing template
//-----------------------------------------------------------------------------------------------------------------------------
class GameObject
{
protected:
	int m_age;
	const char* m_pName;
	float* m_pData;	// TODO: change this to template

public:
	GameObject();
	GameObject(int age, const char* pName);
	GameObject(const GameObject& other);
	GameObject& operator=(const GameObject& other);
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(GameObject&& other) noexcept;
	~GameObject();

	virtual void DoWork();
	virtual bool IsAdult() const;

	int GetAge() const { return m_age; }
	const char* GetName() const { return m_pName; }
};

class Person : public GameObject
{
private:
	int m_id;

public:
	Person();
	Person(int age, const char* pName, int id);
	Person(const Person& other);
	Person& operator=(const Person& other);
	Person(Person&& other) noexcept;
	Person& operator=(Person&& other) noexcept;
	~Person();

	virtual void DoWork() override final;
	virtual bool IsAdult() const override final;

	int GetId() const { return m_id; }
};

// Paste this to a cpp file to cout objects
#if 0
std::ostream& operator<<(std::ostream& stream, GameObject* pObject)
{
	stream << "Name: " << pObject->GetName() << ", age: " << pObject->GetAge();

	if (Person* pPerson = dynamic_cast<Person*>(pObject); pPerson != nullptr)
	{
		stream << ", Person id: " << pPerson->GetId();
	}

	return stream;
}
#endif