#pragma once

namespace Nes
{
	class Apu
	{
	public:
		class In;
		Apu();

		const std::shared_ptr<s3d::IAudioStream>& GetStream() const;

	private:
		class Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
