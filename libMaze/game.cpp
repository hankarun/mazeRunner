#include "game.h"

namespace maze {
	Game::Game(int width, int height)
		: cellSize(10)
		, screenHeight(height)
		, screenWidth(width)
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

	void Game::updateCanvasSize(int width, int height)
	{
		if (width != screenWidth && height != screenHeight)
		{
			screenWidth = width;
			screenHeight = height;
			reset();
		}
	}

	void Game::update(double time)
	{
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
}
