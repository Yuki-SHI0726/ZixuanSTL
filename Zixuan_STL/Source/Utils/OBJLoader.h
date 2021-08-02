#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Log.h"

#include <vector>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <cstring>

namespace Zixuan_STL
{
static constexpr size_t kPositionIndex = 0;
static constexpr size_t kTextureCoordsIndex = 1;
static constexpr size_t kVertexNormalsIndex = 2;

enum class FaceType
{
	kPositionOnly,					// f 1 2 3				� Vertex position only
	kPositionTextureCoords,			// f 1/4 2/3 3/1		� Vertex position and texture coordinates
	kPositionNormal,				// f 1//3 2//7 3//4		� Vertex position and normals
	kPositionTextureCoordsNormal	// f 1/4/3 2/3/7 3/1/4  � Vertex position, texture coordinates, and normals
};

struct Vertex
{
	Vector3 pos;
	Vector2 textureCoords;
	Vector3 vertexNormals;
	Vector3 color;

	Vertex() = default;
	Vertex(Vector3 _pos)
		: pos{ _pos }
		, textureCoords{}
		, vertexNormals{}
		, color{}
	{}

	Vertex(Vector3 _pos, Vector2 coords, Vector3 normals, Vector3 color)
		: pos{ _pos }
		, textureCoords{ coords }
		, vertexNormals{ normals }
		, color{ color }
	{}
};

//-------------------------------------------------------------------------------------------------------------------------------------
// Param:
//	- input: A string
//
// Return:
//	- The first word got ended with a " "
//-------------------------------------------------------------------------------------------------------------------------------------
std::string FirstWord(const std::string& input)
{
	size_t firstSpacePos = input.find_first_of(" ");
	if (firstSpacePos == std::string::npos)
		return input;

	return input.substr(0, firstSpacePos);
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Param:
//	- input: A string
//
// Return:
//	- The string without the first word ended with a " "
//-------------------------------------------------------------------------------------------------------------------------------------
std::string PoppedFirstWord(const std::string& input)
{
	size_t firstSpacePos = input.find_first_of(" ");
	if (firstSpacePos == std::string::npos)
		return input;

	return input.substr(firstSpacePos + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Purpose:
//  - Write a vector of string into the data buffer
// 
// Param:
//	- input: A string to Split
//  - kDivider: The divider used to split the string
//  - data: Write the result vector into this data
//-------------------------------------------------------------------------------------------------------------------------------------
void Split(std::string input, const char kDivider, std::vector<std::string>& data)
{
	// Loop
	while (true)
	{
		// Find the first kDivider position
		size_t spacePos = input.find_first_of(kDivider);

		// If there is none kDivider in the string, it's time to add last float item to the vector and return
		if (spacePos == std::string::npos)
		{
			data.emplace_back(input);
			return;
		}

		// Add first number to the result vector
		data.emplace_back(input.substr(0, spacePos));

		// Move on by cutting the first number off
		input = input.substr(spacePos + 1);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Param:
//	- vec: A templated vector contains desired elements
//	- inputIndex: A string of number
//
// Return:
//	- A const ref value from vec at the inputIndex
//-------------------------------------------------------------------------------------------------------------------------------------
template<typename Type>
const Type& GetVal(const std::vector<Type>& vec, const std::string& inputIndex)
{
	size_t index = static_cast<size_t>(std::stoi(inputIndex));
	--index;
	return vec[index];
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Accepts a filename to load and two vectors for the vertex and index data to be stored within. 
// If an error occurs (file not found, malformed data, etc.) then this function should return false.
//-------------------------------------------------------------------------------------------------------------------------------------
bool LoadOBJFile(const char* pFilename,
	std::vector<Vertex>& outVertices,
	std::vector<uint32_t>& outIndices)
{
	//----------------------------------------------------------------
	// Load the obj file
	//----------------------------------------------------------------
	std::ifstream fileInput{ pFilename, std::ios_base::in };
	if (!fileInput.is_open())
	{
		Log::Get().PrintInColor(Log::Color::kRed, "Error: Cannot open OBJ file\n");
		return false;
	}

	//----------------------------------------------------------------
	// Data
	//----------------------------------------------------------------
	std::vector<Vector3> positions;
	std::vector<Vector2> textureCoords;
	std::vector<Vector3> vertexNormals;
	std::vector<std::string> currData;
	std::vector<std::vector<Vector3>> faces;
	std::string thisLine;

	//----------------------------------------------------------------
	// Parse!
	//----------------------------------------------------------------
	while (std::getline(fileInput, thisLine))
	{
		currData.clear();

		//----------------------------------------------------------------
		// Vertex position
		//----------------------------------------------------------------
		if (FirstWord(thisLine) == "v")
		{
			Split(PoppedFirstWord(thisLine), ' ', currData);
			Vector3 currPosition;
			currPosition.x = std::stof(currData[0]);
			currPosition.y = std::stof(currData[1]);
			currPosition.z = std::stof(currData[2]);
			positions.emplace_back(currPosition);
		}

		//----------------------------------------------------------------
		// Texture coordinates
		//----------------------------------------------------------------
		else if (FirstWord(thisLine) == "vt")
		{
			Split(PoppedFirstWord(thisLine), ' ', currData);
			Vector2 currCoords;
			currCoords.x = std::stof(currData[0]);
			currCoords.y = std::stof(currData[1]);
			textureCoords.emplace_back(currCoords);
		}

		//----------------------------------------------------------------
		// Vertex normal
		//----------------------------------------------------------------
		else if (FirstWord(thisLine) == "vn")
		{
			Split(PoppedFirstWord(thisLine), ' ', currData);
			Vector3 currNormals;
			currNormals.x = std::stof(currData[0]);
			currNormals.y = std::stof(currData[1]);
			currNormals.z = std::stof(currData[2]);
			vertexNormals.emplace_back(currNormals);
		}

		//----------------------------------------------------------------
		// Generate Face
		//----------------------------------------------------------------
		else if (FirstWord(thisLine) == "f")
		{
			// Data
			Split(PoppedFirstWord(thisLine), ' ', currData);
			std::vector<Vector3> currFaces;

			// Walk through every face info
			for (const std::string& faceStr : currData)
			{
				std::vector<std::string> vertexInfo;
				Split(faceStr, '/', vertexInfo);

				Vector3 currFace;
				currFace.x = std::stof(vertexInfo[0]) - 1;	// Position
				currFace.y = std::stof(vertexInfo[1]) - 1;	// Texture coords
				currFace.z = std::stof(vertexInfo[2]) - 1;	// Vertex normal
				currFaces.emplace_back(currFace);
			}
			faces.emplace_back(currFaces);
		}
	}

	//----------------------------------------------------------------
	// Reindex
	//----------------------------------------------------------------
	for (const Vector3& v : positions)
		outVertices.emplace_back(v);

	for (const std::vector<Vector3>& triangle : faces)
	{
		for (const Vector3& face : triangle)
		{
			outVertices[(size_t)face.x].textureCoords = textureCoords[(size_t)face.y];
			outVertices[(size_t)face.x].vertexNormals = vertexNormals[(size_t)face.z];
			outIndices.emplace_back((uint32_t)face.x);
		}
	}

	//----------------------------------------------------------------
	// Close OBJ file, return true
	//----------------------------------------------------------------
	fileInput.close();
	return true;
}
}