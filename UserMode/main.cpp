#include "UM-Memory.h"
#include "globalFunctions.h"
#include "RenderGraphics.h"
#include "Entity.h"
#include <iostream>
#include <windows.h> //for colors
#include <iomanip>
#include <thread>


Memory* mem = nullptr;
UINT64 StringAddress = 0;
int RefreshDelaySeconds = 10;

class ConsoleMenu {
	vector<std::string> menuLines;
	std::map<std::string, int> LineNums;
public:
	int lineCount = 0;
	int selectedLine = 0;
	std::map<int, std::string> variableMap;

	void addMenuLine(std::string text, float num, std::string var) {
		stringstream stream;
		stream << std::fixed << std::setprecision(1) << num;
		menuLines.push_back(text + stream.str());
		LineNums[text] = lineCount;
		variableMap[lineCount] = var;
		lineCount++;
	}

	void updateMenuLine(std::string text, float num) {
		stringstream stream;
		stream << std::fixed << std::setprecision(1) << num;
		menuLines[LineNums.at(text)] = (text + stream.str());
	}

	int getLineNumberByText(std::string text) {
		return LineNums.at(text);
	}

	void setSelected(int num) {
		selectedLine = num;
	}

	std::string getSelectedLineVariable() {
		return variableMap[selectedLine];
	}

	void printMenu() {
		for (int i = 0; i < menuLines.size(); i++) {
			if (i == selectedLine) {
				//print green
				textcolor(4);
				cout << menuLines[i] << endl;
				textcolor(7);
			}
			else {
				cout << menuLines[i] << endl;
			}
		}
	}
};


void printInfo(ConsoleMenu menu) {
	menu.printMenu();
	wcout << L"Held Weapon: " << HeldWeaponCheck << endl;
	cout << skCrypt("Last Gun Edited: ") << recoilWeaponCheck << endl;
	if (defaultRecoilSettingsAutomatic.find(recoilWeaponCheck) != defaultRecoilSettingsAutomatic.end())
		cout << skCrypt("Default Recoil Values: ") << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[0] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[1] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[2] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[3] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[4] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[5] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[6] << ", " << defaultRecoilSettingsAutomatic.at(recoilWeaponCheck)[7] << endl;
	if (editedRecoilAutomatic.find(recoilWeaponCheck) != editedRecoilAutomatic.end())
		cout << skCrypt("Edited Recoil Values: ") << editedRecoilAutomatic.at(recoilWeaponCheck)[0] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[1] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[2] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[3] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[4] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[5] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[6] << ", " << editedRecoilAutomatic.at(recoilWeaponCheck)[7] << endl;
}

void printInstructions(ConsoleMenu menu) {
	system(CLEAR);
	printInfo(menu);
	cout << "\n\n\n\n\n\n\n\n" << endl;
	cout << skCrypt("INSERT: Apply recoil and aimcone effects") << endl;
	cout << skCrypt("DEL: Revert recoil and aimcone to default values") << endl;
	cout << skCrypt("UP/DOWN: Move up and down through the menu") << endl;
	cout << skCrypt("LEFT/RIGHT: Adjust variables up and down") << endl;
	cout << skCrypt("HOME: Start Radar Scanning\t\t END: hold to end radar scanning") << endl;
}

void sonarLoop(Radar& myRadar) {
	//UNTIL END IS PRESSED 
	int count = 1;
	GameData gData;
	radarLoop(myRadar, gData, true);
	while (!(GetKeyState(VK_END) & 0x8000)) {

		//if count % (1000ms in a second * 10 Seconds)/10ms Sleep time we use == 0
		if (count % ((100 * RefreshDelaySeconds) / 10) == 0) {
			radarLoop(myRadar, gData, true);		//Detect all entities
			count = 1;
		}
		else
		{
			radarLoop(myRadar, gData, false);		//Detect players and check if static entities have been destroyed
		}

		count++;
		Sleep(2);
	}
}

void draw(Radar& myRadar) {
	bool drawRadar = false;
	myRadar.createRadarWindow();
	while (true) {
		if (drawRadar) {
			myRadar.drawSonar();	//Draw entities on the radar

			if (GetKeyState(VK_DOWN) & 0x8000) {	//if down arrow pressed
				myRadar.menu.nextItem();
				Sleep(100);
			}
			if (GetKeyState(VK_UP) & 0x8000) {		//if up arrow pressed
				myRadar.menu.previousItem();
				Sleep(100);
			}
			if (GetKeyState(VK_LEFT) & 0x8000 || GetKeyState(VK_RIGHT) & 0x8000) {	//if left/right arrow pressed
				myRadar.menu.changeItem();
				Sleep(100);
			}
			Sleep(2);
		}
		else {
			myRadar.drawBlank();
		}

		if (drawRadar && (GetKeyState(VK_END) & 0x8000)) drawRadar = !drawRadar;
		if (!drawRadar && (GetKeyState(VK_HOME) & 0x8000)) drawRadar = !drawRadar;
	}
}

void gameLoop() {
	float recoilAdjustment = .1f;
	float aimconeAdjustment = .1f;
	int radarAdjustment = 10;
	int delayTime = 100;

	ConsoleMenu menu;
	menu.addMenuLine("Current Recoil Multiplier: ", recoilMultiplierAdjustable, "RECOIL");
	menu.addMenuLine("Current AimCone Multiplier: ", aimconeMultiplier, "AIMCONE");
	menu.addMenuLine("Current Radar distance: ", radarDistance, "DISTANCE");
	printInstructions(menu);

	Radar myRadar = Radar(1000, 1000);
	std::thread drawingThread(draw, std::ref(myRadar));		//created thread never access the game

	while (true) {
		//printf("\t\tIN GAMELOOP:");
		//entityLoop();
		if (GetKeyState(VK_DOWN) & 0x8000) {
			if (!(menu.selectedLine + 1 == menu.lineCount)) {
				menu.setSelected(menu.selectedLine + 1);
			}
			printInstructions(menu);
			Sleep(100);
		}

		if (GetKeyState(VK_UP) & 0x8000) {
			if (!(menu.selectedLine - 1 == -1)) {
				menu.setSelected(menu.selectedLine - 1);
			}
			printInstructions(menu);
			Sleep(100);
		}


		//increase
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			if (menu.getSelectedLineVariable() == "RECOIL") {
				if (recoilMultiplierAdjustable + recoilAdjustment > 1.0f) {
					recoilMultiplierAdjustable = 1.0f;
				}
				else {
					recoilMultiplierAdjustable += recoilAdjustment;//increase recoil
				}
				menu.updateMenuLine("Current Recoil Multiplier: ", recoilMultiplierAdjustable);
			}
			else if (menu.getSelectedLineVariable() == "AIMCONE") {
				if (aimconeMultiplier + aimconeAdjustment > 1.0f) {
					aimconeMultiplier = 1.0f;
				}
				else {
					aimconeMultiplier += aimconeAdjustment;//increase recoil
				}
				menu.updateMenuLine("Current AimCone Multiplier: ", aimconeMultiplier);
			}
			else if (menu.getSelectedLineVariable() == "DISTANCE") {
				if (radarDistance + radarAdjustment >= 600) {
					radarDistance = 600;
				}
				else {
					radarDistance += radarAdjustment;//increase radar
				}
				menu.updateMenuLine("Current Radar distance: ", radarDistance);
			}
			
			printInstructions(menu);
			Sleep(delayTime);
		}

		if (GetKeyState(VK_LEFT) & 0x8000) { //decrease aimcone
			if (menu.getSelectedLineVariable() == "RECOIL") {
				if (recoilMultiplierAdjustable - recoilAdjustment <= 0.1f) {
					recoilMultiplierAdjustable = 0.1f;
				}
				else {
					recoilMultiplierAdjustable -= recoilAdjustment;//decrease recoil
				}
				menu.updateMenuLine("Current Recoil Multiplier: ", recoilMultiplierAdjustable);
			}
			else if (menu.getSelectedLineVariable() == "AIMCONE") {
				if (aimconeMultiplier - aimconeAdjustment <= .1f) {
					aimconeMultiplier = .1f;
				}
				else {
					aimconeMultiplier -= aimconeAdjustment;//decrease aimcone
				}
				menu.updateMenuLine("Current AimCone Multiplier: ", aimconeMultiplier);
			}
			else if (menu.getSelectedLineVariable() == "DISTANCE") {
				if (radarDistance - radarAdjustment <= 0) {
					radarDistance = 10;
				}
				else {
					radarDistance -= radarAdjustment;//decrease aimcone
				}
				menu.updateMenuLine("Current Radar distance: ", radarDistance);
				printInstructions(menu);
			}

			printInstructions(menu);
			Sleep(delayTime);
		}





		if (GetKeyState(VK_INSERT) & 0x8000) {
			system(BLUE);

			initializeSettings();
			basePlayerLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInstructions(menu);
		}

		if (GetKeyState(VK_DELETE) & 0x8000) {
			system(RED);
			recoilMultiplierAdjustable = 1.0f;
			aimconeMultiplier = 1.0f;

			menu.updateMenuLine("Current Recoil Multiplier: ", recoilMultiplierAdjustable);
			menu.updateMenuLine("Current AimCone Multiplier: ", aimconeMultiplier);

			initializeSettings();
			basePlayerLoop();

			Sleep(delayTime);
			system(CLEAR);
			system(WHITE);
			printInstructions(menu);
		}


		//Radar
		if (GetKeyState(VK_HOME) & 0x8000) {
			system(CLEAR);
			std::cout << skCrypt("hold END until menu reappears...\n");
			sonarLoop(myRadar); //until end is pressed

			//After END is pressed clear the console and print the main menu
			system(WHITE);
			printInstructions(menu);
		}
	}
}

int main()
{
	bool breakVal = false;
	
	auto gameName = skCrypt("RustClient.exe");
	auto gameBase = skCrypt("GameAssembly.dll");
	process_id = mem->get_process_id(gameName.decrypt());
	base_address = mem->get_module_base_address(gameBase.decrypt());
	gameName.clear();
	gameBase.clear();


	if (!base_address)
	{
		printf(skCrypt("Could Not Find Game..."));
		Radar myRadar = Radar(1000, 1000);
		int c = 0;
		while (true) {
			//std::thread drawingThread(draw, std::ref(myRadar))
			myRadar.drawWindowTesting();
			myRadar.clearBlips();
			Sleep(100);
		}

		Sleep(5000);
	}
	else
	{
		printf(skCrypt("Game Found...\n"));
		
		initializeSettings();
		gameLoop();
		breakVal = true;
	}
	cout << skCrypt("CLOSING");
	Sleep(1000);
	return NULL;
}