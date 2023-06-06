#include "board.h"

namespace maze
{
	Board::Board(int width, int height)
		: width(width), height(height)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				Cell cell;
				cell.position = { (float)i, (float)j };
				cells.push_back(cell);
			}
		}
		boardColorer.init(this);
	}

	int Board::toIndex(const Position& pos)
	{
		return pos.x + pos.y * width;
	}

	std::vector<Cell*> Board::getUnvisitedNeighBours(Cell* p)
	{
		std::vector<Cell*> neighbours;

		// north (x, y -1)
		if (p->position.y > 0)
		{
			int index = p->position.x + (p->position.y - 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// south (x, y + 1)
		if (p->position.y < (height - 1))
		{
			int index = p->position.x + (p->position.y + 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// east (x - 1, y)
		if (p->position.x > 0)
		{
			int index = p->position.x - 1 + p->position.y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// west (x + 1, y)
		if (p->position.x < (width - 1))
		{
			int index = p->position.x + 1 + p->position.y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		return neighbours;
	}

	void Board::setColorer(BoardColor colorer)
	{
		boardColorer = colorer;
		colorer.init(this);
	}
	BoardColor* Board::getColorer()
	{
		return &boardColorer;
	}
}
