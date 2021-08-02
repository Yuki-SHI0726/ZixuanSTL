#pragma once
#include <iostream>

//------------------------------------------------------------------------------------------------------
// Plain Actor class for simple usage
//------------------------------------------------------------------------------------------------------
class Actor
{
private:
	int m_data;
	int* m_pPtr;

public:
	Actor();
	Actor(int data);
	Actor(int data, int* pPtr);
	Actor(const Actor& other);
	Actor& operator=(const Actor& other);
	Actor(Actor&& other) noexcept;
	Actor& operator=(Actor&& other) noexcept;
	~Actor();

	// API
	template <typename Func> void Work(Func&& func);

	// Modifiers
	void SetData(int data) { m_data = data; }
	int GetData() const { return m_data; }

	// Overloadings
	friend std::ostream& operator<<(std::ostream& stream, const Actor& actor)
	{
		stream << "Actor data: " << actor.m_data;
		if (actor.m_pPtr)
			stream << ". Actor ptr: " << *actor.m_pPtr;
		return stream;
	}
};

//------------------------------------------------------------------------------------------------------
// Recursively process func until m_data is less or equal to 0
// func must take in a int value and a pointer
//------------------------------------------------------------------------------------------------------
template<typename Func>
inline void Actor::Work(Func&& func)
{
	if (m_data <= 0)
		return;

	--m_data;
	func(m_data, m_pPtr);
	Work(std::forward<Func>(func));
}

