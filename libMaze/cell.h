#pragma once
#include "common.h"

namespace maze
{
	class Cell
	{
	public:
		Position position = {};
		char data = 0x0F;
		bool getWall(int index) const;
		void setWall(int index, bool value);
		void setVisited(bool value);
		bool isVisited() const;
		// . 1 .
		// 2   3
		// . 4 .
		
		// . 0 .
		// 1   2
		// . 3 .
		void removeWalls(Cell* n);
	};
}