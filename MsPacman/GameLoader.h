#pragma once
#include "Singleton.h"
#include "String"
class GameLoader : public dae::Singleton<GameLoader>
{
public: 
	void loadGameJSON(const std::string& path);
};

