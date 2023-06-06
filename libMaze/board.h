#pragma once
#include "common.h"
#include "cell.h"
#include "boardcolors.h"

#include <vector>

namespace maze
{
	class Board
	{
	public:
		Board(int width, int height);

		int toIndex(const Position& pos);

		std::vector<Cell*> getUnvisitedNeighBours(Cell* p);

		void setColorer(BoardColor colorer);

		BoardColor* getColorer();
	public:
		std::vector<Cell> cells;
		float width = 50;
		float height = 50;
		Position target = {};
		Position home = {};
		BoardColor boardColorer;
	};
}