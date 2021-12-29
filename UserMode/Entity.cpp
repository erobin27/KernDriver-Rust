#include "UM-Memory.h"
#include "GameSDK.h"
#include "AimFunc.h"
#include "GStructs.h"
#include "VMProtectSDK.h"

bool mfound = false;

std::string readCharString(DWORD64 address, int length) {
	std::string str = "";
	for (int i = 0; i < length; i++) {
		str += mem->Read<char>(address + i);
	}

	return str;
}

void entityLoop() {
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