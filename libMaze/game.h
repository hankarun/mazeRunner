#pragma once
#include "board.h"
#include "boardcolors.h"
#include "boardgenerator.h"

namespace maze {
	class Game
	{
	public:
		Game(int width, int height);

		void init();

		void reset();

		void updateCanvasSize(int width, int height);

		void update(double time);

		int getCellRowCount();

		int getCellColumnCount();
	public:
		int speed = 1;
		bool loop = false;
		bool showColor = false;
		int screenWidth = 800;
		int screenHeight = 800;
		int cellSize = 10;

		Board board;
		BoardGenerator boardGenerator;
	};
}