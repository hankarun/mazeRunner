#include "game.h"

int getCellRowCount()
{
	return screenHeight / cellSize;
}

int getCellColumnCount()
{
	return screenWidth / cellSize;
}

Game::Game()
	: board(getCellColumnCount(), getCellRowCount())
	, boardGenerator(&colorer)
{
	colorer.init(&board);
}

void Game::init()
{
	boardGenerator.init(&board);
}

void Game::resetBoard()
{
	board = Board(getCellColumnCount(), getCellRowCount());
	boardGenerator.init(&board);
	colorer.init(&board);
}

void Game::updateCanvasSize()
{
	int currentscreenWidth = GetScreenWidth();
	int currentscreenHeight = GetScreenHeight();

	if (currentscreenWidth != screenWidth && currentscreenHeight != screenHeight)
	{
		screenWidth = currentscreenWidth;
		screenHeight = currentscreenHeight;
		resetBoard();
	}
}

void Game::update(double time)
{
	updateCanvasSize();

	if (loop && boardGenerator.isFinished())
	{
		resetBoard();
	}

	for (int i = 0; i < speed; ++i)
		boardGenerator.update(&board);
}
