#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <unordered_map>

//-----------------------------------------------------------------------------------------------------------
// Custom logger
//-----------------------------------------------------------------------------------------------------------
#define LOG(category, format, ...) { Log::Get().LogInfo(category, __LINE__, __FILE__, format, __VA_ARGS__); } 

class Log
{
public:
	enum class Color : int
	{
		kBlack = 0,
		kBlue = 1,
		kGreen = 2,
		kCyan = 3,
		kRed = 4,
		kMagenta = 5,
		kBrown = 6,
		kLightGray = 7,
		kDarkGray = 8,
		kLightBlue = 9,
		kLightGreen = 10,
		kLightCyan = 11,
		kLightRed = 12,
		kLightMagenta = 13,
		kYellow = 14,
		kWhite = 15,
		kBlueBackground = 63,
		kRedBackground = BACKGROUND_RED,

		kNum = 16
	};

private:
	HANDLE m_consoleHandle;
	Color m_currentColor;
	std::unordered_map<std::string, Color> m_category;     // map for log categories

public:
	// Getter
	static Log& Get();

	// Setter
	void SetColor(Color color);

	// API
	void LogInfo(const std::string& category, int line, const char* file, const char* format, ...);
	template<typename Type>	void PrintInColor(Color color, const Type& message);
	template<typename Type>	void Print(const Type& message);

private:
	Log();
};

//-----------------------------------------------------------------------------------------------------------
// Immediately set color back to current after printed message
//-----------------------------------------------------------------------------------------------------------
template<typename Type>
inline void Log::PrintInColor(Color color, const Type& message)
{
	SetConsoleTextAttribute(m_consoleHandle, static_cast<int>(color));
	std::cout << message;
	SetConsoleTextAttribute(m_consoleHandle, static_cast<int>(m_currentColor));
}

template<typename Type>
inline void Log::Print(const Type& message)
{
	std::cout << message;
}

/*

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int k = 1; k < 255; k++)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(hConsole, k);
		std::cout << k << " I want to be nice today!" << std::endl;
	}

*/