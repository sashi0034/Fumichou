#pragma once

namespace Util
{
	void RegisterTomlStyleSheetAddon();

	TOMLValue GetTomlStyleValue(const String& valuePath);

	template <typename T>
	inline T GetTomlStyle(const String& valuePath)
	{
#if _DEBUG
		return GetTomlStyleValue(valuePath).get<T>();;
#else
		static HashTable<String, T> s_hash{};
		auto&& found = s_hash.find(valuePath);
		if (found != s_hash.end()) return found->second;
		auto&& loaded = GetTomlStyleValue(valuePath).get<T>();
		s_hash[valuePath] = loaded;
		return loaded;
#endif
	}
}
