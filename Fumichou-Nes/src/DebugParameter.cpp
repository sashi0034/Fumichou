#include "stdafx.h"
#include "DebugParameter.h"

namespace
{
	Nes::DebugParameter s_instance{};
}

namespace Nes
{
	DebugParameter& DebugParameter::Instance()
	{
		return s_instance;
	}
}
