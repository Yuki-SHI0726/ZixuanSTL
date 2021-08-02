#pragma once

#include <assert.h>
#include <iostream>
#include <math.h>
#include <algorithm>

//---------------------------------------------------------------------------------------------------------------------
// Vector 3
//---------------------------------------------------------------------------------------------------------------------
class Vector3
{
public:
	float x;
	float y;
	float z;

	constexpr Vector3(float x, float y, float z);
	constexpr Vector3(void);
	constexpr Vector3(float unit);
	constexpr Vector3(const Vector3&) = default;
	constexpr Vector3& operator=(const Vector3&) = default;

	// APIs'
	constexpr static Vector3 Unit();
	constexpr bool operator==(const Vector3& other) const;
	constexpr Vector3 operator-() const;
	float Length() const;
	constexpr float SquaredLength() const;
	constexpr Vector3 Cross(const Vector3& right) const;
	constexpr static Vector3 Cross(const Vector3& left, const Vector3& right);
	constexpr float Dot(const Vector3& right) const;
	constexpr static float Dot(const Vector3& left, const Vector3& right);
	void Scale(const Vector3& other);
	void Scale(float num);
	constexpr void Normalize();
	constexpr Vector3 Normalized();
	constexpr static Vector3 Normalized(const Vector3& vector);
	Vector3 Lerp(const Vector3& other, float interpolant);
	constexpr static Vector3 Lerp(const Vector3& a, const Vector3& b, float interpolant);
	float DistanceTo(const Vector3& other) const;
	constexpr float DistanceToSquared(const Vector3& other) const;
	static Vector3 Clamp(const Vector3& val, const Vector3& low, const Vector3& high);
	void RotateZCounterClockwise(float angle);
	void RotateZClockwise(float angle);

	// Operator with value
	constexpr Vector3 operator+(float num) const;
	constexpr Vector3 operator-(float num) const;
	constexpr Vector3 operator*(float num) const;
	constexpr Vector3 operator/(float num) const;

	// Operator with another vector
	constexpr Vector3 operator+(const Vector3& other) const;
	constexpr Vector3 operator-(const Vector3& other) const;
	Vector3 operator*(const Vector3& other) const;
	constexpr Vector3 operator/(const Vector3& other) const;
	
	// Operator= with another vector
	constexpr void operator+=(const Vector3& other);
	constexpr void operator-=(const Vector3& other);
	void operator*=(const Vector3& other);
	constexpr void operator/=(const Vector3& other);

	// Operator= with value
	constexpr void operator+=(float num);
	constexpr void operator-=(float num);
	constexpr void operator*=(float num);
	constexpr void operator/=(float num);

	// Getters
	constexpr float X() const { return x; }
	constexpr float Y() const { return y; }
	constexpr float Z() const { return z; }

	// Operator cout
	friend std::ostream& operator<<(std::ostream& stream, const Vector3& vector)
	{
		stream << "Vector3{" << vector.X() << "," << vector.Y() << "," << vector.Z() << "}";
		return stream;
	}
};

//---------------------------------------------------------------------------------------------------------------------
// Ctor with parameters
//---------------------------------------------------------------------------------------------------------------------
inline constexpr Vector3::Vector3(float x, float y, float z)
	: x{ x }
	, y{ y }
	, z{ z }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Ctor without params
//---------------------------------------------------------------------------------------------------------------------
inline constexpr Vector3::Vector3(void)
	: x{ 0.0f }
	, y{ 0.0f }
	, z{ 0.0f }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Generate unit vector3 with unit
//---------------------------------------------------------------------------------------------------------------------
inline constexpr Vector3::Vector3(float Unit)
	: x{ Unit }
	, y{ Unit }
	, z{ Unit }
{
}

//---------------------------------------------------------------------------------------------------------------------
// Generate Unit Vector
//---------------------------------------------------------------------------------------------------------------------
constexpr inline Vector3 Vector3::Unit()
{
	return Vector3(1, 1, 1);
}

//---------------------------------------------------------------------------------------------------------------------
// Returns if this vector3 object has the same x,y,z to the other
//---------------------------------------------------------------------------------------------------------------------
constexpr inline bool Vector3::operator==(const Vector3& other) const
{
	return (this->x == other.x && 
			this->y == other.y && 
			this->z == other.z);
}

//---------------------------------------------------------------------------------------------------------------------
// Returns a vector of negatived x,y,z
//---------------------------------------------------------------------------------------------------------------------
constexpr inline Vector3 Vector3::operator-() const
{
	return Vector3{ -x, -y, -z };
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the distance from 0,0,0 to *this.
//---------------------------------------------------------------------------------------------------------------------
inline float Vector3::Length() const
{
	return sqrtf(SquaredLength());
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the square of the distance from 0, 0, 0 to * this.
//---------------------------------------------------------------------------------------------------------------------
constexpr inline float Vector3::SquaredLength() const
{
	return static_cast<float>((x * x) + (y * y) + (z * z));
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
constexpr inline float Vector3::Dot(const Vector3& right) const
{
	return static_cast<float>((this->x * right.x) +
							  (this->y * right.y) +
							  (this->z * right.z));
}

inline constexpr float Vector3::Dot(const Vector3& left, const Vector3& right)
{
	return static_cast<float>((left.x * right.x) +
							  (left.y * right.y) +
							  (left.z * right.z));
}

//---------------------------------------------------------------------------------------------------------------------
// Multiplies vectors
//---------------------------------------------------------------------------------------------------------------------
inline void Vector3::Scale(const Vector3& other)
{
	*this *= other;
}

//---------------------------------------------------------------------------------------------------------------------
// Multiplies num
//---------------------------------------------------------------------------------------------------------------------
inline void Vector3::Scale(float num)
{
	*this *= num;
}

//---------------------------------------------------------------------------------------------------------------------
// Normalize the vector (make the length = 1)
//---------------------------------------------------------------------------------------------------------------------
inline constexpr void Vector3::Normalize()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return;

	float length = Length();
	x /= length;
	y /= length;
	z /= length;
}
inline constexpr Vector3 Vector3::Normalized()
{
	Vector3 copy = *this;
	copy.Normalize();
	return copy;
}

//---------------------------------------------------------------------------------------------------------------------
// Normalize the vector (make the length = 1)
//---------------------------------------------------------------------------------------------------------------------
inline constexpr Vector3 Vector3::Normalized(const Vector3& vector)
{
	Vector3 copy = vector;
	copy.Normalize();
	return copy;
}

//---------------------------------------------------------------------------------------------------------------------
// Linearly interpolates between this point and another
// a + t(b-a)
//---------------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::Lerp(const Vector3& other, float interpolant)
{
	interpolant = std::clamp(interpolant, 0.0f, 1.0f);
	return *this + ((other - *this) * interpolant);
}

//---------------------------------------------------------------------------------------------------------------------
// Get distance to other vector
// Formula:
//		Distance = square root((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)
//---------------------------------------------------------------------------------------------------------------------
inline float Vector3::DistanceTo(const Vector3& other) const
{
	return sqrtf(DistanceToSquared(other));
}

//---------------------------------------------------------------------------------------------------------------------
// Get squared distance to other vector
// Formula:
//		Distance = (x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2
//---------------------------------------------------------------------------------------------------------------------
inline constexpr float Vector3::DistanceToSquared(const Vector3& other) const
{
	//return (*this - other).SquaredLength();

	float dx = (this->x - other.x) * (this->x - other.x);	// (x1 - x2)^2
	float dy = (this->y - other.y) * (this->y - other.y);	// (y1 - y2)^2
	float dz = (this->z - other.z) * (this->z - other.z);	// (y1 - y2)^2
	return (dx + dy + dz);
}

//---------------------------------------------------------------------------------------------------------------------
// If val is less than low, return low, if val is greater than high, return high, if neither, return val
//---------------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::Clamp(const Vector3& val, const Vector3& low, const Vector3& high)
{
	if (val.Length() < low.Length())
		return low.Length();

	else if (val.Length() > high.Length())
		return high.Length();

	else
		return val.Length();
}

//---------------------------------------------------------------------------------------------------------------------
// Rotate Z-axis
//---------------------------------------------------------------------------------------------------------------------
inline void Vector3::RotateZCounterClockwise(float radians)
{
	Vector3 temp = *this;
	x = (temp.x * std::cos(radians)) - (temp.y * std::sin(radians));
	y = (temp.x * std::sin(radians)) + (temp.y * std::cos(radians));
}

inline void Vector3::RotateZClockwise(float angle)
{
	Vector3 temp = *this;
	x = (temp.x * std::cos(angle)) + (temp.y * std::sin(angle));
	y = (-temp.x * std::sin(angle)) + (temp.y * std::cos(angle));
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the cross product of *this and right.
// The cross product of two vectors results in a third vector which is perpendicular to the two input vectors.
// The result's magnitude is equal to the magnitudes of the two inputs multiplied together and then multiplied by the sine of the angle between the inputs. 
// You can determine the direction of the result vector using the "left hand rule".
// a × b = |a| |b| sin(θ) n
// |a × b| = |a| |b| sin(θ)
//
// | a | is the magnitude(length) of vector a
// | b | is the magnitude(length) of vector b
// θ is the angle between a and b
// n is the unit vector at right angles to both a and b
//---------------------------------------------------------------------------------------------------------------------
constexpr inline Vector3 Vector3::Cross(const Vector3& right) const
{
	float x = (this->y * right.z) - (this->z * right.y);
	float y = (this->z * right.x) - (this->x * right.z);
	float z = (this->x * right.y) - (this->y * right.x);
	return Vector3{ x,y,z };
}
inline constexpr Vector3 Vector3::Cross(const Vector3& left, const Vector3& right)
{
	float x = (left.y * right.z) - (left.z * right.y);
	float y = (left.z * right.x) - (left.x * right.z);
	float z = (left.x * right.y) - (left.y * right.x);
	return Vector3{ x,y,z };
}


constexpr inline void Vector3::operator+=(const Vector3& other)
{
	*this = *this + other;
}


constexpr inline void Vector3::operator-=(const Vector3& other)
{
	*this = *this - other;
}


inline void Vector3::operator*=(const Vector3& other)
{
	*this = *this * other;
}


constexpr inline void Vector3::operator/=(const Vector3& other)
{
	*this = *this / other;
}


constexpr inline void Vector3::operator+=(float num)
{
	*this = *this + num;
}


constexpr inline void Vector3::operator-=(float num)
{
	*this = *this - num;
}


constexpr inline void Vector3::operator*=(float num)
{
	*this = *this * num;
}


constexpr inline void Vector3::operator/=(float num)
{
	*this = *this / num;
}


constexpr inline Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3
	{
		this->x + other.x,
			this->y + other.y,
			this->z + other.z
	};
}


constexpr inline Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3
	{
		this->x - other.x,
			this->y - other.y,
			this->z - other.z
	};
}


inline Vector3 Vector3::operator*(const Vector3& other) const
{
	return Vector3
	{
		this->x * other.x,
			this->y * other.y,
			this->z * other.z
	};
}


constexpr inline Vector3 Vector3::operator/(const Vector3& other) const
{
	return Vector3
	{
		this->x / other.x,
			this->y / other.y,
			this->z / other.z
	};
}


constexpr inline Vector3 Vector3::operator+(float num) const
{
	return Vector3
	{
		this->x + num,
			this->y + num,
			this->z + num
	};
}


constexpr inline Vector3 Vector3::operator-(float num) const
{
	return Vector3
	{
		this->x - num,
			this->y - num,
			this->z - num
	};
}


constexpr inline Vector3 Vector3::operator*(float num) const
{
	return Vector3
	{
		this->x * num,
			this->y * num,
			this->z * num
	};
}


constexpr inline Vector3 Vector3::operator/(float num) const
{
	return Vector3
	{
		this->x / num,
			this->y / num,
			this->z / num
	};
}


constexpr Vector3 operator+(float num, const Vector3& other)
{
	return other + num;
}


constexpr Vector3 operator-(float num, const Vector3& other)
{
	return other - num;
}

Vector3 operator*(float num, const Vector3& other)
{
	return other * num;
}


Vector3 operator/(float num, const Vector3& other)
{
	return other / num;
}
