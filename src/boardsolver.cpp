#include "boardsolver.h"

BoardSolver::BoardSolver(Board* board)
	: board(board) {

}

void BoardSolver::iterate(int count)
{

}

bool BoardSolver::isSolved() const
{
	return solved;
}

const std::vector<Cell*>& BoardSolver::getSolution() const
{
	return solution;
}
