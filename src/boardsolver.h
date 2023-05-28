#pragma once
#include "board.h"

class BoardSolver
{
public:
	BoardSolver(Board* board);

	void iterate(int count);

	bool isSolved() const;

	const std::vector<Cell*>& getSolution() const;
private:
	Board* board;
	bool solved = false;
	std::vector<Cell*> solution;
};