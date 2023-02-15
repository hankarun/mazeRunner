#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <random>
#include <unordered_map>

#include "raylib.h"
#include "raymath.h"

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

struct Cell
{
	float x = 0;
	float y = 0;
	bool visited = false;
	// . 1 .
	// 2   3
	// . 4 .
	std::array<bool, 4> walls = { true, true, true, true };
	int groupId = 0;
	// . 0 .
	// 1   2
	// . 3 .
	void removeWalls(Cell* n)
	{
		if (x == n->x - 1)
		{
			walls[2] = false;
			n->walls[1] = false;
		}

		if (n->x == x - 1)
		{
			walls[1] = false;
			n->walls[2] = false;
		}

		if (y == n->y - 1)
		{
			walls[3] = false;
			n->walls[0] = false;
		}

		if (n->y == y - 1)
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
				cell.x = i;
				cell.y = j;
				cells.push_back(cell);
			}
		}
	}

	std::vector<Cell*> getUnvisitedNeighBours(Cell* p)
	{
		std::vector<Cell*> neighbours;

		// north (x, y -1)
		if (p->y > 0)
		{
			int index = p->x + (p->y - 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// south (x, y + 1)
		if (p->y < (height - 1))
		{
			int index = p->x + (p->y + 1) * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// east (x - 1, y)
		if (p->x > 0)
		{
			int index = p->x - 1 + p->y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		// west (x + 1, y)
		if (p->x < (width - 1))
		{
			int index = p->x + 1 + p->y * width;
			if (!cells.at(index).visited)
				neighbours.push_back(&cells[index]);
		}

		return neighbours;
	}

	std::vector<Cell> cells;
	float width = 50;
	float height = 50;
};


struct BoardGenerator
{
	std::stack<Cell*> traverse;
	Cell* currrent = nullptr;
	int currentGroupId = 1;

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
		board->cells[0].visited = true;
		traverse.push(&board->cells[0]);
	}

	void update(Board* board)
	{
		if (traverse.empty())
			return;
		currrent = traverse.top();
		traverse.pop();
		auto neighBours = board->getUnvisitedNeighBours(currrent);
		if (!neighBours.empty())
		{
			currrent->groupId = currentGroupId;
			traverse.push(currrent);
			int count = neighBours.size();
			int lucky = rand() % count;
			auto nextCell = neighBours[lucky];
			nextCell->visited = true;
			nextCell->groupId = currentGroupId;
			currrent->removeWalls(nextCell);
			traverse.push(nextCell);
		}
		else
		{
			currentGroupId++;
		}
	}
};

std::unordered_map<int, Color> groupColors;

Color getGroupColor(int groupId)
{
	auto result = groupColors.find(groupId);
	if (result == groupColors.end())
	{
		unsigned char r = rand() % 255;
		unsigned char g = rand() % 255;
		unsigned char b = rand() % 255;
		groupColors[groupId] = Color{ r, g, b, 255 };
		return groupColors[groupId];
	}
	return result->second;
}

void draw(Board* board, bool showColor)
{
	for (const auto& cell : board->cells)
	{
		float x = cell.x * cellSize;
		float y = cell.y * cellSize;

		if (cell.visited)
		{
			Color c(WHITE);
			if (showColor)
				c = getGroupColor(cell.groupId);
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

}

void draw(BoardGenerator* generator)
{
	if (!generator->traverse.empty())
	{
		auto top = generator->traverse.top();
		float x = top->x * cellSize;
		float y = top->y * cellSize;
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
}

int main(int argc, char* argv[])
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "MazeRunner");
	SetTargetFPS(60);

	Board board(getCellColumnCount(), getCellRowCount());
	BoardGenerator boardGenerator;

	boardGenerator.init(&board);

	int speed = 1;
	bool loop = false;
	bool showInfo = true;
	bool showColor = false;

	while (!WindowShouldClose())
	{
		int currentscreenWidth = GetScreenWidth();
		int currentscreenHeight = GetScreenHeight();
		
		if (currentscreenWidth != screenWidth && currentscreenHeight != screenHeight)
		{
			screenWidth = currentscreenWidth;
			screenHeight = currentscreenHeight;
			board = Board(getCellColumnCount(), getCellRowCount());
			boardGenerator.init(&board);
		}

		if (loop && boardGenerator.isFinished())
		{
			board = Board(getCellColumnCount(), getCellRowCount());
			boardGenerator.init(&board);
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
					board = Board(getCellColumnCount(), getCellRowCount());
					boardGenerator.init(&board);
				}

				boardGenerator.finish(&board);
			}

			if (key == KEY_L)
				loop = !loop;

			if (key == KEY_C)
				showColor = !showColor;

			if (key ==  KEY_KP_ADD)
				speed += 1;
			if (key ==  KEY_KP_SUBTRACT)
				speed -= 1;
			if (key == KEY_R)
			{
				board = Board(getCellColumnCount(), getCellRowCount());
				boardGenerator.init(&board);
				speed = 1;
			}

			if (key == KEY_Q)
			{
				cellSize += 1;
				board = Board(getCellColumnCount(), getCellRowCount());
				boardGenerator.init(&board);
			}

			if (key == KEY_W)
			{
				cellSize -= 5;
				cellSize = std::max(3, cellSize);
				board = Board(getCellColumnCount(), getCellRowCount());
				boardGenerator.init(&board);
			}

			key = GetKeyPressed();
		}

		speed = std::max(1, speed);

		for(int i = 0; i < speed; ++i)
			boardGenerator.update(&board);

		BeginDrawing();
		ClearBackground(DARKGRAY);
		draw(&board, showColor);
		draw(&boardGenerator);

		if (showInfo)
			drawInfo(speed, loop);


		EndDrawing();
	}

	CloseWindow();

	return 0;
}