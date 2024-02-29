#pragma once

namespace Gui
{
	class DebuggerScript
	{
	public:
		DebuggerScript();

		void Refresh();

		[[nodiscard]] const DateTime& LastReloadedTime() const;
		[[nodiscard]] FilePathView Filepath() const;
		[[nodiscard]] bool IsSucceeded() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
