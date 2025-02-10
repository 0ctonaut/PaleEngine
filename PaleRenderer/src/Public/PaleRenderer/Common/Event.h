#pragma once
#include <vector>
#include <functional>

template <typename... TArgs>
class Event
{
public:
	void subscribe(const std::function<void(TArgs...)>& vListener)
	{
		m_Listeners.push_back(vListener);
	}

	void invoke(TArgs... vArgs)
	{
		for (int i = 0; i < m_Listeners.size(); ++i)
		{
			m_Listeners[i](vArgs...);
		}
	}
private:
	std::vector<std::function<void(TArgs...)>> m_Listeners;
};