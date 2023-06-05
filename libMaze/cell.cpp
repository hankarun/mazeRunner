#include "cell.h"

namespace maze
{
	// . 0 .
	// 1   2
	// . 3 .

	void Cell::removeWalls(Cell* n)
	{
		if (position.x == n->position.x - 1)
		{
			walls[2] = false;
			n->walls[1] = false;
		}

		if (n->position.x == position.x - 1)
		{
			walls[1] = false;
			n->walls[2] = false;
		}

		if (position.y == n->position.y - 1)
		{
			walls[3] = false;
			n->walls[0] = false;
		}

		if (n->position.y == position.y - 1)
		{
			walls[0] = false;
			n->walls[3] = false;
		}
	}
}
