#pragma once
#include "UM-Memory.h"
#include <iostream>
#include <map>
#include <list>
#include <algorithm> 
#include <stdlib.h>
#include <fstream>
#include "settings.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
/*
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
		std::wcout << mem->Read<UCHAR>(address + i * 2);
		str += mem->Read<wchar_t>(address + i * 2);
	}

	return str;
}
*/


//Colors
#define BLUE  "Color 09"
#define BLACK  "Color 00"
#define GREEN  "Color 0A"
#define AQUA  "Color 0B"
#define RED  "Color 0C"
#define PURPLE  "Color 0D"
#define YELLOW  "Color 0E"
#define WHITE  "Color 0F"
#define CLEAR "cls"

//printf("%sred\n", KRED);
//printf("%sgreen\n", KGRN);
//printf("%syellow\n", KYEL);
//printf("%sblue\n", KBLU);
//printf("%smagenta\n", KMAG);
//printf("%scyan\n", KCYN);
//printf("%swhite\n", KWHT);
//printf("%snormal\n", KNRM);

std::map<std::string, std::vector<float>> recoilSettings;
std::map<std::string, std::vector<float>> defaultRecoilSettings;
std::map<std::string, std::vector<float>> recoilMultiplier;
float recoilMultiplierAdjustable = 1.0f;
std::map<std::string, bool> activeMods;

std::string left(string line, char delimeter) {
	string str = line.substr(0, line.find(delimeter));
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

std::string right(string line, char delimeter) {
	string str = line.substr(line.find(delimeter) + 1, line.length());
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

std::vector<float> readConfigLine(string values) {
	char* pend;
	vector <float> vals;
	stringstream check1(values);
	string intermediate;

	while (getline(check1, intermediate, ',')) //tokenize string with ',' as delimeter
	{
		vals.push_back(strtof(intermediate.c_str(), &pend));
	}
	return vals;
}

void printConfigTable(string left, vector<float>right) {
	cout << left << " : ";
	for (int i = 0; i < right.size(); i++) {
		cout << right[i] << " ";
	}
	cout << "\n";
}

void readSection(string sectionTitle, string myText, bool &READ, bool print) {
	if (myText.find(sectionTitle) != string::npos) {
		//cout << "TRUE: " << myText << "\n";
		READ = true;
		return;
	}
	if (myText.find(")") != string::npos) {
		//cout << "FALSE: " << myText << "\n";
		READ = false;
	}

	if (READ && sectionTitle.compare("Recoil(") == 0) {
		//PRINT CONFIG TABLE
		printConfigTable(left(myText, ':'), readConfigLine(right(myText, ':')));
		
		//FILL RECOIL MAP
		recoilSettings[left(myText, ':')] = readConfigLine(right(myText, ':'));
	}

	if (READ && sectionTitle.compare("RecoilMultiplier(") == 0) {
		//PRINT CONFIG TABLE
		printConfigTable(left(myText, ':'), readConfigLine(right(myText, ':')));

		//FILL RECOIL MAP
		recoilMultiplier[left(myText, ':')] = readConfigLine(right(myText, ':'));
	}

	if (READ && sectionTitle.compare("DefaultRecoil(") == 0) {
		//PRINT CONFIG TABLE
		printConfigTable(left(myText, ':'), readConfigLine(right(myText, ':')));

		//FILL RECOIL MAP
		defaultRecoilSettings[left(myText, ':')] = readConfigLine(right(myText, ':'));
	}

	if (READ && sectionTitle.compare("ActiveMods(") == 0) {
		//PRINT CONFIG TABLE
		printConfigTable(left(myText, ':'), readConfigLine(right(myText, ':')));

		//FILL ACTIVE MODS MAP
		activeMods[left(myText, ':')] = (0 < (int)readConfigLine(right(myText, ':'))[0]);
	}
}

bool getSettings() {

	//readLines
	string myText;
	bool READ = false;
	bool print = false;
	string currSection;
	
	//FIND THE CURRENT PATH TO THE RUNNING EXE THEN FIND "config.txt" IN THAT SAME DIRECTORY
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string directory(buffer);
	directory.erase(directory.find_last_of('\\') + 1, directory.length());
	directory += "config.txt";
	cout << "Current Directory:" << directory << endl;

	ifstream in(directory);
	if (!in) {	//return error
		cout << "ERROR: Failed to find config.txt" << endl;
		return false;
	}
	while (getline(in, myText)) { //read the lines
		if (myText.compare("Recoil(") == 0) {
			cout << "-----RECOIL SETTINGS-----" << endl;
			currSection = "Recoil(";
			print = false;
		}
		if (myText.compare("DefaultRecoil(") == 0) {
			cout << "-----DEFAULT RECOIL SETTINGS-----" << endl;
			currSection = "DefaultRecoil(";
			print = false;
		}
		if (myText.compare("RecoilMultiplier(") == 0) {
			cout << "-----MULTIPLIER SETTINGS-----" << endl;
			currSection = "RecoilMultiplier(";
			print = true;
		}
		if (myText.compare("ActiveMods(") == 0) {
			cout << "-----ACTIVE MODS-----" << endl;
			currSection = "ActiveMods(";
			print = false;
		}

		readSection(currSection, myText ,READ, print);

	}
	in.close();
	return true;

	//seperate gunName and vals
	//transform vals to float
	//put into map for that gun
}

void updateSettings() {
	if (!getSettings()){
		cout << "ERROR: cannot find config.txt" << endl;
	}
}

void initializeSettings() {
	//initialize all mods to false
	activeMods["Recoil"] = false;

	//call update settings to read config
	updateSettings();
}