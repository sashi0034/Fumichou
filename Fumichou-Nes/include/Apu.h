#pragma once

namespace Nes
{
	class Apu
	{
	public:
		class Impl;
		Apu();

	private:
		std::shared_ptr<Impl> p_impl;
	};
}
