#pragma once

#include "boardsolver.h"
#include "board.h"
#include "boardcolors.h"
#include "boardgenerator.h"
#include "game.h"

class GameDrawer
{
public:
	GameDrawer(Game* game);

	void draw(BoardSolver* solver);

	void draw(Board* board, bool showColor, BoardColor* colorer);

	void draw(BoardGenerator* generator);

	void drawInfo(Game* game);

private:
	Game* game;
};