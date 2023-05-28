#pragma once
#include "board.h"
#include "boardcolors.h"
#include "boardgenerator.h"

int getCellRowCount();

int getCellColumnCount();

class Game
{
public:
	Game();

	void init();

	void resetBoard();

	void updateCanvasSize();

	void update(double time);

public:
	Board board;
	BoardColor colorer;
	BoardGenerator boardGenerator;

	int speed = 1;
	bool loop = false;
	bool showColor = false;
};