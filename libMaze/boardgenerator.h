#pragma once
#include <stack>

#include "cell.h"
#include "board.h"
#include "boardcolors.h"

namespace maze {
	class BoardGenerator
	{
	public:
		BoardGenerator();

		bool isFinished() const;

		void finish(Board* board);

		void init(Board* board);

		void update(Board* board);
	public:
		std::stack<Cell*> traverse;
		Cell* currrent = nullptr;
		int currentGroupId = 1;
	};
}