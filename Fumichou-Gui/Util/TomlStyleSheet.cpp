#include "stdafx.h"
#include "TomlStyleSheet.h"

namespace
{
	constexpr FilePathView tomlDirectory = U"asset";
	constexpr FilePathView tomlFilename = U"stylesheet.toml";
	const FilePath tomlFilepath = tomlDirectory + U"/" + tomlFilename;

	struct TomlState
	{
		DirectoryWatcher directoryWatcher{tomlDirectory};
		TOMLReader toml{
#if _DEBUG
			tomlFilepath
#else
			Resource(tomlFilepath)
#endif
		};
	};

	TomlState* s_instance;

	class TomlStyleSheetAddon : public IAddon
	{
	public:
		~TomlStyleSheetAddon() override = default;

		bool init() override
		{
			s_instance = &m_state;
			return true;
		}

		bool update() override
		{
			for (auto [path, action] : m_state.directoryWatcher.retrieveChanges())
			{
				if (FileSystem::FileName(path) == tomlFilename.data())
					m_state.toml.open(tomlFilepath);
			}
			return true;
		}

	private:
		TomlState m_state{};
	};
}

void Util::RegisterTomlStyleSheetAddon()
{
	Addon::Register<TomlStyleSheetAddon>(U"TomlStyleSheetAddon");
}

TOMLValue Util::GetTomlStyleValue(const String& valuePath)
{
	auto&& value = s_instance->toml[valuePath];;
#if _DEBUG
	if (value.isEmpty())
	{
		Console.writeln(U"TOML parameter error", U"'{}' is missing."_fmt(valuePath));
	}
#endif
	return value;
}
