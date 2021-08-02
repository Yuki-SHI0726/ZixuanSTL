#pragma once
#include "Vector3.h"
#include "Helpers.h"

#include <assert.h>

//---------------------------------------------------------------------------------------------------------------------
// Custom Circle 3d class
//---------------------------------------------------------------------------------------------------------------------
class Sphere
{
public:
	Vector3 m_center;
	float m_radius;
	float m_diameter;

	constexpr Sphere(const Vector3 center, float radius);
	Sphere() = delete;	// I don't allow people to create a circle with out a center or a radius

	// API
	bool IsOverlapping(const Sphere& other) const;
	constexpr float Volume() const;
	constexpr float Circumference() const;
	constexpr float Area() const;
};

inline constexpr Sphere::Sphere(const Vector3 center, float radius)
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
inline bool Sphere::IsOverlapping(const Sphere& other) const
{
	// Get distance between two circles' centers
	float centerDistance = this->m_center.DistanceTo(other.m_center);

	// Return result
	return centerDistance < (this->m_radius + other.m_radius);
}

//---------------------------------------------------------------------------------------------------------------------
// Get Volume
// Formula:
//		Volume = (4/3) * Pi * (r^3)
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Sphere::Volume() const
{
	return (4 * PI * Exponential(m_radius, 3)) / 3;
}

//---------------------------------------------------------------------------------------------------------------------
// Get Circumference
// Formula:
//		Circumference = 2 * Pi * r
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Sphere::Circumference() const
{
	return 2 * m_radius * PI;
}

//---------------------------------------------------------------------------------------------------------------------
// Get Area
// Formula:
//		Area = 4 * Pi * (radius^2)
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Sphere::Area() const
{
	return 4 * PI * (m_radius * m_radius);
}
