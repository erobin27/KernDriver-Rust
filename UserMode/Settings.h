#pragma once
#include <map>
#include <iostream>

extern std::map<std::string, std::vector<float>> recoilSettings;
extern std::map<std::string, std::vector<float>> defaultRecoilSettings;
extern std::map<std::string, std::vector<float>> recoilMultiplier;

extern float recoilMultiplierAdjustable;
extern std::map<std::string, std::vector<float>> defaultRecoilSettingsAutomatic;

extern std::map<std::string, bool> activeMods;