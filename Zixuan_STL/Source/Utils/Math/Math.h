#pragma once

#define PI 3.14159f	// Pi value

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