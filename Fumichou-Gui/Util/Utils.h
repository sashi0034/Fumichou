#pragma once

namespace Util
{
	inline void WaitAnyKeyOnConsole()
	{
		(void)(std::getchar());
	}

	class DeferAction
	{
	public:
		[[nodiscard]]
		DeferAction(const std::function<void()>& action) : m_action(action)
		{
		}

		~DeferAction()
		{
			m_action();
		}

	private:
		std::function<void()> m_action;
	};
}
