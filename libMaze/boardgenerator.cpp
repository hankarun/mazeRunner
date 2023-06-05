#include "boardgenerator.h"

namespace maze {
	BoardGenerator::BoardGenerator(BoardColor* colorer)
		: colorer(colorer) {}

	bool BoardGenerator::isFinished() const
	{
		return traverse.empty();
	}

	void BoardGenerator::finish(Board* board)
	{
		while (!traverse.empty())
			update(board);
	}

	void BoardGenerator::init(Board* board)
	{
		traverse = std::stack<Cell*>();
		float startCellX = rand() % (int)board->width;
		float startCellY = rand() % (int)board->height;
		board->home = { startCellX, startCellY };
		float startIndex = board->toIndex(board->home);
		board->cells[startIndex].visited = true;
		traverse.push(&board->cells[startIndex]);

		board->target = { float(rand() % (int)board->width), float(rand() % (int)board->height) };
		colorer->init(board);
	}

	void BoardGenerator::update(Board* board)
	{
		if (traverse.empty())
			return;
		currrent = traverse.top();
		traverse.pop();

		auto neighBours = board->getUnvisitedNeighBours(currrent);
		if (currrent->position == board->target)
		{
			neighBours.clear();
		}

		if (!neighBours.empty())
		{
			colorer->groupIds.at(board->toIndex(currrent->position)) = currentGroupId;
			traverse.push(currrent);
			int count = neighBours.size();
			int lucky = rand() % count;
			auto nextCell = neighBours[lucky];
			nextCell->visited = true;
			colorer->groupIds.at(board->toIndex(nextCell->position)) = currentGroupId;
			currrent->removeWalls(nextCell);
			traverse.push(nextCell);
		}
		else
		{
			currentGroupId++;
		}
	}
}
