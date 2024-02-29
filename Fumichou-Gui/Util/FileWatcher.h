#pragma once

namespace Util
{
	class FileWatcher
	{
	public:
		FileWatcher() = default;
		FileWatcher(FilePathView file);

		[[nodiscard]] FilePathView TargetFile() { return m_targetFile; }
		[[nodiscard]] bool CheckChanged();

	private:
		FilePath m_targetFile{};
		DirectoryWatcher m_directoryWatcher{};
	};
}
