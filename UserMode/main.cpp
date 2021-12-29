#include "UM-Memory.h"
#include "Entity.h"
#include "globalFunctions.h"
#include <iostream>
#include <windows.h> //for colors

Memory* mem = nullptr;
UINT64 StringAddress = 0;

void gameLoop() {
	float recoilAdjustment = .1f;
	while (true) {
		//printf("\t\tIN GAMELOOP:");
		//entityLoop();

		if (GetKeyState(VK_INSERT) & 0x8000) {
			initializeSettings();
			entityLoop();
			Sleep(1000);
		}



		if (GetKeyState(VK_END) & 0x8000) {
			break;
		}


		//INCREASE RECOIL MULTIPLIER
		if (GetKeyState(VK_UP) & 0x8000) {
			if (recoilMultiplierAdjustable + recoilAdjustment > 1.0f) {
				recoilMultiplierAdjustable = 1.0f;
				system(CLEAR);
				system(BLUE);
				cout << "Current Recoil Multiplier: " << recoilMultiplierAdjustable;
			}
			else {
				recoilMultiplierAdjustable += recoilAdjustment;//increase recoil
				system(CLEAR);
				system(BLUE);
				cout << "Current Recoil Multiplier: " << recoilMultiplierAdjustable;
			}
			Sleep(300);
		}


		//DECREASE RECOIL MULTIPLIER
		if (GetKeyState(VK_DOWN) & 0x8000) { //decrease recoil
			if (recoilMultiplierAdjustable - recoilAdjustment <= 0.0f) {
				recoilMultiplierAdjustable = 0.1f;
				system(CLEAR);
				system(AQUA);
				cout << "Current Recoil Multiplier: " << recoilMultiplierAdjustable;
			} 
			else {
				recoilMultiplierAdjustable -= recoilAdjustment;//decrease recoil
				system(CLEAR);
				system(AQUA);
				cout << "Current Recoil Multiplier: " << recoilMultiplierAdjustable;
				//printf("Current Recoil: %f", recoilMultiplierAdjustable);
			}
			Sleep(300);
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