#include "game.h"

Game::Game()
	: cellSize(10)
	, screenHeight(800)
	, screenWidth(800)
	, board(getCellColumnCount(), getCellRowCount())
	, boardGenerator(&colorer)
{
	colorer.init(&board);
}

void Game::init()
{
	boardGenerator.init(&board);
}

void Game::reset()
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
		reset();
	}
}

void Game::update(double time)
{
	updateCanvasSize();

	if (loop && boardGenerator.isFinished())
	{
		reset();
	}

	for (int i = 0; i < speed; ++i)
		boardGenerator.update(&board);
}

int Game::getCellRowCount()
{
	return screenHeight / cellSize;
}

int Game::getCellColumnCount()
{
	return screenWidth / cellSize;
}
