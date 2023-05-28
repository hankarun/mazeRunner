#pragma once
#include "raylib.h"
#include "raymath.h"
#include <array>

struct Position
{
	float x;
	float y;
};

bool operator==(const Position& lhs, const Position& rhs);