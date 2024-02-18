#include "stdafx.h"
#include "StandardController.h"

namespace Nes
{
	StandardController::StandardController()
	{
		m_controllers[0][ControllerButtons::A] = KeySlash;
		m_controllers[0][ControllerButtons::B] = KeyUnderscore_JIS;
		m_controllers[0][ControllerButtons::Select] = KeySpace;
		m_controllers[0][ControllerButtons::Start] = KeyEnter;
		m_controllers[0][ControllerButtons::Up] = KeyW;
		m_controllers[0][ControllerButtons::Down] = KeyS;
		m_controllers[0][ControllerButtons::Left] = KeyA;
		m_controllers[0][ControllerButtons::Right] = KeyD;
	}
}
