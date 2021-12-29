#include "UM-Memory.h"
#include "Entity.h"
#include "globalFunctions.h"
#include <iostream>
#include <windows.h> //for colors

Memory* mem = nullptr;
UINT64 StringAddress = 0;

void printInfo() {
	system(CLEAR);
	cout << "Current Multiplier: " << recoilMultiplierAdjustable << endl;
	wcout << L"Held Weapon: " << HeldWeaponCheck << endl;
	cout << "Last Gun Edited: " << recoilWeaponCheck << endl;
	if (defaultRecoilSettingsAutomatic.find(recoilWeaponCheck) != defaultRecoilSettingsAutomatic.end())
		cout << "Default Recoil Values: " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[0] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[1] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[2] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[3] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[4] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[5] << endl;
	if (editedRecoilAutomatic.find(recoilWeaponCheck) != editedRecoilAutomatic.end())
		cout << "Edited Recoil Values: " << editedRecoilAutomatic.at(recoilWeaponCheck)[0] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[1] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[2] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[3] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[4] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[5] << endl;
}

void gameLoop() {
	float recoilAdjustment = .1f;
	int delayTime = 100;
	while (true) {
		//printf("\t\tIN GAMELOOP:");
		//entityLoop();

		if (GetKeyState(VK_INSERT) & 0x8000) {
			system(BLUE);

			initializeSettings();
			entityLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInfo();
		}

		if (GetKeyState(VK_DELETE) & 0x8000) {
			system(RED);
			recoilMultiplierAdjustable = 1.0f;

			initializeSettings();
			entityLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInfo();
		}


		//INCREASE RECOIL MULTIPLIER
		if (GetKeyState(VK_UP) & 0x8000) {
			if (recoilMultiplierAdjustable + recoilAdjustment > 1.0f) {
				recoilMultiplierAdjustable = 1.0f;
			}
			else {
				recoilMultiplierAdjustable += recoilAdjustment;//increase recoil
			}
			printInfo();
			Sleep(delayTime);
		}


		//DECREASE RECOIL MULTIPLIER
		if (GetKeyState(VK_DOWN) & 0x8000) { //decrease recoil
			if (recoilMultiplierAdjustable - recoilAdjustment <= 0.0f) {
				recoilMultiplierAdjustable = 0.1f;
			} 
			else {
				recoilMultiplierAdjustable -= recoilAdjustment;//decrease recoil
			}
			printInfo();
			Sleep(delayTime);
		}


		if (GetKeyState(VK_END) & 0x8000) {
			break;
		}
	}
}

int main()
{
	printf("PID: %d", process_id);
	printf("Base Address: %Id", base_address);
	bool breakVal = false;
	
		process_id = mem->get_process_id("RustClient.exe");
		base_address = mem->get_module_base_address("GameAssembly.dll");
	

		if (!base_address)
		{
			printf("Could Not Find Game...");
			Sleep(5000);
		}
		else
		{
			printf("Game Found...\n");
			printf("PID: %d\n", process_id);
			printf("Base Address: %Id\n", base_address);
			printf("\tp: %p\n\n\n", base_address);
			
			
			initializeSettings();
			gameLoop();
			breakVal = true;
		}
	cout << "CLOSING";
	Sleep(1000);
	return NULL;
}