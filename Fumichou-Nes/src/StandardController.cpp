#include "stdafx.h"
#include "StandardController.h"

namespace Nes
{
	StandardController::StandardController()
	{
		m_controllers[0][ControllerButtons::A] = s3d::KeySlash;
		m_controllers[0][ControllerButtons::B] = s3d::KeyUnderscore_JIS;
		m_controllers[0][ControllerButtons::Select] = s3d::KeySpace;
		m_controllers[0][ControllerButtons::Start] = s3d::KeyEnter;
		m_controllers[0][ControllerButtons::Up] = s3d::KeyW;
		m_controllers[0][ControllerButtons::Down] = s3d::KeyS;
		m_controllers[0][ControllerButtons::Left] = s3d::KeyA;
		m_controllers[0][ControllerButtons::Right] = s3d::KeyD;
	}
}
