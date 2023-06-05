#include "boardsolver.h"

namespace maze {
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
}
