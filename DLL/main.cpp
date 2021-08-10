// CSGOHackRedux.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"
/*
 Opens a handle to the CS:GO process and retrieves the memory locations of client.dll and engine.dll
*/
void getAddress() {
	DWORD pid;
	HWND hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	GetWindowThreadProcessId(hwnd, &pid);
	ProcessID = pid;
	HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, pid);
	HMODULE hMods[1024];
	DWORD cbNeeded;
	MODULEINFO modInf{ 0 };
	TCHAR modNames[1024];
	if (EnumProcessModules(h, hMods, sizeof(hMods), &cbNeeded)) {
		for (uint32_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			if (GetModuleFileNameEx(h, hMods[i], (LPWSTR)modNames, sizeof(modNames) / sizeof(TCHAR))) {
				if (wcsstr(modNames, L"\\csgo\\bin\\client.dll")) {
					if (GetModuleInformation(h, hMods[i], &modInf, sizeof(MODULEINFO)))
						ClientAddress = (DWORD)modInf.lpBaseOfDll;
				}
				else if (wcsstr(modNames, L"\\bin\\engine.dll")) {
					if (GetModuleInformation(h, hMods[i], &modInf, sizeof(modInf)))
						EngineAddress = (DWORD)modInf.lpBaseOfDll;
				}
			}
		}
		CloseHandle(h);
		return;
	}
	else {
		CloseHandle(h);
		return;
	}
}
/*
 Reads the current loaded map from CS:GO and passes it to the BSP Parser
*/
void loadMap() {
	std::string mapname = "";
	//read mapname
	for (int i = 0; i < 4; i++) {
		std::string temp = "";
		DWORD map = DRIVER.ReadMemory<DWORD>(ProcessID, ClientState + offsets::dwClientState_Map + i * 4, 4);
		std::string line;
		std::stringstream name(std::bitset<32>(map).to_string());
		std::getline(name, line);
		std::istringstream in(line);
		std::bitset<8> bs;
		while (in >> bs) {
			if (char(bs.to_ulong()) == '\0') {
				i = 99;
			}
			else {
				temp += char(bs.to_ulong());
			}
		}
		reverse(temp.begin(), temp.end());
		mapname += temp;
	}

	//input your own csgo dir
	bsp.load_map("Counter-Strike Global Offensive\\csgo\\maps", mapname + ".bsp");
}
/*
 Exposed function for updating the settings from the GUI
  @param pointer to a populated settings object
*/

void __stdcall updateSettings(const Settings& newSettings) {
	currentSettings = newSettings;
	currentSettings.aimTime /= 1000;
	FriendlyGlow.r = newSettings.friendly[0] / 255.f;
	FriendlyGlow.g = newSettings.friendly[1] / 255.f;
	FriendlyGlow.b = newSettings.friendly[2] / 255.f;
	EnemyGlow.r = newSettings.enemy[0] / 255.f;
	EnemyGlow.g = newSettings.enemy[1] / 255.f;
	EnemyGlow.b = newSettings.enemy[2] / 255.f;
}
/*
 Exposed function for updating the Anti Flash variable
*/
void __stdcall updateAntiFlash() {
	DRIVER.WriteMemory(ProcessID, LocalPlayer + offsets::m_flFlashMaxAlpha, 255.f - (currentSettings.antiFlash * 255.f));
}

/* 
 Exposed function for conducting BHop reads/write
*/
void __stdcall doBhop() {
	while (currentSettings.bhopOn) {
		if (GetAsyncKeyState(32) != 0) {
			if (DRIVER.ReadMemory<int>(ProcessID, LocalPlayer + offsets::m_fFlags, 4) == 257) {
				DRIVER.WriteMemory(ProcessID, ClientAddress + offsets::dwForceJump, 6);
			}
		}
		Sleep(5);
	}
	
}
/*
 Exposed function for initializing the needed variables for proper function
 @return Status code 
*/
int __stdcall init() {
	getAddress();
	LocalPlayer = DRIVER.ReadMemory<DWORD>(ProcessID, ClientAddress + offsets::dwLocalPlayer, 4);
	ClientState = DRIVER.ReadMemory<DWORD>(ProcessID, EngineAddress + offsets::dwClientState, 4);
	teamId = DRIVER.ReadMemory<int>(ProcessID, LocalPlayer + offsets::m_iTeamNum, 4);
	updateAntiFlash();
	loadMap();
	if (LocalPlayer == 2) return LocalPlayer;
	return !(ClientAddress == 0 || EngineAddress == 0);
}
/*
 Exposed function for conducting all repeating activities
*/
void __stdcall mainLoop() {

	while (currentSettings.running) {
		LocalPlayerID = DRIVER.ReadMemory<DWORD>(ProcessID, ClientState + offsets::dwClientState_GetLocalPlayer, 4);
		aimbot.setAimPunch(DRIVER.ReadMemory<Vector3>(ProcessID, LocalPlayer + offsets::m_aimPunchAngle, sizeof(Vector3)));
		int crosshairId = DRIVER.ReadMemory<int>(ProcessID, LocalPlayer + offsets::m_iCrosshairId, 4);


		Vector3 viewAngles = DRIVER.ReadMemory<Vector3>(ProcessID, ClientState + offsets::dwClientState_ViewAngles, 12);
		viewAngles.z = 0;

		Vector3 playerPos = DRIVER.ReadMemory<Vector3>(ProcessID, LocalPlayer + offsets::m_vecOrigin, 12);
		playerPos.z += DRIVER.ReadMemory<float>(ProcessID, LocalPlayer + offsets::m_vecViewOffset + 0x8, 4);

		int target = -1;
		bool hasTarget = false;
		Vector3 closestAng;
		float closestTarget = 0xFFFFFF;

		for (int i = 2; i < 32; i++) {

			DWORD entLoc = DRIVER.ReadMemory<DWORD>(ProcessID, ClientAddress + offsets::dwEntityList + ((i - 1) * 0x10), sizeof(DWORD));
			if (entLoc == 0) {
				i = 100;
				break;
			}
			EntityStruct entity = DRIVER.ReadMemory<EntityStruct>(ProcessID, entLoc, sizeof(EntityStruct));



			if (entity.health > 0 && !entity.dormant) {

				int glowIndex = 0x38 * DRIVER.ReadMemory<int>(ProcessID, entLoc + offsets::m_iGlowIndex, 4);

				if (teamId != entity.teamNum) {
					if (currentSettings.enemyESP > 0 && (currentSettings.enemyESP == 2 || entity.spottedMask & (1 << LocalPlayerID))) {
						doGlow(false, glowIndex, entity.health);
					}

					Vector3 headPos = DRIVER.ReadGappedVec3(ProcessID, entity.boneMatrix + (0x30 * currentSettings.boneId + 0xC), 16);

					if (currentSettings.radar) DRIVER.WriteMemory(ProcessID, entLoc + offsets::m_bSpotted, true);
					//std::cout << entity + offsets::m_bSpotted << std::endl;
					if (currentSettings.triggerOn) triggerBot(i, crosshairId);
					//bsp.is_visible({ playerPos.x,playerPos.y,playerPos.z }, { headPos.x, headPos.y, headPos.z })
					if (currentSettings.aimOn && entity.spottedMask & (1 << LocalPlayerID)) {

						if (aimbot.findClosestTarget(playerPos, viewAngles, headPos, &closestTarget, &closestAng)) {
							hasTarget = true;
							target = i;
						}
					}
				}
				else {
					if (currentSettings.teamESP) doGlow(true, glowIndex, 100);
				}
			}

		}
		
		waitTime -= timeInc;
		if (hasTarget && waitTime <= 0) {

			
			if (currentSettings.silentAim && (GetAsyncKeyState(1) != 0)) {
				DRIVER.WriteMemory(ProcessID, ClientState + offsets::dwClientState_ViewAngles, FixAngs(closestAng));
				DRIVER.WriteMemory(ProcessID, ClientAddress + offsets::dwForceAttack, 1);

				Sleep(15);

				DRIVER.WriteMemory(ProcessID, ClientAddress + offsets::dwForceAttack, 0);

				waitTime = currentSettings.antiTransfer;
				DRIVER.WriteMemory(ProcessID, ClientState + offsets::dwClientState_ViewAngles, viewAngles);

			}
			else {
				DRIVER.WriteMemory(ProcessID, ClientState + offsets::dwClientState_ViewAngles, aimbot.getNewAngles(viewAngles, closestAng, target));
			}
				
		}	
		else {
			if (waitTime <= 0) aimbot.updateTarget(-1);
			Vector3 rcs = aimbot.RecoilControl(viewAngles, false);
			if (rcs == viewAngles) {

			}
			else {
				DRIVER.WriteMemory(ProcessID, ClientState + offsets::dwClientState_ViewAngles, rcs);
			}
			
		}
		Sleep(sleepTime);
	}
	
}
/*
 Handles wall hacks/spectator glow
 @param If the entity is friendly or not
 @param The glow index of the entity
 @param The entity's health
*/
void doGlow(bool isFriendly, int index, int health) {

	GlowStruct glow = isFriendly ? FriendlyGlow : EnemyGlow;
	DWORD GlowObjectManager = DRIVER.ReadMemory<DWORD>(ProcessID, ClientAddress + offsets::dwGlowObjectManager, 4);
	GlowStruct readGlow = DRIVER.ReadMemory<GlowStruct>(ProcessID, GlowObjectManager + index + 0x8, sizeof(GlowStruct));
	//DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x8, glow, sizeof(glow));
	readGlow.r = glow.r * health / 100.0f;
	readGlow.g = glow.g;
	readGlow.b = glow.b * health / 100.0f;
	readGlow.a = glow.a;
	readGlow.rwo = glow.rwo;
	readGlow.rwuo = glow.rwuo;
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x8, readGlow, sizeof(readGlow));
	/*DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x8, glow.r);
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0xC, glow.g);
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x10, glow.b);
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x14, glow.a);
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x28, glow.rwo);
	DRIVER.WriteMemory(ProcessID, GlowObjectManager + index + 0x29, glow.rwuo);*/
}
/*
 Handles the trigger bot functionality
 @param Id of an enemy entity
 @param Id of entity in players crosshair
*/
void triggerBot(int enemyId, int crosshairId) {
	if (crosshairId == enemyId) {
		DRIVER.WriteMemory(ProcessID, ClientAddress + offsets::dwForceAttack, 1);
		Sleep(5);
		DRIVER.WriteMemory(ProcessID, ClientAddress + offsets::dwForceAttack, 0);
	}
}




