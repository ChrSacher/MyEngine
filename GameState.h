#pragma once

struct GameState
{
	bool playing;
	bool paused;
	bool mainmenu;
	bool escapemenu;
	bool drawgrid;
	bool cameramove;
	bool ray;
	bool render;
	bool update;
	static GameState state;
};
