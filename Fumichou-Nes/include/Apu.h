#pragma once

namespace Nes
{
	class Apu
	{
	public:
		class In;
		Apu();

	private:
		class Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
