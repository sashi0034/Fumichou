#include "stdafx.h"
#include "KeyRepetition.h"

namespace
{
	struct RepeatData
	{
		// 入力
		double repeatDelay{};
		double repeatInterval{};

		// 状態
		double count{};
		double next{};
		bool isRepeating{};
	};

	struct InternalState
	{
		HashTable<uint8, RepeatData> tables{};
	};

	InternalState s_instance;

	class KeyRepetitionAddon : public IAddon
	{
	public:
		~KeyRepetitionAddon() override = default;

		bool init() override
		{
			return true;
		}

		bool update() override
		{
			const double dt = Scene::DeltaTime();
			for (auto& t : s_instance.tables)
			{
				const auto input = Input(InputDeviceType::Keyboard, t.first);
				if (input.pressed())
				{
					t.second.count += dt;
					if (input.down())
					{
						// 最初のフレーム
						t.second.isRepeating = true;
					}
					else if (t.second.count < t.second.next)
					{
						// リピート待機
						t.second.isRepeating = false;
					}
					else
					{
						// リピート発生
						t.second.isRepeating = true;
						t.second.next += t.second.repeatInterval;
					}
				}
				else
				{
					// 押されていない
					t.second.count = 0;
					t.second.next = t.second.repeatDelay;
					t.second.isRepeating = false;
				}
			}
			return true;
		}
	};

	bool createNewData(Input input, double repeatDelay, double repeatInterval)
	{
		s_instance.tables[input.code()] = RepeatData{
			.repeatDelay = repeatDelay,
			.repeatInterval = repeatInterval,
			.next = repeatDelay,
			.isRepeating = input.down()
		};
		return input.down();
	}
}

namespace Util
{
	bool IsKeyRepeating(Input input, double repeatDelay, double repeatInterval)
	{
		auto&& found = s_instance.tables.find(input.code());
		if (found != s_instance.tables.end())
		{
			found->second.repeatDelay = repeatDelay;
			found->second.repeatInterval = repeatInterval;
			return found->second.isRepeating;
		}

		// 初期化
		return createNewData(input, repeatDelay, repeatInterval);
	}

	void RegisterKeyRepetitionAddon()
	{
		Addon::Register<KeyRepetitionAddon>(U"KeyRepetition");
	}
}
