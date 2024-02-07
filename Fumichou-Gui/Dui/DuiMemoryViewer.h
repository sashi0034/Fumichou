#pragma once

namespace Dui
{
	class DuiMemoryViewer
	{
	public:
		DuiMemoryViewer();
		void Update(std::string_view viewName, std::span<uint8 const> memory);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
