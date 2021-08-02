#pragma once

#include "Vector2.h"

#include <cstdlib>

//---------------------------------------------------------------------------------------------------------------------
// Custom Triangle class
//---------------------------------------------------------------------------------------------------------------------
class Triangle
{
private:
	static constexpr size_t kNumPoints = 3;
	Vector2 m_vectors[kNumPoints];

public:
	Triangle(const Vector2& vector1, const Vector2& vector2, const Vector2& vector3);
	Triangle() = default;

	// API
	float GetSideLength(size_t index) const;
	float GetPerimeter() const;
	float GetArea() const;

private:
	size_t GetValidIndex(size_t input) const;
};

//---------------------------------------------------------------------------------------------------------------------
// Ctor with three vectors
//---------------------------------------------------------------------------------------------------------------------
inline Triangle::Triangle(const Vector2& vector1, const Vector2& vector2, const Vector2& vector3)
{
	m_vectors[0] = vector1;
	m_vectors[1] = vector2;
	m_vectors[2] = vector3;
}

//---------------------------------------------------------------------------------------------------------------------
// Get side length from input point 
// Formula:
//		SideLength = sqrt((x1 - x2)^2 + (y1 - y2)^2)
//---------------------------------------------------------------------------------------------------------------------
inline float Triangle::GetSideLength(size_t index) const
{
	assert(index >= 0 && index < kNumPoints);

	// Get the second point
	size_t secondPointIndex = GetValidIndex(index + 1);

	// Return calculated side length
	return m_vectors[index].DistanceTo(m_vectors[secondPointIndex]);
}

//---------------------------------------------------------------------------------------------------------------------
// Return Perimeter of this triangle 
// Formula:
//		Perimeter = ab + bc + ac
//---------------------------------------------------------------------------------------------------------------------
inline float Triangle::GetPerimeter() const
{
	float result = 0;

	for (size_t i = 0; i < kNumPoints; ++i)
		result += GetSideLength(i);

	return result;
}

//---------------------------------------------------------------------------------------------------------------------
// Return Area of this triangle 
// Formula:
//		Area = |(x1*y2 + x2*y3 + x3*y1 - y1*x2 - y2*x3 - y3*x1) / 2|
//---------------------------------------------------------------------------------------------------------------------
inline float Triangle::GetArea() const
{
	float first = 0;		// (x1*y2 + x2*y3 + x3*y1)
	float second = 0;		// (y1*x2 - y2*x3 - y3*x1)

	for (size_t i = 0; i < kNumPoints; ++i)
	{
		// Get (x1*y2 + x2*y3 + x3*y1)
		first += m_vectors[i].x * m_vectors[GetValidIndex(i + 1)].y;

		// Get (y1*x2 - y2*x3 - y3*x1)
		second += m_vectors[i].y * m_vectors[GetValidIndex(i + 1)].x;
	}

	// Get result
	return std::fabs((first - second) / 2);
}

//---------------------------------------------------------------------------------------------------------------------
// Return a valid index of vector
//---------------------------------------------------------------------------------------------------------------------
inline size_t Triangle::GetValidIndex(size_t input) const
{
	// If input index is the number of max vector, set it back to 0. 
	return input % kNumPoints;
}
