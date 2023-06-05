#include "common.h"

bool operator==(const maze::Position& lhs, const maze::Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}