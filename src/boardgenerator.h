#pragma once
#include <stack>

#include "cell.h"
#include "board.h"
#include "boardcolors.h"

class BoardGenerator
{
public:
	BoardGenerator(BoardColor* colorer);

	bool isFinished() const;

	void finish(Board* board);

	void init(Board* board);

	void update(Board* board);

public:
	std::stack<Cell*> traverse;
	Cell* currrent = nullptr;
	int currentGroupId = 1;
	BoardColor* colorer;
};