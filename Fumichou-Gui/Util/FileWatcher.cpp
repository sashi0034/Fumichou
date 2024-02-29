#include "stdafx.h"
#include "FileWatcher.h"

namespace Util
{
	FileWatcher::FileWatcher(FilePathView file) :
		m_targetFile(file),
		m_directoryWatcher(FileSystem::ParentPath(file, 0))
	{
	}

	bool FileWatcher::CheckChanged()
	{
		for (auto&& change : m_directoryWatcher.retrieveChanges())
		{
			if (FileSystem::FileName(change.path) == FileSystem::FileName(m_targetFile))
			{
				return true;
			}
		}
		return false;
	}
}
