#include "stdafx.h"
#include "HwFrame.h"

struct Nes::HwFrame::Impl
{
	Hardware m_hardware{};
};

namespace Nes
{
	HwFrame::HwFrame() :
		p_impl(std::make_shared<Impl>())
	{
	}

	const Hardware& HwFrame::GetEnv()
	{
		return p_impl->m_hardware;
	}
}
