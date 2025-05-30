#pragma once
enum class MsPacmanEvent
{
	EATEN_BIG_PELLET,
	EATEN_SMALL_PELLET,
	EATEN_GHOST,
	LEVEL_COMPLETE,
	START_LEVEL,
	GAME_OVER,
	DIE,
	SPAWN_FRUIT,
	EATEN_CHERRY,
	EATEN_STRAWBERRY,
	EATEN_ORANGE
};

class MsPacmanObserver
{
public:
	virtual void OnNotify(MsPacmanEvent event) = 0;
};