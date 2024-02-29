#pragma once

namespace Gui
{
	class DebuggerScript
	{
	public:
		DebuggerScript();

		void Refresh();
		const DateTime& LastReloadedTime() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
