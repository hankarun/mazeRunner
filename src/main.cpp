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

void UpdateDrawFrame(void* userData)
{
	auto game = (Game*)userData;

	game->update(0);

	BeginDrawing();
	ClearBackground(DARKGRAY);
	GameDrawer drawer(game);
	drawer.draw(&game->board, game->showColor, &game->colorer);
	drawer.draw(&game->boardGenerator);
	drawer.drawInfo(game);

	EndDrawing();
}

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

int main(int argc, char* argv[])
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 800, "MazeRunner");

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