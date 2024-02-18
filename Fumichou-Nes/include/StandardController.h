#pragma once

namespace Nes
{
	// https://www.nesdev.org/wiki/Standard_controller

	namespace ControllerButtons
	{
		constexpr uint8 A{0};
		constexpr uint8 B{1};
		constexpr uint8 Select{2};
		constexpr uint8 Start{3};
		constexpr uint8 Up{4};
		constexpr uint8 Down{5};
		constexpr uint8 Left{6};
		constexpr uint8 Right{7};
	}

	constexpr uint8 ControllerCount_2 = 2;

	class StandardController
	{
	public:
		StandardController();
		using input_array = std::array<InputGroup, 8>;
		class In;

	private:
		bool m_strobe{};
		std::array<input_array, ControllerCount_2> m_controllers{};
		mutable std::array<uint8, ControllerCount_2> m_states{};
	};
}
