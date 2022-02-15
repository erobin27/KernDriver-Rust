#include "Entity.h"
#include "GameSDK.h"
#include "AimFunc.h"
#include <codecvt>
#include <array>

bool mfound = false;
LPlayerBase LocalPlayer;

//BoxType to String
/*
std::string boxTypeToString(int type) {
	switch (type) {
	case 0:
		return "hiddenhackablecrate";
	case 1:
		return "crate_basic";
	case 2:
		return "crate_elite";
	case 3:
		return "crate_mine";
	case 4:
		return "crate_normal";
	case 5:
		return "crate_normal_2";
	case 6:
		return "crate_normal_2_food";
	case 7:
		return "crate_normal_2_medical";
	case 8:
		return "crate_tools";
	case 9:
		return "crate_underwater_advanced";
	case 10:
		return "crate_underwater_basic";
	case 11:
		return "crate_ammunition";
	case 12:
		return "crate_food_1";
	case 13:
		return "crate_food_2";
	case 14:
		return "crate_fuel";
	case 15:
		return "crate_medical";
	case 16:
		return "codelockedhackablecrate";
	case 17:
		return "codelockedhackablecrate_oilrig";
	case 18:
		return "bradley_crate";
	case 19:
		return "heli_crate";
	case 20:
		return "cratemarker";
	case 21:
		return "survey_crater";
	case 22:
		return "survey_crater_oil";
	default:
		return "unknown";
	}
};
*/  


enum RadarColors {
	Enemy = 1,
	ToolCupboard = 2,
	GroundItem = 3,
	Sleeper = 4,
};

bool withinPerimeterRange(Vector3 localPos, Vector3 entityPos, float range, Radar &myRadar) {
	float x = entityPos.x - localPos.x;
	float y = entityPos.z - localPos.z;

	if (!myRadar.menu.menuItems["show tunnels"]) {	//DO NOT SHOW PEOPLE IN THE TUNNELS
		//if below ground dont show above
		if (localPos.y < -45.0 && entityPos.y > -3) return false;	//if player is in the tunnels dont show things above ground
		
		//if above dont show below
		if (localPos.y > -10.0 && entityPos.y < -45) return false;
	}
	
	if (fabs(x) < range && fabs(y) < range) return true;

	return false;
}

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

std::string readStringFromMem(uint64_t stringAddr) {
	std::array<char, 64> name_buffer;
	name_buffer = mem->Read<std::array<char, 64>>(stringAddr);
	name_buffer[63] = '\0';
	return name_buffer.data();
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

void radarLoop(Radar &myRadar, GameData &gData, bool RefreshEntities) {
	myRadar.clearBlips();

	//if we did not check for new entities then update old ones by checking if someone has destroyed them
	if (!RefreshEntities) {
		for (std::vector<EntityClass>::iterator i = gData.Entities.begin(); i != gData.Entities.end();)
		{
			if (i->isActive()) {		//If the entity has not been mined/picked
				if (myRadar.menu.menuItems[i->name] && withinPerimeterRange(gData.LocalPos, i->position, myRadar.range, myRadar)) {
					myRadar.createLootBlips(*i);
				}
				i++;
			}
			else
				i = gData.Entities.erase(i);		//if it has been mined remove it
		}
	} 
	else {
		gData.clear();
	}



	/*
	* 
	*	BEGIN GAME FUNCTIONS THINGS
	* 
	*/

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
	BaseNetworkable = mem->Read<DWORD64>(base_address + g_BN_Steam);
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
		std::string ClassName = readCharString(ObjName, 15);

		DWORD64 Object = mem->Read<DWORD64>(Entity + 0x10);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = mem->Read<DWORD64>(Object + 0x28);
		//std::cout << ClassName << std::endl;

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

					gData.LocalPos = LocalPos;
					//add if in range
					myRadar.createPlayerBlips(Player, Radar::blipType::localPlayer);
				}
			}//end local player
			else if (!Player->isSleeping() && withinPerimeterRange(LocalPos, Player->GetPosition(), myRadar.range, myRadar)) {														//if not sleeping
				if (mem->Read<bool>(ObjectClass + oWasDead)) continue;	//dont show on radar if player is dead
				if (LocalTeam == Player->GetTeam() && LocalTeam != 0){
					if (myRadar.menu.menuItems["show team"]) {										//if we choose to render teammates then render
						if (mem->Read<bool>(ObjectClass + oWasDead)) continue;
						myRadar.createPlayerBlips(Player, Radar::blipType::teammate);
						continue;
					}
					else {
						continue;
					}
				}	//if the players are on your team dont show on radar		<---------- IS TEAMMATE
				//Players.push_back(Player);
				myRadar.createPlayerBlips(Player, Radar::blipType::enemy);
			}
			else if (Player->isSleeping() & myRadar.menu.menuItems["show sleepers"] & withinPerimeterRange(LocalPos, Player->GetPosition(), myRadar.range, myRadar)) {
				myRadar.createPlayerBlips(Player, Radar::blipType::sleeper);
			}
		}//end BasePlayer if statement
		else if (RefreshEntities) {
			if (ClassName.find(std::string("LootContainer")) != std::string::npos) {			//if the entity is a player
				//Get object name
				DWORD64 LocalObjectClass = mem->Read<DWORD64>(Object + 0x30);
				if (LocalObjectClass <= 100000) return;
				DWORD64 LocalObjectName = mem->Read<DWORD64>(LocalObjectClass + 0x60);
				std::string name = readStringFromMem(LocalObjectName);
				name = name.substr(name.find_last_of("/") + 1);
				name = name.substr(0, name.find("."));

				EntityClass box(Entity, ObjectClass, name, EntityClass::EntityTypes::Lootbox);

				gData.Entities.emplace_back(box);
				if (myRadar.menu.menuItems[box.name] && withinPerimeterRange(LocalPos, box.position, myRadar.range, myRadar))	myRadar.createLootBlips(box);

			}
			else if (ClassName.find(std::string("Ore")) != std::string::npos) {			//if the entity is a player

				//Get object name
				DWORD64 LocalObjectClass = mem->Read<DWORD64>(Object + 0x30);
				if (LocalObjectClass <= 100000) return;
				DWORD64 LocalObjectName = mem->Read<DWORD64>(LocalObjectClass + 0x60);
				std::string name = readStringFromMem(LocalObjectName);
				name = name.substr(name.find_last_of("/") + 1);
				name = name.substr(0, name.find("."));

				EntityClass ore(Entity, ObjectClass, name, EntityClass::EntityTypes::Ore);



				gData.Entities.emplace_back(ore);
				if (myRadar.menu.menuItems[ore.name] && withinPerimeterRange(LocalPos, ore.position, myRadar.range, myRadar)) {
					myRadar.createLootBlips(ore);
				}
				//if(lootBox.type > 0)	myRadar.createLootBlips(lootBox);
			}
		}	 //Get static entities in loop
	}//end entity forloop


	myRadar.swapBlipBuffers();
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