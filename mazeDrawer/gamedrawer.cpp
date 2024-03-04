#include "gamedrawer.h"

#include "raylib.h"

GameDrawer::GameDrawer(maze::Game* game)
	: game(game)
{
}

void GameDrawer::draw(maze::BoardSolver* solver)
{
	for (auto cell : solver->getSolution())
	{
		float x = cell->position.x * game->cellSize;
		float y = cell->position.y * game->cellSize;
		DrawRectangle(x, y, game->cellSize, game->cellSize, BLUE);
	}
}


void GameDrawer::draw(maze::Board* board, bool showColor)
{
	auto colorer = board->getColorer();
	int cellSize = game->cellSize;

	for (const auto& cell : board->cells)
	{
		float x = cell.position.x * cellSize + offsetX;
		float y = cell.position.y * cellSize + offsetY;

		if (cell.isVisited())
		{
			Color c(WHITE);
			if (showColor)
			{
				auto cellColor = colorer->getGroupColor(board->toIndex(cell.position));
				c = {cellColor.r, cellColor.g, cellColor.b, cellColor.a};
			}
			DrawRectangle(x, y, cellSize, cellSize, c);
		}
		if (cell.getWall(0))
			DrawLine(x, y, x + cellSize, y, BLACK);
		if (cell.getWall(1))
			DrawLine(x, y, x, y + cellSize, BLACK);
		if (cell.getWall(2))
			DrawLine(x + cellSize, y, x + cellSize, y + cellSize, BLACK);
		if (cell.getWall(3))
			DrawLine(x, y + cellSize, x + cellSize, y + cellSize, BLACK);
	}

	Color targetColor{
		255,
		0,
		0,
		255
	};
	int targetSize = cellSize - 4;
	DrawRectangle(board->target.x * cellSize + 2 + offsetX, board->target.y * cellSize + 2 + offsetY, targetSize, targetSize, targetColor);

	targetColor = Color{
		0,
		255,
		0,
		255
	};
	int homeIndex = board->home.x + board->home.y * board->width;
	const auto& homeCell = board->cells.at(homeIndex);
	int homeSize = cellSize - 4;
	DrawRectangle(homeCell.position.x * cellSize + 2 + offsetX, homeCell.position.y * cellSize + 2 + offsetY, homeSize, homeSize, targetColor);
}

void GameDrawer::draw(maze::BoardGenerator* generator)
{
	int cellSize = game->cellSize;

	if (!generator->traverse.empty())
	{
		auto top = generator->traverse.top();
		float x = top->position.x * cellSize + offsetX;
		float y = top->position.y * cellSize + offsetY;
		DrawRectangle(x, y, cellSize, cellSize, GREEN);
	}
}