#pragma once
#include <vector>
#include <memory>
#include "MsPacmanObserver.h"

class MsPacmanSubject
{
public:
	void AddObserver(MsPacmanObserver* observer);
	void RemoveObserver(MsPacmanObserver* observer);
	void Notify(MsPacmanEvent event);

private:
	std::vector<MsPacmanObserver*> m_Observers;
};
