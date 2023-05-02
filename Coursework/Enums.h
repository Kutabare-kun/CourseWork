#pragma once

enum class SceneUpdate
{
	MAIN,
	SELECTLEVEL,
	GAME,
	BUILDLEVEL,
	NONE
};


enum class PaintFlag
{
	P_NONE = 0,
	P_DELETE = 1,
	P_WALL = 2,
	P_START = 4,
	P_FINISH = 8,
	P_PAINT = 16,
	P_WATCH = 32,
};
