#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <random>
#include <unordered_map>

#include "cell.h"
#include "board.h"
#include "boardsolver.h"
#include "boardcolors.h"
#include "boardgenerator.h"
#include "game.h"
#include "gamedrawer.h"

#pragma comment(lib, "winmm.lib")

bool showInfo = true;

void updateKeys(Game* game)
{
	int key = GetKeyPressed();

	while (key > 0)
	{
		if (key == KEY_H)
			showInfo = !showInfo;

		if (key == KEY_F)
		{
			if (game->boardGenerator.isFinished())
			{
				game->resetBoard();
			}

			game->boardGenerator.finish(&game->board);
		}

		if (key == KEY_L)
			game->loop = !game->loop;

		if (key == KEY_KP_ADD)
			game->speed += 1;
		if (key == KEY_KP_SUBTRACT)
			game->speed -= 1;
		if (key == KEY_R)
		{
			game->resetBoard();
			game->speed = 1;
		}
		game->speed = std::max(1, game->speed);
		if (key == KEY_Q)
		{
			cellSize += 1;
			game->resetBoard();
		}

		if (key == KEY_W)
		{
			cellSize -= 5;
			cellSize = std::max(3, cellSize);
			game->resetBoard();
		}

		key = GetKeyPressed();
	}
}

void UpdateDrawFrame(void* userData)
{
	auto game = (Game*)userData;

	updateKeys(game);
	game->update(0);

	BeginDrawing();
	ClearBackground(DARKGRAY);
	GameDrawer drawer;
	drawer.draw(&game->board, game->showColor, &game->colorer);
	drawer.draw(&game->boardGenerator);

	if (showInfo)
		drawer.drawInfo(game);
	EndDrawing();
}

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

int main(int argc, char* argv[])
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "MazeRunner");

	Game game;
	game.init();

#ifdef PLATFORM_WEB
	emscripten_set_main_loop_arg(UpdateDrawFrame, &game, 0, 1);
#else
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		UpdateDrawFrame(&game);
	}
#endif

	CloseWindow();

	return 0;
}