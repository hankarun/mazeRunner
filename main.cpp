#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <random>

#include "raylib.h"
#include "raymath.h"

#pragma comment(lib, "winmm.lib")

int screenWidth = 800;
int screenHeight = 800;

float height = 50;
float width = 50;


struct Cell
{
	float x = 0;
	float y = 0;
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
			traverse.push(currrent);
			int count = neighBours.size();
			int lucky = rand() % count;
			auto nextCell = neighBours[lucky];
			nextCell->visited = true;
			currrent->removeWalls(nextCell);
			traverse.push(nextCell);
		}
	}
};

void draw(Board* board)
{
	float cellWidth = screenWidth / width;
	float cellHeight = screenHeight / height;
	for (const auto& cell : board->cells)
	{
		float x = cell.x * cellWidth;
		float y = cell.y * cellHeight;

		if (cell.visited)
			DrawRectangle(x, y, cellWidth, cellHeight, WHITE);
		if (cell.walls[0])
			DrawLine(x, y, x + cellWidth, y, BLACK);
		if (cell.walls[1])
			DrawLine(x, y, x, y + cellWidth, BLACK);
		if (cell.walls[2])
			DrawLine(x + cellWidth, y, x + cellWidth, y + cellWidth, BLACK);
		if (cell.walls[3])
			DrawLine(x, y + cellWidth, x + cellWidth, y + cellWidth, BLACK);
	}

}

void draw(BoardGenerator* generator)
{
	float cellWidth = screenWidth / width;
	float cellHeight = screenHeight / height;
	if (!generator->traverse.empty())
	{
		auto top = generator->traverse.top();
		float x = top->x * cellWidth;
		float y = top->y * cellHeight;
		DrawRectangle(x, y, cellWidth, cellHeight, GREEN);
	}
}

void drawInfo(int speed)
{
	DrawRectangle(10, 10, 250, 163, Fade(SKYBLUE, 0.8f));
	DrawRectangleLines(10, 10, 250, 163, BLUE);

	DrawText("Keyboard Control:", 20, 20, 10, WHITE);
	DrawText("F - Finish", 40, 40, 10, WHITE);
	DrawText("R - Reset", 40, 60, 10, WHITE);
	DrawText("+ - Speed up", 40, 80, 10, WHITE);
	DrawText("- - Slow down", 40, 100, 10, WHITE);
	DrawText("W - Increase Size", 40, 120, 10, WHITE);
	DrawText("Q - Decrease Size", 40, 140, 10, WHITE);
	DrawText(TextFormat("Size: %.0f %.0f Speed: %d", width, height, speed), 40, 160, 10, WHITE);
}

int main(int argc, char* argv[])
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, "MazeRunner");
	SetTargetFPS(60);

	Board board(width, height);
	BoardGenerator boardGenerator;

	boardGenerator.init(&board);

	int speed = 1;

	while (!WindowShouldClose())
	{
		int key = GetKeyPressed();

		while (key > 0)
		{
			if (key == KEY_F)
			{
				if (boardGenerator.isFinished())
				{
					board = Board(width, height);
					boardGenerator.init(&board);
				}

				boardGenerator.finish(&board);
			}
			if (key ==  KEY_KP_ADD)
				speed += 1;
			if (key ==  KEY_KP_SUBTRACT)
				speed -= 1;
			if (key == KEY_R)
			{
				board = Board(width, height);
				boardGenerator.init(&board);
				speed = 1;
			}

			if (key == KEY_Q)
			{
				width += 10;
				height += 10;
				board = Board(width, height);
				boardGenerator.init(&board);
			}

			if (key == KEY_W)
			{
				width -= 10;
				height -= 10;
				width = std::max(10.0f, width);
				height = std::max(10.0f, height);
				board = Board(width, height);
				boardGenerator.init(&board);
			}

			key = GetKeyPressed();
		}

		speed = std::max(1, speed);

		for(int i = 0; i < speed; ++i)
			boardGenerator.update(&board);

		BeginDrawing();
		ClearBackground(DARKGRAY);
		draw(&board);
		draw(&boardGenerator);

		drawInfo(speed);


		EndDrawing();
	}

	CloseWindow();

	return 0;
}