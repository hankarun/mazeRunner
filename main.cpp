#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <random>
#include <unordered_map>

#include "rlib/include/raylib.h"
#include "rlib/include/raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

#pragma comment(lib, "winmm.lib")

int screenWidth = 800;
int screenHeight = 800;

int cellSize = 10;

float getCellRowCount()
{
	return screenHeight / cellSize;
}

float getCellColumnCount()
{
	return screenWidth / cellSize;
}

struct Position
{
	float x;
	float y;
};

bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct Cell
{
	Position position = {};
	bool visited = false;
	// . 1 .
	// 2   3
	// . 4 .
	std::array<bool, 4> walls = { true, true, true, true };
	// . 0 .
	// 1   2
	// . 3 .
	void removeWalls(Cell* n)
	{
		if (position.x == n->position.x - 1)
		{
			walls[2] = false;
			n->walls[1] = false;
		}

		if (n->position.x == position.x - 1)
		{
			walls[1] = false;
			n->walls[2] = false;
		}

		if (position.y == n->position.y - 1)
		{
			walls[3] = false;
			n->walls[0] = false;
		}

		if (n->position.y == position.y - 1)
		{
			walls[0] = false;
			n->walls[3] = false;
		}
	}
};

struct Board
{
	Board(int width, int height)
		: width(width), height(height)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				Cell cell;
				cell.position = { (float)i, (float)j };
				cells.push_back(cell);
			}
		}
	}

	int toIndex(const Position& pos)
	{
		return pos.x + pos.y * width;
	}

	std::vector<Cell*> getUnvisitedNeighBours(Cell* p)
	{
		std::vector<Cell*> neighbours;

		// north (x, y -1)
		if (p->position.y > 0)
		{
			int index = p->position.x + (p->position.y - 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// south (x, y + 1)
		if (p->position.y < (height - 1))
		{
			int index = p->position.x + (p->position.y + 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// east (x - 1, y)
		if (p->position.x > 0)
		{
			int index = p->position.x - 1 + p->position.y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// west (x + 1, y)
		if (p->position.x < (width - 1))
		{
			int index = p->position.x + 1 + p->position.y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		return neighbours;
	}

	std::vector<Cell> cells;
	float width = 50;
	float height = 50;

	Position target = {};
	Position home = {};
};

struct BoardColor
{
	std::vector<int> groupIds;
	std::unordered_map<int, Color> groupColors;

	void init(Board* board)
	{
		groupIds.resize(board->cells.size());
		std::fill(groupIds.begin(), groupIds.end(), 0);
	}

	Color getGroupColor(int index)
	{
		auto result = groupColors.find(groupIds.at(index));
		if (result == groupColors.end())
		{
			unsigned char r = rand() % 255;
			unsigned char g = rand() % 255;
			unsigned char b = rand() % 255;
			groupColors[groupIds.at(index)] = Color{ r, g, b, 255 };
			return groupColors[groupIds.at(index)];
		}
		return result->second;
	}
};

struct BoardGenerator
{
	BoardGenerator(BoardColor* colorer)
		: colorer(colorer) {}
	std::stack<Cell*> traverse;
	Cell* currrent = nullptr;
	int currentGroupId = 1;
	BoardColor* colorer;

	bool isFinished() const
	{
		return traverse.empty();
	}

	void finish(Board* board)
	{
		while (!traverse.empty())
			update(board);
	}

	void init(Board* board)
	{
		traverse = std::stack<Cell*>();
		float startCellX = rand() % (int)board->width;
		float startCellY = rand() % (int)board->height;
		board->home = { startCellX, startCellY };
		float startIndex = board->toIndex(board->home);
		board->cells[startIndex].visited = true;
		traverse.push(&board->cells[startIndex]);

		board->target = { float(rand() % (int)board->width), float(rand() % (int)board->height) };
		colorer->init(board);
	}

	void update(Board* board)
	{
		if (traverse.empty())
			return;
		currrent = traverse.top();
		traverse.pop();

		auto neighBours = board->getUnvisitedNeighBours(currrent);
		if (currrent->position == board->target)
		{
			neighBours.clear();
		}

		if (!neighBours.empty())
		{
			colorer->groupIds.at(board->toIndex(currrent->position)) = currentGroupId;
			traverse.push(currrent);
			int count = neighBours.size();
			int lucky = rand() % count;
			auto nextCell = neighBours[lucky];
			nextCell->visited = true;
			colorer->groupIds.at(board->toIndex(nextCell->position)) = currentGroupId;
			currrent->removeWalls(nextCell);
			traverse.push(nextCell);
		}
		else
		{
			currentGroupId++;
		}
	}
};

struct BoardSolver
{
	BoardSolver(Board* board)
		: board(board) {

	}

	void iterate(int count)
	{

	}

	bool isSolved() const
	{
		return solved;
	}

	Board* board;
	bool solved = false;
	std::vector<Cell*> solution;
};

void draw(BoardSolver* solver)
{
	for (auto cell : solver->solution)
	{
		float x = cell->position.x * cellSize;
		float y = cell->position.y * cellSize;
		DrawRectangle(x, y, cellSize, cellSize, BLUE);
	}
}

void draw(Board* board, bool showColor, BoardColor* colorer)
{
	for (const auto& cell : board->cells)
	{
		float x = cell.position.x * cellSize;
		float y = cell.position.y * cellSize;

		if (cell.visited)
		{
			Color c(WHITE);
			if (showColor)
				c = colorer->getGroupColor(board->toIndex(cell.position));
			DrawRectangle(x, y, cellSize, cellSize, c);
		}
		if (cell.walls[0])
			DrawLine(x, y, x + cellSize, y, BLACK);
		if (cell.walls[1])
			DrawLine(x, y, x, y + cellSize, BLACK);
		if (cell.walls[2])
			DrawLine(x + cellSize, y, x + cellSize, y + cellSize, BLACK);
		if (cell.walls[3])
			DrawLine(x, y + cellSize, x + cellSize, y + cellSize, BLACK);
	}

	Color targetColor{
		255,
		0,
		0,
		255
	};
	DrawRectangle(board->target.x * cellSize, board->target.y * cellSize, cellSize, cellSize, targetColor);

	targetColor = Color{
	0,
	255,
	0,
	255
	};
	int homeIndex = board->home.x + board->home.y * board->width;
	const auto& homeCell = board->cells.at(homeIndex);
	DrawRectangle(homeCell.position.x * cellSize, homeCell.position.y * cellSize, cellSize, cellSize, targetColor);
}

void draw(BoardGenerator* generator)
{
	if (!generator->traverse.empty())
	{
		auto top = generator->traverse.top();
		float x = top->position.x * cellSize;
		float y = top->position.y * cellSize;
		DrawRectangle(x, y, cellSize, cellSize, GREEN);
	}
}

void drawInfo(int speed, bool loop)
{
	DrawRectangle(10, 10, 290, 123, Fade(SKYBLUE, 0.9f));
	DrawRectangleLines(10, 10, 290, 123, BLUE);

	DrawText("Keyboard Control:", 20, 20, 10, WHITE);
	DrawText("F - Finish / R - Reset", 40, 40, 10, WHITE);
	DrawText("+ - Speed up / - - Slow down", 40, 60, 10, WHITE);
	DrawText("Size W - Increase  / Q - Decrease", 40, 80, 10, WHITE);
	DrawText("H - Show/Hide L - Loop On/Off - C Color On/Off", 40, 100, 10, WHITE);
	DrawText(TextFormat("Size: %d Speed: %d, Loop: %d", cellSize, speed, loop), 40, 120, 10, WHITE);
	static bool drawRing = false;
	drawRing = GuiCheckBox(Rectangle{ 600, 320, 20, 20 }, "Draw Ring", &drawRing);
}

class Game
{
public:
	Game()
		: board(getCellColumnCount(), getCellRowCount())
		, boardGenerator(&colorer)
	{
	}

	void init()
	{
		boardGenerator.init(&board);
	}

	void resetBoard()
	{
		board = Board(getCellColumnCount(), getCellRowCount());
		boardGenerator.init(&board);
	}

	void updateCanvasSize()
	{
		int currentscreenWidth = GetScreenWidth();
		int currentscreenHeight = GetScreenHeight();

		if (currentscreenWidth != screenWidth && currentscreenHeight != screenHeight)
		{
			screenWidth = currentscreenWidth;
			screenHeight = currentscreenHeight;
			resetBoard();
		}
	}

	void update(double time)
	{
		updateCanvasSize();

		if (loop && boardGenerator.isFinished())
		{
			resetBoard();
		}

		int key = GetKeyPressed();

		while (key > 0)
		{
			if (key == KEY_H)
				showInfo = !showInfo;

			if (key == KEY_F)
			{
				if (boardGenerator.isFinished())
				{
					resetBoard();
				}

				boardGenerator.finish(&board);
			}

			if (key == KEY_L)
				loop = !loop;

			if (key == KEY_C)
				showColor = !showColor;

			if (key == KEY_KP_ADD)
				speed += 1;
			if (key == KEY_KP_SUBTRACT)
				speed -= 1;
			if (key == KEY_R)
			{
				resetBoard();
				speed = 1;
			}

			if (key == KEY_Q)
			{
				cellSize += 1;
				resetBoard();
			}

			if (key == KEY_W)
			{
				cellSize -= 5;
				cellSize = std::max(3, cellSize);
				resetBoard();
			}

			key = GetKeyPressed();
		}

		speed = std::max(1, speed);

		for (int i = 0; i < speed; ++i)
			boardGenerator.update(&board);

		BeginDrawing();
		ClearBackground(DARKGRAY);
		draw(&board, showColor, &colorer);
		draw(&boardGenerator);

		if (showInfo)
			drawInfo(speed, loop);


		EndDrawing();
	}

private:
	Board board;
	BoardColor colorer;
	BoardGenerator boardGenerator;

	int speed = 1;
	bool loop = false;
	bool showInfo = true;
	bool showColor = false;
};

void UpdateDrawFrame(void* userData)
{
	auto game = (Game*)userData;
	game->update(0);
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