/*This file is part of Output Blaster.

Output Blaster is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Output Blaster is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Output Blaster.If not, see < https://www.gnu.org/licenses/>.*/

#include "WackyRaces.h"

static int WindowsLoop()
{
	INT_PTR lampbase = helpers->ReadIntPtr(0x7E00590, true);
	INT_PTR lamps = helpers->ReadIntPtr(lampbase + 0x45, false);
	UINT8 Seatlamp1 = helpers->ReadByte(lampbase + 0x53, false);
	UINT8 Seatlamp2 = helpers->ReadByte(lampbase + 0x51, false);
	UINT8 Seatlamp3 = helpers->ReadByte(lampbase + 0x23, false);
	UINT8 Seatlamp4 = helpers->ReadByte(lampbase + 0x4D, false);
	UINT8 Seatlamp5 = helpers->ReadByte(lampbase + 0x1F, false);
	UINT8 Seatlamp6 = helpers->ReadByte(lampbase + 0x49, false);

	Outputs->SetValue(OutputLampStart, !!(lamps & 0x80));
	Outputs->SetValue(OutputLampView1, !!(lamps & 0x100));
	Outputs->SetValue(OutputPanelLamp, !!(lamps & 0x08));
	Outputs->SetValue(OutputSlot1Lamp, !!(lamps & 0x400));
	Outputs->SetValue(OutputSlot2Lamp, !!(lamps & 0x10));
	Outputs->SetValue(OutputSlot3Lamp, !!(lamps & 0x200));
	Outputs->SetValue(OutputSeat1Lamp, !!(Seatlamp1 & 0xFF));
	Outputs->SetValue(OutputSeat2Lamp, !!(Seatlamp2 & 0xFF));
	Outputs->SetValue(OutputSeat3Lamp, !!(Seatlamp3 & 0xFF));
	Outputs->SetValue(OutputSeat4Lamp, !!(Seatlamp4 & 0xFF));
	Outputs->SetValue(OutputSeat5Lamp, !!(Seatlamp5 & 0xFF));
	Outputs->SetValue(OutputSeat6Lamp, !!(Seatlamp6 & 0xFF));
	Outputs->SetValue(OutputSeat7Lamp, !!(lamps & 0x4000));
	Outputs->SetValue(OutputFFB, !!(lamps & 0x2000));
	Outputs->SetValue(OutputFFB1, !!(lamps & 0x1000));
	Outputs->SetValue(OutputFFB2, !!(lamps & 0x40));
	Outputs->SetValue(OutputFFB3, !!(lamps & 0x800));
	Outputs->SetValue(OutputFFB4, !!(lamps & 0x20));
	return 0;
}

static DWORD WINAPI OutputsAreGo(LPVOID lpParam)
{
	while (true)
	{
		WindowsLoop();
		Sleep(SleepA);
	}
}

void WackyRaces::OutputsGameLoop()
{
	if (!init)
	{
		Outputs = CreateOutputsFromConfig();
		m_game.name = "Wacky Races";
		Outputs->SetGame(m_game);
		Outputs->Initialize();
		Outputs->Attached();
		CreateThread(NULL, 0, OutputsAreGo, NULL, 0, NULL);
		while (GetMessage(&Msg1, NULL, NULL, 0))
		{
			TranslateMessage(&Msg1);
			DispatchMessage(&Msg1);
		}
		init = true;
	}
}