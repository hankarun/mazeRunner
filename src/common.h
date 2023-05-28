#pragma once
#include "raylib.h"
#include "raymath.h"


#include <array>

struct Position
{
	float x;
	float y;
};

static int screenWidth = 800;
static int screenHeight = 800;
static int cellSize = 10;

bool operator==(const Position& lhs, const Position& rhs);