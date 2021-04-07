#pragma once
#include <cassert>
#include <type_traits>
#include <numeric>
#include <iostream>

//--------------------------------------------------------------------------------------------
// Convert a decimal number to character
//--------------------------------------------------------------------------------------------
template<typename Decimal>
constexpr char DecimalToChar(Decimal decimal)
{
	static_assert(std::is_arithmetic_v<Decimal>, "Must be a decimal type to convert to char");
	char result = static_cast<char>(decimal + 48);
	return result;
}

//--------------------------------------------------------------------------------------------
// Get input according to the prompt
// kMax is a default parameter to max
//--------------------------------------------------------------------------------------------
template<typename T>
constexpr T GetInput(const char* kPrompt, const T kLimit, const T kMax = std::numeric_limits<T>::max())
{
	T num = 0;
	while (true)
	{
		std::cout << "How many " << kPrompt << "? (At least " << kLimit << ")" << std::endl;
		std::cin >> num;

		if (num < kLimit)
		{
			std::cout << "Too few, try again." << std::endl;
			continue;
		}

		if (kMax != std::numeric_limits<T>::max() && num > kMax)
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
template<typename T>
constexpr T Exponential(T base, T exponent)
{
	static_assert(std::is_arithmetic_v<T>, "Must be a arithmetic type to get exponential");

	if (exponent == 0)
		return 1;

	else if (exponent == 1)
		return base;

	else
	{
		T result = base;
		for (T i = 0; i < exponent - 1; ++i)
			result *= base;
		return result;
	}
}