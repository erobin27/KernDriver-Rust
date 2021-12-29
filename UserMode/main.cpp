#include "UM-Memory.h"
#include "Entity.h"
#include "globalFunctions.h"
Memory* mem = nullptr;
UINT64 StringAddress = 0;

void gameLoop() {
	
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
		/*
		if (GetKeyState(VK_UP) & 0x8000)
		{
			Vars::Weapon::Recoil += .1f;//increase recoil
			printf("Current Recoil: %f", Vars::Weapon::Recoil);
		}

		if (GetKeyState(VK_DOWN) & 0x8000) { //decrease recoil
			if (Vars::Weapon::Recoil - .1f < 0) continue; else {
				Vars::Weapon::setRecoil(Vars::Weapon::Recoil - .1f, TRUE);
				printf("Current Recoil: %f", Vars::Weapon::Recoil);
			}
		}
		*/
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