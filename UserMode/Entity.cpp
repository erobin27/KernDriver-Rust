#include "Entity.h"
#include "GameSDK.h"
#include "AimFunc.h"
#include <codecvt>

bool mfound = false;
LPlayerBase LocalPlayer;

enum RadarColors {
	Enemy = 1,
	ToolCupboard = 2,
	GroundItem = 3,
	Sleeper = 4,
};

float toDegrees(float val) {
	return val / 3.141592653 * 180;
}

std::string wstring_to_string(const std::wstring& wstr)
{
	static std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > converter;

	return converter.to_bytes(wstr);
}

std::string readCharString(DWORD64 address, int length) {
	std::string str = "";
	for (int i = 0; i < length; i++) {
		str += mem->Read<char>(address + i);
	}

	return str;
}

std::wstring readWCharString(DWORD64 address, int length) {
	std::wstring str = L"";
	for (int i = 0; i < length; i++) {
		//std::wcout << mem->Read<UCHAR>(address + i * 2);
		str += mem->Read<wchar_t>(address + i * 2);
	}

	return str;
}

void basePlayerLoop() {
	//printf("++++++++++++++++++++++++++++++++ENTITYLOOP\n");
	bool LP_isValid = false;
	DWORD64 BaseNetworkable;
	BaseNetworkable = mem->Read<DWORD64>(mem->get_module_base_address("GameAssembly.dll") + g_BN_Steam);
	DWORD64 EntityRealm = mem->Read<DWORD64>(BaseNetworkable + oEntityRealm);
	DWORD64 ClientEntities = mem->Read<DWORD64>(EntityRealm);
	DWORD64 ClientEntities_list = mem->Read<DWORD64>(ClientEntities + oClientEntitiesList);
	DWORD64 ClientEntities_values = mem->Read<DWORD64>(ClientEntities_list + oClientEntitiesValues);
	if (!ClientEntities_values) return;
	int EntityCount = mem->Read<int>(ClientEntities_values + 0x10);
	//printf("EntityCount: %i\n",EntityCount);
	DWORD64 EntityBuffer = mem->Read<DWORD64>(ClientEntities_values + 0x18);
	
	for (int i = 0; i <= EntityCount; i++)
	{
		DWORD64 Entity = mem->Read<DWORD64>(EntityBuffer + 0x20 + (i * 0x8));
		if (Entity <= 100000) continue;
		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = mem->Read<DWORD64>(Object + 0x30);
		if (ObjectClass <= 100000) continue;
		DWORD64 ObjectName = mem->Read<DWORD64>(ObjectClass + 0x60);
		std::string name = readCharString(ObjectName, 30);
		if (name.find(std::string("Local")) != std::string::npos) {//if local is contained in class name
			//printf("\nName: %s\n", name.c_str());
			BasePlayer* Player = (BasePlayer*)mem->Read<DWORD64>(Object + 0x28);
			if (!mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + 0x50) || mem->Read<bool>(mem->Read<DWORD64>(Object + 0x28) + 0x18) || !mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + oPlayerModel))
			{
				printf("-------------ERROR\n");
				continue;
			}
			/*
			if (!mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + 0x50))
				printf("1");
			if (mem->Read<bool>(mem->Read<DWORD64>(Object + 0x28) + 0x28) )
				printf("2");
			if (!mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + oPlayerModel)) 
				printf("3");
			*/
			if (Player != LocalPlayer.BasePlayer) {
				//printf("LocalPlayer %lld\n", Player->GetSteamID());
				mfound = false;
			}

			//Vector3 pos = Player->GetPosition();
			//printf("lp pos %f %f %f\n", pos.x, pos.y, pos.z);
			LocalPlayer.BasePlayer = Player;
			//printf("LocalPlayer %lld\n", Player->GetSteamID());
			LP_isValid = true;

			//once LP is found break out of for loop - Decreases LAG
			break;
		}
	}

	if (LP_isValid) {
		printf("++++++++++++++++++++++++++++++++LP VALID\n");
		WeaponPatch(); //call hacks
	}
	else {
		LocalPlayer.BasePlayer = nullptr;
	}
}

void radarLoop(Radar &myRadar) {
	myRadar.clearBlips();

	BasePlayer* LocalPlayer = nullptr;
	Vector3 LocalPos;
	ULONG LocalTeam;
	int LocalLookingDegree;
	
	/*
	* 
	* FIND THE ENTITY LOOP
	* 
	*/
	std::vector<BasePlayer*> Players;
	DWORD64 BaseNetworkable;
	BaseNetworkable = mem->Read<DWORD64>(mem->get_module_base_address("GameAssembly.dll") + g_BN_Steam);
	DWORD64 EntityRealm = mem->Read<DWORD64>(BaseNetworkable + oEntityRealm);
	DWORD64 ClientEntities = mem->Read<DWORD64>(EntityRealm);
	DWORD64 ClientEntities_list = mem->Read<DWORD64>(ClientEntities + oClientEntitiesList);
	DWORD64 ClientEntities_values = mem->Read<DWORD64>(ClientEntities_list + oClientEntitiesValues);
	if (!ClientEntities_values) return;
	int EntityCount = mem->Read<int>(ClientEntities_values + 0x10);
	//printf("EntityCount: %i\n",EntityCount);
	DWORD64 EntityBuffer = mem->Read<DWORD64>(ClientEntities_values + 0x18);


	/*
	* 
	* LOOP THROUGH ALL ENTITY'S AND FIND ALL THE PLAYERS
	* 
	*/
	for (int i = 0; i <= EntityCount; i++)
	{
		DWORD64 Entity = mem->Read<DWORD64>(EntityBuffer + 0x20 + (i * 0x8));
		if (Entity <= 100000) continue;
		DWORD64 Obj = mem->Read<DWORD64>(Entity + 0x0);
		DWORD64 ObjName = mem->Read<DWORD64>(Obj + 0x10);
		std::string ClassName = readCharString(ObjName, 16);
		//std::cout << ClassName << std::endl;
		//std::cout << LocalPos.Length() << std::endl;

		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = mem->Read<DWORD64>(Object + 0x28);
		//std::cout << ClassName << std::endl;
		//std::cout << ClassName << std::endl;
		/*
		if (ClassName.find(std::string("LootContainer")) != std::string::npos) {
			std::cout << ClassName << std::hex << 0x20 + (i * 0x8) << std::endl;
			DWORD64 storNamePtr = mem->Read<DWORD64>(ObjectClass + 0x368);
			int storNameLen = mem->Read<int>(storNamePtr + 0x10);
			DWORD64 storName = storNamePtr + 0x14;
			std::wstring wstorString = readWCharString(storName, storNameLen);
			//std::cout << wstring_to_string(wstorString) << std::endl;
		}

		if (ClassName.find(std::string("StorageContainer")) != std::string::npos) {
			std::cout << ClassName << std::hex << 0x20 + (i * 0x8) << std::endl;
			DWORD64 storNamePtr = mem->Read<DWORD64>(Entity + 0x60);
			int storNameLen = mem->Read<int>(storNamePtr + 0x10);
			DWORD64 storName = storNamePtr + 0x14;
			std::wstring wstorString = readWCharString(storName, storNameLen);
			//std::cout << wstring_to_string(wstorString) << std::endl;
		}
		if (ClassName.find(std::string("Bear")) != std::string::npos) {
			std::cout << ClassName << std::endl;
		}
		/*

		/*
		* 
		* IF THE ENTITY IS A PLAYER
		* 
		*/
		if (ClassName.find(std::string("BasePlayer")) != std::string::npos) {			//if the entity is a player
			BasePlayer* Player = (BasePlayer*)ObjectClass;			//Create a player object for the player
			if (!LocalPlayer) {			//if local player has not been found
				DWORD64 LocalObjectClass = mem->Read<DWORD64>(Object + 0x30);
				if (LocalObjectClass <= 100000) continue;
				DWORD64 LocalObjectName = mem->Read<DWORD64>(LocalObjectClass + 0x60);

				std::string name = readCharString(LocalObjectName, 60);
				if (name.find(std::string("Local")) != std::string::npos) {//if local is contained in class name
					LocalPlayer = Player;
					LocalPos = LocalPlayer->GetPosition();
					LocalTeam = LocalPlayer->GetTeam();
					LocalLookingDegree = LocalPlayer->GetLookDegree();
					myRadar.createPlayerBlips(Player, Radar::blipType::localPlayer);
				}
			}//end local player
			else if (!Player->isSleeping()) {	//if not sleeping
				if (LocalTeam == Player->GetTeam() && LocalTeam != 0) continue;	//if the players are on your team dont show on radar		<---------- IS TEAMMATE
				if (mem->Read<bool>(ObjectClass + oWasDead)) continue;	//dont show on radar if player is dead
				//Players.push_back(Player);
				myRadar.createPlayerBlips(Player, Radar::blipType::enemy);
			}
		}//end BasePlayer if statement
	}//end entity forloop

	/*
	std::vector<Vector3> DrawRadarPosition;

	//RADAR
	std::vector<std::pair<int, int>> RadarDegrees; //angle, distance
	for (int i = 0; i < Players.size(); i++) {
		Vector3 Position = Players[i]->GetPosition();
		Vector3 DrawPosition;
		float xPos = Position[0] - LocalPos[0];
		float yPos = Position[2] - LocalPos[2];
		int distance = sqrt(pow(xPos, 2) + pow(yPos, 2));

		//float localLookRadians = LocalLookingDegree / 180 * 3.141592653;


		float rotX  = cos(LocalLookingDegree * 3.141592653 / 180)* xPos - sin(LocalLookingDegree * 3.141592653 / 180) * yPos;
		float rotY = cos(LocalLookingDegree * 3.141592653 / 180) * yPos + sin(LocalLookingDegree * 3.141592653 / 180) * xPos;
		if (distance < radarDistance)	DrawRadarPosition.push_back({ rotX / radarDistance, rotY/radarDistance, (float)RadarColors::Enemy});

		float Angle = 0;
		if ((xPos > 0 && yPos > 0) || (xPos < 0 && yPos < 0))	//if in quadrant 1 or 3
		{
			Angle = atan(std::abs(xPos) / std::abs(yPos)) * (180 / 3.141592653);
			if (xPos < 0) Angle += 180;
		}
		else {
			Angle = atan(std::abs(yPos) / std::abs(xPos)) * (180 / 3.141592653);
			if (xPos > 0) {
				Angle += 90;
			}
			else {
				Angle += 270;
			}
		}
		RadarDegrees.push_back(std::make_pair((int)Angle, distance));
	}

	bool Alert = false;
	for (int i = 0; i < RadarDegrees.size(); i++) {
		if (RadarDegrees[i].second <= radarDistance)	//if within radar
		{
			system("Color 40");
			std::cout << "ENEMY NEAR WITHIN " << radarDistance << "M: " << std::endl;
			Alert = true;
			break;
		}
	}

	if (Alert) {
		for (int i = 0; i < RadarDegrees.size(); i++) {
			if(RadarDegrees[i].second <= radarDistance)	//if within radar
			{
				std::cout << i + 1 << ":\t" << RadarDegrees[i].first << " degrees " << RadarDegrees[i].second << "M away\n" << std::endl;
			}
		}
	}
	else {
		system("Color 0F");
		std::cout << radarDistance << "M clear...	hold END to stop radar scanning." << std::endl;
	}

	drawWindow(window, DrawRadarPosition);
	*/
}


/*
		DWORD64 Entity = mem->Read<DWORD64>(EntityBuffer + 0x20 + (i * 0x8));
		if (Entity <= 100000) continue;
		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = mem->Read<DWORD64>(Object + 0x30);
		if (ObjectClass <= 100000) continue;
		DWORD64 ObjectName = mem->Read<DWORD64>(ObjectClass + 0x60);
		std::string name = readCharString(ObjectName, 60);
		std::cout << name << std::endl;
		*/
		/*
		if (name.find(std::string("Local")) != std::string::npos) {//if local is contained in class name
			//printf("\nName: %s\n", name.c_str());
			BasePlayer* Player = (BasePlayer*)mem->Read<DWORD64>(Object + 0x28);
			if (!mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + 0x50) || mem->Read<bool>(mem->Read<DWORD64>(Object + 0x28) + 0x18) || !mem->Read<DWORD64>(mem->Read<DWORD64>(Object + 0x28) + oPlayerModel))
			{
				printf("-------------ERROR\n");
				continue;
			}
			if (Player != LocalPlayer.BasePlayer) {
				//printf("LocalPlayer %lld\n", Player->GetSteamID());
				mfound = false;
			}
		}
		*/