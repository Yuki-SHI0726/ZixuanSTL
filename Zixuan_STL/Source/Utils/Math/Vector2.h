#pragma once

#include <cmath>
#include <iostream>
#include <assert.h>

//---------------------------------------------------------------------------------------------------------------------
// Vector 2D class
//---------------------------------------------------------------------------------------------------------------------
class Vector2
{
public:
	float x;
	float y; 
	
	Vector2(float _x, float _y);
	Vector2(void);
	Vector2(float unit);
	Vector2(const Vector2&) = default;
	Vector2& operator=(const Vector2&) = default;

	// API
	static Vector2 Unit();
	float DistanceTo(const Vector2 other) const;
	float DistanceToSquared(const Vector2 other) const;
	float Length() const;
	float SquaredLength() const;
	float Dot(const Vector2 & right) const;
	static float Dot(const Vector2 & left, const Vector2 & right);
	void Normalize();
	static Vector2 Normalized(const Vector2 & vector);
	Vector2 Normalized();
	void RotateCounterClockwise(float angleRadians);
	void RotateClockwise(float angleRadians);

	// Operators
	Vector2 operator-(const Vector2 & right) const;
	Vector2 operator+(const Vector2 & right) const;
	friend std::ostream& operator<<(std::ostream & Stream, const Vector2 & Vector)
	{
		Stream << "Vector2{" << Roundedf(Vector.x) << "," << Roundedf(Vector.y) << "}";
		return Stream;
	}

private:
	float static Roundedf(float component)
	{
		return std::roundf(component * 1000.f) * 0.001f;
	}
};

//---------------------------------------------------------------------------------------------------------------------
// Ctor with parameters
//---------------------------------------------------------------------------------------------------------------------
inline  Vector2::Vector2(float _x, float _y)
	: x{ _x }
	, y{ _y }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Ctor without parameters
//---------------------------------------------------------------------------------------------------------------------
inline  Vector2::Vector2(void)
	: x{ 0 }
	, y{ 0 }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Generate unit Vector2 with unit
//---------------------------------------------------------------------------------------------------------------------
inline  Vector2::Vector2(float unit)
	: x{ unit }
	, y{ unit }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Generate Unit Vector (1, 1, 1)
//---------------------------------------------------------------------------------------------------------------------
inline  Vector2 Vector2::Unit()
{
	return Vector2(1.0f, 1.0f);
}

//---------------------------------------------------------------------------------------------------------------------
// Operator << overloading
// Formula:
//		Distance = square root((x1 - x2)^2 + (y1 - y2)^2)
//---------------------------------------------------------------------------------------------------------------------
inline  float Vector2::DistanceTo(const Vector2 other) const
{
	return std::sqrtf(DistanceToSquared(other));
}

//---------------------------------------------------------------------------------------------------------------------
// Get squared distance to other vector
// Formula:
//		Distance = (x1 - x2)^2 + (y1 - y2)^2
//---------------------------------------------------------------------------------------------------------------------
inline  float Vector2::DistanceToSquared(const Vector2 other) const
{
	float dx = (this->x - other.x) * (this->x - other.x);	// (x1 - x2)^2
	float dy = (this->y - other.y) * (this->y - other.y);	// (y1 - y2)^2
	return (dx + dy);
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the distance from 0,0,0 to *this.
//---------------------------------------------------------------------------------------------------------------------
inline float Vector2::Length() const
{
	return std::sqrtf(SquaredLength());
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the square of the distance from 0, 0, 0 to * this.
//---------------------------------------------------------------------------------------------------------------------
inline float Vector2::SquaredLength() const
{
	return static_cast<float>((x * x) + (y * y));
}

//---------------------------------------------------------------------------------------------------------------------
// a · b = | a | × | b | × cos(θ)
// θ = acos(a·b), where a and b should be normalized before dot calculation
// 
// Where:
// | a | is the magnitude(length) of vector a
// | b | is the magnitude(length) of vector b
// θ is the angle between a and b
//
// Or
//
// a · b = ax × bx + ay × by
//---------------------------------------------------------------------------------------------------------------------
inline  float Vector2::Dot(const Vector2& right) const
{
	return static_cast<float>((this->x * right.x) +
							  (this->y * right.y));
}
inline  float Vector2::Dot(const Vector2& left, const Vector2& right)
{
	return static_cast<float>((left.x * right.x) +
							  (left.y * right.y));
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the dot product of *this and right.
// The dot product is a float value equal to the magnitudes of the two vectors multiplied together and then multiplied by the cosine of the angle between them.
// a · b = |a||b| × cos(θ)
// Where:
// | a | is the magnitude(length) of vector a
// | b | is the magnitude(length) of vector b
// θ is the angle between a and b
//
// θ = acos([AB]/[|A||B|])
// θ = acos(A·B), (AB) needs to be normalized before dot operation
//
// Or
//
// a · b = ax × bx + ay × by
//---------------------------------------------------------------------------------------------------------------------
inline  void Vector2::Normalize()
{
	if (x == 0.0f && y == 0.0f)
		return;

	float length = Length();
	x /= length;
	y /= length;
}

inline  Vector2 Vector2::Normalized(const Vector2& vector)
{
	Vector2 copy = vector;
	copy.Normalize();
	return copy;
}

inline  Vector2 Vector2::Normalized()
{
	Vector2 copy = *this;
	copy.Normalize();
	return copy;
}


inline  Vector2 Vector2::operator-(const Vector2& right) const
{
	return Vector2
	{		
		this->x - right.x,
		this->y - right.y,
	};
}


inline  Vector2 Vector2::operator+(const Vector2& right) const
{
	return Vector2
	{
		this->x + right.x,
		this->y + right.y,
	};
}


inline void Vector2::RotateCounterClockwise(float angleRadians)
{
	Vector2 copy = *this;
	x = (copy.x * std::cos(angleRadians)) - (copy.y * std::sin(angleRadians));
	y = (copy.x * std::sin(angleRadians)) + (copy.y * std::cos(angleRadians));
}


inline void Vector2::RotateClockwise(float angleRadians)
{
	Vector2 temp = *this;
	x = (temp.x * std::cos(angleRadians)) + (temp.y * std::sin(angleRadians));
	y = (-temp.x * std::sin(angleRadians)) + (temp.y * std::cos(angleRadians));
}
