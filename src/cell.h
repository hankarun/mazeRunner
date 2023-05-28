#pragma once
#include "common.h"

class Cell
{
public:
	Position position = {};
	bool visited = false;
	// . 1 .
	// 2   3
	// . 4 .
	std::array<bool, 4> walls = { true, true, true, true };
	// . 0 .
	// 1   2
	// . 3 .
	void removeWalls(Cell* n);
};