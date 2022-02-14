#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"
#include "GameSDK.h"

// FreeType Headers
//#include <ft2build.h>
//#include FT_FREETYPE_H 

#include <vector>

#include <string>
#include "myMath.h"

int DrawRadar();
void closeWindow();
void drawWindow(GLFWwindow* window, std::vector<Vector3> NearbyPlayersInfo);
GLFWwindow* CreateGLWindow(int windowX, int windowY);
GLTtext* CreateGLText();
void initWindow(GLFWwindow* window);

class RadarMenu {
public:
	int x;
	int y;
	std::map<std::string, bool> menuItems;
	std::vector<std::string> keys;
	std::vector<int> DrawListVec;
	int selectedItem;

	RadarMenu() {
	}

	RadarMenu(int x, int y) {
		this->x = x;
		this->y = y;
		initMenuOptions();
		this->selectedItem = 0;
	}

	void nextItem() {
		if (this->selectedItem + 1 >= menuItems.size()) {
			this->selectedItem = 0;
		}
		else {
			this->selectedItem++;
		}
	}

	void previousItem() {
		if (this->selectedItem - 1 < 0) {
			this->selectedItem = menuItems.size()-1;
		}
		else {
			this->selectedItem--;
		}
	}

	void changeItem() {
		menuItems[keys[this->selectedItem]] = !menuItems[keys[this->selectedItem]];
	}


	//map of bools
	void initMenuOptions();
};

class Blip {
public:
	std::string name;
	float x;
	float y;
	float z;
	float size;
	std::string color;
	float health;
	float lookDirection;

	Blip() {

	}

	Blip(std::string name, float x, float y, float z, std::string color, float size, float health = 0, float lookDirection = 0) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->z = z;
		this->size = size;
		this->color = color;
		this->health = health;
		this->lookDirection = lookDirection;
	}
};

class Radar {
	GLFWwindow* window;
	GLTtext* text;
	int windowX;
	int windowY;
	Blip centerBlip;
	//RadarMenu menu;
	std::vector<Blip> blipList;
	//vector<int> RenderList;

public:
	RadarMenu menu;
	int range;
	Radar(int sizeX, int sizeY) {
		this->menu = RadarMenu(sizeX, sizeY / 3);
		this->windowX = sizeX;
		this->windowY = sizeY;
		this->window = CreateGLWindow(sizeX, sizeY + this->menu.y);
		this->range = 200;
	}

	enum blipType {
		localPlayer = 0,
		teammate = 1,
		enemy = 2,
		ai = 3,
		sleeper = 4
	};

	//menu funcs
	void renderMenu();

	//GL functions
	GLFWwindow* getWindow();
	void closeWindow();

	//Math functions
	float RadarOrMenu(float y, bool onMenu);
	void setColor(std::string color);
	void setRange(int range);

	//Drawing Functions
	void drawFilledCircle(GLfloat x, GLfloat y, float size, std::string color, bool onMenu = false);
	void drawHollowCircle(GLfloat x, GLfloat y, float size, std::string color, bool onMenu = false);
	void drawLineByAngle(GLfloat x, GLfloat y, float angle, float size, std::string color);
	void drawText(GLfloat x, GLfloat y, float size, std::string type, bool onMenu = false);
	void drawColoredText(GLfloat x, GLfloat y, float size, std::string type, std::string color, bool onMenu = false);
	void drawRect(GLfloat x, GLfloat y, GLfloat length, GLfloat height, std::string color, float percent = 1.0, std::string alignment = "LEFT", bool onMenu = false);
	void drawHealthBar(GLfloat x, GLfloat y, float size, float percent, float yOffset = 10);
	void drawTriangle(GLfloat x, GLfloat y, float size, std::string color, bool down = false, bool onMenu = false);


	//Blip Functions
	bool createPlayerBlips(BasePlayer* player, int type);
	void createLootBlips(EntityClass loot);
	void renderBlip(Blip blip, bool rotate = true);
	bool renderBlipName(Blip blip, bool rotate = true);
	void clearBlips();

	//Window Display Functions
	void drawBlank();
	void drawWindowTesting();
	void drawSonar();

};