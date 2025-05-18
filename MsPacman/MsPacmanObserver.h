#pragma once
enum class MsPacmanEvent
{
	EATEN_BIG_PELLET,
	EATEN_SMALL_PELLET,
	EATEN_GHOST,
	LEVEL_COMPLETE,
	START_LEVEL,
	GAME_OVER,
};

class MsPacmanObserver
{
public:
	virtual void OnNotify(MsPacmanEvent event) = 0;
};