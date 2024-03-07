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
	constexpr FilePathView examplePath = U"scripting.example.as";
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

	void __cdecl watch_pc(uint16 pc, asIScriptFunction* callback)
	{
		Util::ScriptFunctionWrapper wrapper{callback};
		Nes::DebugParameter::Instance().watchPc[pc] = [wrapper]()
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
		Script::GetEngine()->RegisterGlobalFunction(
			"void watch_pc(uint16, CALLBACK @)",
			asFunctionPtr(watch_pc),
			asCALL_CDECL);
	}
}

struct DebuggerScript::Impl
{
	bool m_initialized{};
	Util::FileWatcher m_fileWatcher{defaultPath};
	Script m_script{defaultPath};
	DateTime m_lastReloadedTime{};
	bool m_succeeded{};

	void Reload()
	{
		checkInitializeEngine();

		m_lastReloadedTime = DateTime::Now();
		m_succeeded = true;

		if (not m_script.reload())
		{
			m_succeeded = false;
			for (auto&& message : m_script.getMessages())
			{
				Console.writeln(message);
			}
		}

		// クリア
		Nes::DebugParameter::Instance().watchInterrupt = {};
		Nes::DebugParameter::Instance().watchPc.fill({});

		// スクリプト実行
		const auto scriptFunc = m_script.getFunction<void()>(U"setup");
		String exception;
		scriptFunc.tryCall(exception);
		if (exception.isEmpty() == false)
		{
			Console.writeln(exception);
			m_succeeded = false;
		}
	}

	void Refresh()
	{
		if (not m_initialized)
		{
			initialize();
		}

		if (m_fileWatcher.CheckChanged())
		{
			Reload();
		}
	}

private:
	void initialize()
	{
		m_initialized = true;
		Reload();

		// スクリプトファイルが存在しないなら凡例を利用
		const auto targetFile = m_fileWatcher.TargetFile();
		if (not FileSystem::Exists(targetFile))
		{
			FileSystem::Copy(examplePath, targetFile);
		}

		m_script = Script(targetFile);
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

	FilePathView DebuggerScript::Filepath() const
	{
		return p_impl->m_fileWatcher.TargetFile();
	}

	bool DebuggerScript::IsSucceeded() const
	{
		return p_impl->m_succeeded;
	}
}
