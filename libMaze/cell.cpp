#include "cell.h"

namespace maze
{
	// . 0 .
	// 1   2
	// . 3 .

	bool Cell::getWall(int index) const
	{
		return (data & 0x0F) & 1 << index;
	}

	void Cell::setWall(int index, bool value)
	{
		if (value)
		{
			data |= 1 << index;
		}
		else
		{
			data &= ~(1 << index);
		}
	}

	void Cell::setVisited(bool value)
	{
		if (value)
		{
			data |= 0x10;
		}
		else
		{
			data &= 0xEF;
		}
	}

	bool Cell::isVisited() const
	{
		return data & 0x10;
	}

	void Cell::removeWalls(Cell* n)
	{
		if (position.x == n->position.x - 1)
		{
			setWall(2, false);
			n->setWall(1, false);
		}

		if (n->position.x == position.x - 1)
		{
			setWall(1, false);
			n->setWall(2, false);
		}

		if (position.y == n->position.y - 1)
		{
			setWall(3, false);
			n->setWall(0, false);
		}

		if (n->position.y == position.y - 1)
		{
			setWall(0, false);
			n->setWall(3, false);
		}
	}
}
