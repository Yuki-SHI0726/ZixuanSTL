#pragma once

#include "Vector2.h"
#include "Math.h"
#include <cstdlib>

//---------------------------------------------------------------------------------------------------------------------
// Custom Circle class
//---------------------------------------------------------------------------------------------------------------------
class Circle
{
public:
	Vector2 m_center;
	float m_radius;	
	float m_diameter;

	constexpr Circle(const Vector2& center, float radius);
	Circle() = delete;	// I don't allow people to create a circle with out a center or a radius

	// API
	bool IsOverlapping(const Circle& other) const;
	constexpr float Circumference() const;
	constexpr float Area() const;
};

//---------------------------------------------------------------------------------------------------------------------
// Custom Circle class
//---------------------------------------------------------------------------------------------------------------------
inline constexpr Circle::Circle(const Vector2& center, float radius)
	: m_center{ center }
	, m_radius{ radius }
	, m_diameter{ m_radius * 2 }
{
	assert(m_radius >= 0);
}

//---------------------------------------------------------------------------------------------------------------------
// Check if this circle is overlapping with the other
// Formula:
//		If their centers' distance is less than this->m_radius + other.m_radius, then they are overlapping, return true
//---------------------------------------------------------------------------------------------------------------------
inline bool Circle::IsOverlapping(const Circle& other) const
{
	// Get distance between two circles' centers
	float centerDistance = this->m_center.DistanceTo(other.m_center);

	// Return result
	return centerDistance < (this->m_radius + other.m_radius);
}

//---------------------------------------------------------------------------------------------------------------------
// Get circumference
// Formula:
//		Circumference = 2*Pi*radius
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Circle::Circumference() const
{
	return 2 * m_radius * PI;
}

//---------------------------------------------------------------------------------------------------------------------
// Get Area
// Formula:
//		Area = Pi * (radius^2)
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Circle::Area() const
{
	return PI * (m_radius * m_radius);
}
