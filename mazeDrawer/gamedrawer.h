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

	void draw(maze::Board* board, bool showColor);

	void draw(maze::BoardGenerator* generator);
private:
	maze::Game* game;
};