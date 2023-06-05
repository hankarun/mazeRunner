#pragma once
#include "common.h"
#include "cell.h"

#include <vector>

namespace maze
{
	class Board
	{
	public:
		Board(int width, int height);

		int toIndex(const Position& pos);

		std::vector<Cell*> getUnvisitedNeighBours(Cell* p);

	public:
		std::vector<Cell> cells;
		float width = 50;
		float height = 50;
		Position target = {};
		Position home = {};
	};
}