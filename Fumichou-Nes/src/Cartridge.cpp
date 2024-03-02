#include "stdafx.h"
#include "Cartridge.h"

#include "Boards/BoardNrom.h"

Nes::Cartridge::Cartridge() :
	m_board(CreateBoardNrom(RomData()))
{
}
