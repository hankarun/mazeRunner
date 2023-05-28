#pragma once
#include "board.h"
#include "boardcolors.h"
#include "boardgenerator.h"

class Game
{
public:
	Game();

	void init();

	void resetBoard();

	void updateCanvasSize();

	void update(double time);

	int getCellRowCount();

	int getCellColumnCount();
public:
	int speed = 1;
	bool loop = false;
	bool showColor = false;
	int screenWidth = 800;
	int screenHeight = 800;
	int cellSize = 10;

	Board board;
	BoardColor colorer;
	BoardGenerator boardGenerator;
};