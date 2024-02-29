#pragma once

namespace Util
{
	class ScriptFunctionWrapper
	{
	public:
		ScriptFunctionWrapper() = default;
		explicit ScriptFunctionWrapper(AngelScript::asIScriptFunction* ptr): m_ptr(ptr) { if (ptr) ptr->AddRef(); }
		~ScriptFunctionWrapper() { if (m_ptr) m_ptr->Release(); }
		// asIScriptFunction* operator->() const { return m_ptr; }
		AngelScript::asIScriptFunction* operator*() const { return m_ptr; }
		operator bool() const { return m_ptr; }

		void InvokeVoid() const
		{
			const auto ctx = Script::GetEngine()->CreateContext();
			ctx->Prepare(m_ptr);
			ctx->Execute();
		}

	private:
		AngelScript::asIScriptFunction* m_ptr{};
	};
}
