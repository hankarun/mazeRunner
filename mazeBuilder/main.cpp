#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <random>
#include <unordered_map>

#include <game.h>

#include <gamedrawer.h>

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls


void drawInfo(maze::Game* game)
{
	DrawRectangle(20, offsetY, 250, 500, Fade(SKYBLUE, 0.9f));
	DrawRectangleLines(20, offsetY, 250, 500, BLUE);

	GuiCheckBox(Rectangle{ 24, offsetY + 6, 24, 24 }, "Show Color", &game->showColor);
	GuiCheckBox(Rectangle{ 24, offsetY + 38, 24, 24 }, "Loop", &game->loop);
	static bool ValueBOx004EditMode = false;
	static bool ValueBOx005EditMode = false;
	if (GuiSpinner(Rectangle{ 104, offsetY + 60, offsetY + 60, 24 }, "Speed", &game->speed, 0, 100, ValueBOx004EditMode)) ValueBOx004EditMode = !ValueBOx004EditMode;
	int cellSize = game->cellSize;
	GuiSpinner(Rectangle{ 104, offsetY + 90, offsetY + 90, 24 }, "Cell Size", &game->cellSize, 0, 100, ValueBOx005EditMode);
	if (cellSize != game->cellSize)
		game->reset();

	if (GuiButton(Rectangle{ 112, offsetY + 120, 112, 24 }, "Finish Generating"))
	{
		if (game->boardGenerator.isFinished())
		{
			game->reset();
		}

		game->boardGenerator.finish(&game->board);
	}
	if (GuiButton(Rectangle{ 24, offsetY + 120, 80, 24 }, "Reset"))
	{
		game->reset();
		game->speed = 1;
	}
}

void UpdateDrawFrame(void* userData)
{
	auto game = (maze::Game*)userData;
	game->updateCanvasSize(500, 500);
	game->update(0);

	BeginDrawing();
	ClearBackground(DARKGRAY);
	GameDrawer drawer(game);
	drawer.draw(&game->board, game->showColor);
	drawer.draw(&game->boardGenerator);

	static bool showInfo = true;
	if (IsKeyPressed(KEY_LEFT_CONTROL))
	{
		showInfo = !showInfo;
	}

	if (showInfo)
		drawInfo(game);

	EndDrawing();
	}

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#else
#pragma comment(lib, "winmm.lib")
#endif

int main(int argc, char* argv[])
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 520, "MazeRunner");

	maze::Game game(800, 800);
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