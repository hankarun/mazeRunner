#pragma once

#include "boardsolver.h"
#include "board.h"
#include "boardcolors.h"
#include "boardgenerator.h"
#include "game.h"

class GameDrawer
{
public:
	GameDrawer(maze::Game* game);

	void draw(maze::BoardSolver* solver);

	void draw(maze::Board* board, bool showColor, maze::BoardColor* colorer);

	void draw(maze::BoardGenerator* generator);

	void drawInfo(maze::Game* game);

private:
	maze::Game* game;
};