#include "UM-Memory.h"
#include "Entity.h"
#include "globalFunctions.h"
#include <iostream>
#include <windows.h> //for colors
#include <GLFW/glfw3.h>

Memory* mem = nullptr;
UINT64 StringAddress = 0;

int DrawRadar() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Sonar", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void printInfo() {
	cout << "Current Recoil Multiplier: " << recoilMultiplierAdjustable << endl;
	cout << "Current AimCone Multiplier: " << aimconeMultiplier << endl;
	cout << "Current Radar distance: " << radarDistance << endl;
	wcout << L"Held Weapon: " << HeldWeaponCheck << endl;
	cout << "Last Gun Edited: " << recoilWeaponCheck << endl;
	if (defaultRecoilSettingsAutomatic.find(recoilWeaponCheck) != defaultRecoilSettingsAutomatic.end())
		cout << "Default Recoil Values: " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[0] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[1] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[2] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[3] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[4] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[5] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[6] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[7] << endl;
	if (editedRecoilAutomatic.find(recoilWeaponCheck) != editedRecoilAutomatic.end())
		cout << "Edited Recoil Values: " << editedRecoilAutomatic.at(recoilWeaponCheck)[0] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[1] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[2] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[3] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[4] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[5] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[6] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[7] << endl;
}

void printInstructions() {
	system(CLEAR);
	printInfo();
	cout << "\n\n\n\n\n\n\n\n" << endl;
	cout << "INSERT: Apply recoil and aimcone effects" << endl;
	cout << "DEL: Revert recoil and aimcone to default values" << endl;
	cout << "UP/DOWN: Adjust recoil multiplier" << endl;
	cout << "LEFT/RIGHT: Adjust aimcone multiplier" << endl;
	cout << "PAGE_UP/PAGE_DOWN: Adjust radar distance" << endl;
	cout << "HOME: Start Radar Scanning\t\t END: hold to end radar scanning" << endl;
}

void gameLoop() {
	float recoilAdjustment = .1f;
	float aimconeAdjustment = .1f;
	int radarAdjustment = 10;
	int delayTime = 100;

	printInstructions();

	while (true) {
		//printf("\t\tIN GAMELOOP:");
		//entityLoop();

		if (GetKeyState(VK_INSERT) & 0x8000) {
			system(BLUE);

			initializeSettings();
			basePlayerLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInstructions();
		}

		if (GetKeyState(VK_DELETE) & 0x8000) {
			system(RED);
			recoilMultiplierAdjustable = 1.0f;
			aimconeMultiplier = 1.0f;

			initializeSettings();
			basePlayerLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInstructions();
		}

		//Radar
		if (GetKeyState(VK_HOME) & 0x8000) {
			Sleep(1000);
			while (!(GetKeyState(VK_END) & 0x8000)) {
				system(CLEAR);
				entityLoop();
				Sleep(1000);
			}
			system(WHITE);
			printInstructions();
		}


		//INCREASE RECOIL MULTIPLIER
		if (GetKeyState(VK_UP) & 0x8000) {
			if (recoilMultiplierAdjustable + recoilAdjustment > 1.0f) {
				recoilMultiplierAdjustable = 1.0f;
			}
			else {
				recoilMultiplierAdjustable += recoilAdjustment;//increase recoil
			}
			printInstructions();
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
			printInstructions();
			Sleep(delayTime);
		}


		//INCREASE AIMCONE MULTIPLIER
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			if (aimconeMultiplier + aimconeAdjustment > 1.0f) {
				aimconeMultiplier = 1.0f;
			}
			else {
				aimconeMultiplier += aimconeAdjustment;//increase recoil
			}
			printInstructions();
			Sleep(delayTime);
		}

		//DECREASE AIMCONE MULTIPLIER
		if (GetKeyState(VK_LEFT) & 0x8000) { //decrease aimcone
			if (aimconeMultiplier - aimconeAdjustment <= 0.0f) {
				aimconeMultiplier = 0.1f;
			}
			else {
				aimconeMultiplier -= aimconeAdjustment;//decrease aimcone
			}
			printInstructions();
			Sleep(delayTime);
		}

		if (GetKeyState(VK_PRIOR) & 0x8000) { // PAGE UP INCREASE RADAR
			if (radarDistance + radarAdjustment >= 400) {
				radarDistance = 400;
			}
			else {
				radarDistance += radarAdjustment;//increase radar
			}
			printInstructions();
			Sleep(delayTime);
		}

		if (GetKeyState(VK_NEXT) & 0x8000) { // PAGE UP INCREASE RADAR
			if (radarDistance - radarAdjustment <= 0) {
				radarDistance = 10;
			}
			else {
				radarDistance -= radarAdjustment;//decrease aimcone
			}
			printInstructions();
			Sleep(delayTime);
		}



		//if (GetKeyState(VK_END) & 0x8000) {
		//	break;
		//}
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
			DrawRadar();
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