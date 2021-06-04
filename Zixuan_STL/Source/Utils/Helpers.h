#pragma once
#include <cassert>
#include <type_traits>
#include <numeric>
#include <iostream>

#define PI 3.14159f	// Pi value

//--------------------------------------------------------------------------------------------
// Convert a decimal number to character
//--------------------------------------------------------------------------------------------
template<typename Decimal>
constexpr char DecimalToChar(Decimal decimal)
{
	static_assert(std::is_arithmetic_v<Decimal>, "Must be a decimal type to convert to char");
	return static_cast<char>(decimal + 48);		// Char to decimal offset val is 48
}

//--------------------------------------------------------------------------------------------
// Get input according to the prompt
// kMax is a default parameter to max
//--------------------------------------------------------------------------------------------
template<typename Type>
constexpr Type GetInput(const char* kPrompt, const Type kLimit, const Type kMax = std::numeric_limits<Type>::max())
{
	Type num = 0;
	while (true)
	{
		std::cout << "How many " << kPrompt << "? (At least " << kLimit << ")" << std::endl;
		std::cin >> num;

		if (num < kLimit)
		{
			std::cout << "Too few, try again." << std::endl;
			continue;
		}

		if (kMax != std::numeric_limits<Type>::max() && num > kMax)
		{
			std::cout << "Too many, try again." << std::endl;
			continue;
		}

		return num;
	}
}

//--------------------------------------------------------------------------------------------
// Return exponential
//--------------------------------------------------------------------------------------------
template<typename Type>
constexpr Type Exponential(Type base, Type exponent)
{
	static_assert(std::is_arithmetic_v<Type>, "Must be a arithmetic type to get exponential");

	if (exponent == 0)
		return 1;

	else if (exponent == 1)
		return base;

	else
	{
		Type result = base;
		for (Type i = 0; i < exponent - 1; ++i)
			result *= base;
		return result;
	}
}

//--------------------------------------------------------------------------------------------
// Convert a radians to degrees
// Degrees = Radians * 180 / Pi
//--------------------------------------------------------------------------------------------
constexpr float RadiansToDegrees(float radians)
{
	return (radians * 180 / PI);
}

//--------------------------------------------------------------------------------------------
// Convert a degrees to radians
// Radians = Degrees * Pi / 180
//--------------------------------------------------------------------------------------------
constexpr float DegreesToRadius(float degrees)
{
	return (degrees * PI / 180);
}

//--------------------------------------------------------------------------------------------
// Return a random possibility according to the given chance
//	-possibility: From 0 to 100, 0 means impossible to happen, 100 means always happen
//--------------------------------------------------------------------------------------------
inline bool RandomPossibility(unsigned int possibility)
{
	assert(possibility >= 0 && possibility <= 100);
	return unsigned(rand() % 100 + 1) <= possibility;
}

//--------------------------------------------------------------------------------------------
// Swap two variables using a temp
//--------------------------------------------------------------------------------------------
template <typename Type>
inline void Swap(Type& left, Type& right)
{
	Type temp = std::move(left);
	left = std::move(right);
	right = std::move(temp);
}