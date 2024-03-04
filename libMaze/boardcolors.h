#pragma once
#include "common.h"

#include <unordered_map>
#include <vector>

namespace maze {
	class Board;

	class BoardColor
	{
	public:
		void init(Board* board);

		Color getGroupColor(int index);

		void setCellGroupId(int index, int groupId);
	private:
		std::vector<int> groupIds;
		std::unordered_map<int, Color> groupColors;
	};
}