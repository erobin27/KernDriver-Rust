#pragma once
#include <map>
#include <iostream>
#include <vector>

extern std::map<std::string, std::vector<float>> recoilSettings;
extern std::map<std::string, std::vector<float>> defaultRecoilSettings;
extern std::map<std::string, std::vector<float>> recoilMultiplier;

extern float recoilMultiplierAdjustable;
extern float aimconeMultiplier;
extern int radarDistance;
extern std::map<std::string, std::vector<float>> defaultRecoilSettingsAutomatic;
extern std::map<std::string, std::vector<float>> editedRecoilAutomatic;
extern std::wstring HeldWeaponCheck;
extern std::string recoilWeaponCheck;
extern std::map<std::string, bool> activeMods;