#pragma once
#include "Mmu.h"

namespace Dui
{
	using MappedArrayView = std::variant<const Nes::Mmu::read_array*, const Nes::Mmu::write_array*>;

	class DuiMappingViewer
	{
	public:
		DuiMappingViewer();
		void Update(std::string_view viewName, MappedArrayView mappedArray);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
