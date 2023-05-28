#include "gamedrawer.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

GameDrawer::GameDrawer(Game* game)
	: game(game)
{
}

void GameDrawer::draw(BoardSolver* solver)
{
	for (auto cell : solver->getSolution())
	{
		float x = cell->position.x * game->cellSize;
		float y = cell->position.y * game->cellSize;
		DrawRectangle(x, y, game->cellSize, game->cellSize, BLUE);
	}
}

void GameDrawer::draw(Board* board, bool showColor, BoardColor* colorer)
{
	int cellSize = game->cellSize;
	for (const auto& cell : board->cells)
	{
		float x = cell.position.x * cellSize;
		float y = cell.position.y * cellSize;

		if (cell.visited)
		{
			Color c(WHITE);
			if (showColor)
				c = colorer->getGroupColor(board->toIndex(cell.position));
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

void GameDrawer::draw(BoardGenerator* generator)
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

void GameDrawer::drawInfo(Game* game)
{
	DrawRectangle(20, 50, 250, 180, Fade(SKYBLUE, 0.9f));
	DrawRectangleLines(20, 50, 250, 180, BLUE);

	GuiCheckBox(Rectangle { 24, 56, 24, 24 }, "Show Color", &game->showColor);
	GuiCheckBox(Rectangle { 24, 88, 24, 24 }, "Loop", &game->loop);
	static bool ValueBOx004EditMode = false;
	static bool ValueBOx005EditMode = false;
	if (GuiSpinner(Rectangle { 104, 120, 120, 24 }, "Speed", & game->speed, 0, 100, ValueBOx004EditMode)) ValueBOx004EditMode = !ValueBOx004EditMode;
	if (GuiSpinner(Rectangle { 104, 184, 120, 24 }, "Cell Size", &game->cellSize, 0, 100, ValueBOx005EditMode)) ValueBOx005EditMode = !ValueBOx005EditMode;

	if (GuiButton(Rectangle{ 112, 152, 112, 24 }, "Finish Generating"))
	{
		if (game->boardGenerator.isFinished())
		{
			game->resetBoard();
		}

		game->boardGenerator.finish(&game->board);
	}
	if (GuiButton(Rectangle{ 24, 152, 80, 24 }, "Reset"))
	{
		game->resetBoard();
		game->speed = 1;
	}
}
