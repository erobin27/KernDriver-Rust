#pragma once
#include "GameSDK.h"

bool compareWeapon(std::wstring compareTo ,std::wstring currWeapon) {
	if (wcscmp(compareTo.c_str(), currWeapon.c_str()) == 0) return true;
	return false;
}

void callRecoil(std::string gunName, WeaponData* Weapon) {
	if (recoilMultiplier.find(gunName) == recoilMultiplier.end()) { 
		std::cout << skCrypt("ERROR: ") << gunName << skCrypt(" not found in config file.") << std::endl;
	}
	
	//set global variable for last gun recoil changed for
	recoilWeaponCheck = gunName;

	//if the default settings have not been entered into the dictionary then find them
	if (defaultRecoilSettingsAutomatic.find(gunName) == defaultRecoilSettingsAutomatic.end()) {
		//Read the default Recoil Values into defRecoil
		float* defRecoil = Weapon->ReadRecoil();//dArr[6];
		float* defAimCone = Weapon->ReadAimCone();//dArr[6];
		//std::cout << "Default MinYaw: " << defRecoil[0] << std::endl;
		//std::cout << "Default MaxYaw: " << defRecoil[1] << std::endl;
		//std::cout << "Default MinPitch: " << defRecoil[2] << std::endl;
		//std::cout << "Default MaxPitch: " << defRecoil[3] << std::endl;
		//std::cout << "Default ADS Scale: " << defRecoil[4] << std::endl;
		//std::cout << "Default Move Pen: " << defRecoil[5] << std::endl;

		//std::cout << "Default AimCone: " << defAimCone[0] << std::endl;
		//std::cout << "Default HipAimCone: " << defAimCone[1] << std::endl;

		//Add default recoil values into defaultRecoilSettingsAutomatic
		for (int i = 0; i < 6; i++) {
			defaultRecoilSettingsAutomatic[gunName].push_back(defRecoil[i]);
		}
		for (int i = 0; i < 2; i++) {
			defaultRecoilSettingsAutomatic[gunName].push_back(defAimCone[i]);
		}

		//clean up
		delete[] defRecoil;
		delete[] defAimCone;
	}

	if(activeMods.at("RecoilMultiplier")){
		//std::cout << ">>>>>>>>>>>>>>>>>>>RECOIL MULTIPLIER<<<<<<<<<<<<<<<<<<<<<<: " << recoilMultiplierAdjustable << std::endl;
		Weapon->NoRecoil(
			defaultRecoilSettings.at(gunName)[0] * recoilMultiplierAdjustable,
			defaultRecoilSettings.at(gunName)[1] * recoilMultiplierAdjustable,
			defaultRecoilSettings.at(gunName)[2] * recoilMultiplierAdjustable,
			defaultRecoilSettings.at(gunName)[3] * recoilMultiplierAdjustable,
			defaultRecoilSettings.at(gunName)[4] * recoilMultiplierAdjustable,
			defaultRecoilSettings.at(gunName)[5] * recoilMultiplierAdjustable
		);
		Weapon->editAimCone(defaultRecoilSettingsAutomatic.at(gunName)[6] * aimconeMultiplier, defaultRecoilSettingsAutomatic.at(gunName)[7] * aimconeMultiplier);
	} else {
		//std::cout << ">>>>>>>>>>>>>>>>>>>CUSTOM RECOIL SETTINGS<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		Weapon->NoRecoil(
			recoilSettings.at(gunName)[0],
			recoilSettings.at(gunName)[1],
			recoilSettings.at(gunName)[2],
			recoilSettings.at(gunName)[3],
			recoilSettings.at(gunName)[4],
			recoilSettings.at(gunName)[5]
		);
	}

	//Read the new values into a global variable
	{
		//Read the edited Recoil Values into defRecoil
		float* edRecoil = Weapon->ReadRecoil();
		float* edAimCone = Weapon->ReadAimCone();

		editedRecoilAutomatic[gunName].clear();
		//Add default recoil values into defaultRecoilSettingsAutomatic
		for (int i = 0; i < 6; i++) {
			editedRecoilAutomatic[gunName].push_back(edRecoil[i]);
		}
		for (int i = 0; i < 2; i++) {
			editedRecoilAutomatic[gunName].push_back(edAimCone[i]);
		}

		//clean up
		delete[] edRecoil;
		delete[] edAimCone;
	}
}

void WeaponPatch()
{
	//printf("+++++++++++++++++++++++++++WeaponPatch");
	std::wstring Hash = L"";
	WeaponData* Weapon = LocalPlayer.BasePlayer->GetActiveWeapon();

	if (Weapon) Hash = Weapon->GetNameHash();

	//std::wcout << "CURRENT WEAPON: " << Hash;
	HeldWeaponCheck = Hash;

	//printf("Hash: %S\nAK: %S",Hash.c_str(), L"rifle.ak");

	if (compareWeapon(L"rifle.ak", Hash)) {
		callRecoil("rifle.ak", Weapon);
	}

	if (compareWeapon(L"rifle.lr300", Hash))
		callRecoil("rifle.lr300", Weapon);

	if (compareWeapon(L"rifle.semiauto", Hash))
		callRecoil("rifle.semiauto", Weapon);
	
	if (compareWeapon(L"lmg.m249", Hash))
		callRecoil("lmg.m249", Weapon);

	if (compareWeapon(L"smg.mp5", Hash))
		callRecoil("smg.mp5", Weapon);

	if (compareWeapon(L"smg.thompson", Hash))
		callRecoil("smg.thompson", Weapon);

	if (compareWeapon(L"smg.2", Hash)) //custom smg
		callRecoil("smg.2", Weapon);

	if (compareWeapon(L"pistol.python", Hash))
		callRecoil("pistol.python", Weapon);

	if (compareWeapon(L"pistol.m92", Hash))
		callRecoil("pistol.m92", Weapon);

	if (compareWeapon(L"pistol.semiauto", Hash))
		callRecoil("pistol.semiauto", Weapon);
	
	if (compareWeapon(L"rifle.m39", Hash))
		callRecoil("rifle.m39", Weapon);

	if (compareWeapon(L"pistol.revolver", Hash))
		callRecoil("pistol.revolver", Weapon);
	

	/*
	switch (Hash) {
		//nullptr
	case 0: return;

		//Weapon
	case 0x5A9F:
	case 0x6B1:
	case 0x74F5:
	case 0x67DA:
	case 0x3146:
	case 0x79C4:
	case 0x573C:
	case 0xB32A:
	case 0xC196:
	case 0x7983:
	case 0xE5EA:
	case 0x77E9:
	case 0xE97D:
	case 0xC2DD:
	case 0x52F7:
	case 0xFDC3:
	case 0x62E5:
	case 0x6A09:
	case 0x3511:
	case 0xCFA1:
	case 0xF87E:
		Weapon->NoRecoil();

	case 0xB0A0:
	case 0x435E:
	{

		if (Hash == 0x435E)
		{
			//Weapon->FatBullet();
			//Weapon->SuperEoka();
			//Weapon->Compund();
		}
	} break;
	case 0xF3B9:
	case 0xCBD8:
	case 0xAB85:
	case 0xFB2B:
	case 0x6104:
	case 0xC859:
	case 0x96D3:
	case 0x7340:
	case 0xC68B:
	case 0x2F12:
	case 0x65B4:
	case 0x1F2D:
	case 0x2658:
	case 0xAC02:
	case 0x282A:
	case 0x8151:
	case 0xFACE:
	case 0x7296:
	case 0x4B0A:
	case 0xCD:
	case 0x2333:

	default: break;
	}
	*/
}
