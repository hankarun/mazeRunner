#pragma once
#include <array>

namespace maze
{
	struct Color
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	struct Position
	{
		int x;
		int y;
	};

}
bool operator==(const maze::Position& lhs, const maze::Position& rhs);