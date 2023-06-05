#include "boardcolors.h"

namespace maze {
	void BoardColor::init(Board* board)
	{
		groupIds.resize(board->cells.size());
		std::fill(groupIds.begin(), groupIds.end(), 0);
	}

	Color BoardColor::getGroupColor(int index)
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
}
