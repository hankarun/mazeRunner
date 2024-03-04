#include "boardgenerator.h"

namespace maze {
	constexpr int maxGroupId = 1000;

	BoardGenerator::BoardGenerator() = default;

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
		int startCellX = rand() % (int)board->width;
		int startCellY = rand() % (int)board->height;
		board->home = { startCellX, startCellY };
		float startIndex = board->toIndex(board->home);
		board->cells[startIndex].setVisited(true);
		traverse.push(&board->cells[startIndex]);

		board->target = { int(rand() % (int)board->width), int(rand() % (int)board->height) };
		currentGroupId = 1;
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
			board->getColorer()->setCellGroupId(board->toIndex(currrent->position), currentGroupId);
			traverse.push(currrent);
			int count = neighBours.size();
			int lucky = rand() % count;
			auto nextCell = neighBours[lucky];
			nextCell->setVisited(true);
			board->getColorer()->setCellGroupId(board->toIndex(nextCell->position), currentGroupId);
			currrent->removeWalls(nextCell);
			traverse.push(nextCell);
		}
		else
		{
			currentGroupId++;
		}

		currentGroupId = currentGroupId % maxGroupId;
	}
}
