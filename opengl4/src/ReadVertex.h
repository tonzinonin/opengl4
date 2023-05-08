#pragma once

#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

class VertexUnion
{
private:
	const char* Path;
	std::vector<float> vertexArr;
public:
	VertexUnion(const char* path) : Path(path)
	{
		std::ifstream file(path);
		std::string line;
		while(getline(file , line))
		{
			if (line.empty())
			{
				continue;
			}
			std::istringstream iss(line);
			std::string token;
			while (std::getline(iss, token, ','))
			{
				vertexArr.push_back(std::stof(token));
			}
		}
	}
	inline unsigned int GetCount() const { return vertexArr.size(); }
	void value(float *arr) const
	{
		int index = 0;
		for (auto it : vertexArr)
		{
			arr[index++] = it;
		}
	}
};