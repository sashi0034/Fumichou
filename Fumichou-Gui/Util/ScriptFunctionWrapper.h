#pragma once

namespace Util
{
	class ScriptFunctionWrapper
	{
	public:
		ScriptFunctionWrapper() = default;
		explicit ScriptFunctionWrapper(AngelScript::asIScriptFunction* ptr): m_ptr(ptr) { if (ptr) ptr->AddRef(); }
		~ScriptFunctionWrapper() { if (m_ptr) m_ptr->Release(); }

		ScriptFunctionWrapper(const ScriptFunctionWrapper& other) : ScriptFunctionWrapper(other.m_ptr)
		{
		}

		ScriptFunctionWrapper& operator=(const ScriptFunctionWrapper& other)
		{
			if (this != &other)
			{
				if (m_ptr) m_ptr->Release();
				m_ptr = other.m_ptr;
				if (m_ptr) m_ptr->AddRef();
			}
			return *this;
		}

		// asIScriptFunction* operator->() const { return m_ptr; }
		AngelScript::asIScriptFunction* operator*() const { return m_ptr; }
		operator bool() const { return m_ptr; }

		void InvokeVoid() const
		{
			const auto ctx = Script::GetEngine()->CreateContext();
			ctx->Prepare(m_ptr);
			ctx->Execute();
			(void)ctx->Release();
		}

	private:
		AngelScript::asIScriptFunction* m_ptr{};
	};
}
