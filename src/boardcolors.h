#pragma once
#include "common.h"

#include "board.h"

#include <unordered_map>

class BoardColor
{
public:
	void init(Board* board);

	Color getGroupColor(int index);

	std::vector<int> groupIds;
	std::unordered_map<int, Color> groupColors;
};