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
		float x = cell.position.x * cellSize;
		float y = cell.position.y * cellSize;

		if (cell.visited)
		{
			Color c(WHITE);
			if (showColor)
			{
				auto cellColor = colorer->getGroupColor(board->toIndex(cell.position));
				c = {cellColor.r, cellColor.g, cellColor.b, cellColor.a};
			}
			DrawRectangle(x, y, cellSize, cellSize, c);
		}
		if (cell.walls[0])
			DrawLine(x, y, x + cellSize, y, BLACK);
		if (cell.walls[1])
			DrawLine(x, y, x, y + cellSize, BLACK);
		if (cell.walls[2])
			DrawLine(x + cellSize, y, x + cellSize, y + cellSize, BLACK);
		if (cell.walls[3])
			DrawLine(x, y + cellSize, x + cellSize, y + cellSize, BLACK);
	}

	Color targetColor{
		255,
		0,
		0,
		255
	};
	DrawRectangle(board->target.x * cellSize, board->target.y * cellSize, cellSize, cellSize, targetColor);

	targetColor = Color{
		0,
		255,
		0,
		255
	};
	int homeIndex = board->home.x + board->home.y * board->width;
	const auto& homeCell = board->cells.at(homeIndex);
	DrawRectangle(homeCell.position.x * cellSize, homeCell.position.y * cellSize, cellSize, cellSize, targetColor);
}

void GameDrawer::draw(maze::BoardGenerator* generator)
{
	int cellSize = game->cellSize;

	if (!generator->traverse.empty())
	{
		auto top = generator->traverse.top();
		float x = top->position.x * cellSize;
		float y = top->position.y * cellSize;
		DrawRectangle(x, y, cellSize, cellSize, GREEN);
	}
}