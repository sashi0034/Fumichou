#include "stdafx.h"
#include "DebuggerScript.h"

#include "DebugParameter.h"
#include "HwFrame.h"
#include "Util/FileWatcher.h"
#include "Util/ScriptFunctionWrapper.h"

using namespace Gui;
using namespace AngelScript;

namespace
{
	constexpr FilePathView defaultPath = U"scripting.as";

	void __cdecl breakpoint()
	{
		Nes::HwFrame::Instance().RequestBreakpoint();
	}

	void __cdecl watch_interrupt(asIScriptFunction* callback)
	{
		Util::ScriptFunctionWrapper wrapper{callback};
		Nes::DebugParameter::Instance().watchInterrupt = [wrapper]()
		{
			wrapper.InvokeVoid();
		};
	}

	bool s_initializedEngine{};

	void checkInitializeEngine()
	{
		if (s_initializedEngine) return;
		s_initializedEngine = true;

		Script::GetEngine()->RegisterFuncdef("void CALLBACK()");
		Script::GetEngine()->RegisterGlobalFunction(
			"void breakpoint()",
			asFunctionPtr(breakpoint),
			asCALL_CDECL);
		Script::GetEngine()->RegisterGlobalFunction(
			"void watch_interrupt(CALLBACK @)",
			asFunctionPtr(watch_interrupt),
			asCALL_CDECL);
	}
}

struct DebuggerScript::Impl
{
	bool m_initialized{};
	Util::FileWatcher m_fileWatcher{defaultPath};
	Script m_script{defaultPath};
	DateTime m_lastReloadedTime{};

	void Reload()
	{
		checkInitializeEngine();

		m_lastReloadedTime = DateTime::Now();

		if (not m_script.reload())
		{
			for (auto&& message : m_script.getMessages())
			{
				Console.write(message);
			}
		}

		const auto scriptFunc = m_script.getFunction<void()>(U"setup");
		String exception;
		scriptFunc.tryCall(exception);
		if (exception.isEmpty() == false)
		{
			Print(exception);
		}
	}

	void Refresh()
	{
		if (not m_initialized || m_fileWatcher.CheckChanged())
		{
			m_initialized = true;
			Reload();
		}
	}
};

namespace Gui
{
	DebuggerScript::DebuggerScript() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void DebuggerScript::Refresh()
	{
		p_impl->Refresh();
	}

	const DateTime& DebuggerScript::LastReloadedTime() const
	{
		return p_impl->m_lastReloadedTime;
	}
}
