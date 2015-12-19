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
	static GameState state;
};
