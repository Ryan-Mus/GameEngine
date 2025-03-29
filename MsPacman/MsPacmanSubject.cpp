#include "MsPacmanSubject.h"

void MsPacmanSubject::AddObserver(MsPacmanObserver* observer)
{
	m_Observers.push_back(observer);
}

void MsPacmanSubject::RemoveObserver(MsPacmanObserver* observer)
{
	for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it)
	{
		if (*it == observer)
		{
			m_Observers.erase(it);
			break;
		}
	}
}

void MsPacmanSubject::Notify(MsPacmanEvent event)
{
	for (auto observer : m_Observers)
	{
		observer->OnNotify(event);
	}
}
