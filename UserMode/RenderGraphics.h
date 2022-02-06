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
	int range;
	Blip centerBlip;
	std::vector<Blip> blipList;
	//vector<int> RenderList;

public:
	Radar(int sizeX, int sizeY) {
		this->windowX = sizeX;
		this->windowY = sizeY;
		this->window = CreateGLWindow(sizeX, sizeY);
		this->range = 200;
	}

	enum blipType {
		localPlayer = 0,
		teammate = 1,
		enemy = 2,
		ai = 3
	};

	GLFWwindow* getWindow();
	void setRange(int range);
	void setColor(std::string color);
	void drawFilledCircle(GLfloat x, GLfloat y, float size, std::string color);
	void drawHollowCircle(GLfloat x, GLfloat y, float size, std::string color);
	void drawLineByAngle(GLfloat x, GLfloat y, float angle, float size, std::string color);
	void drawText(GLfloat x, GLfloat y, float size, std::string type);
	void clearBlips();
	void renderBlip(Blip blip, bool rotate = true);
	bool renderBlipName(Blip blip, bool rotate = true);
	void drawBlank();
	bool createPlayerBlips(BasePlayer* player, int type);
	void createLootBlips(LootContainer::container loot);
	void drawRect(GLfloat x, GLfloat y, GLfloat length, GLfloat height, std::string color, float percent = 1.0, std::string alignment = "LEFT");
	void drawHealthBar(GLfloat x, GLfloat y, float size, float percent, float yOffset = 10);
	void drawTriangle(GLfloat x, GLfloat y, float size, std::string color, bool down = false);
	void drawWindowTesting();
	void closeWindow();
};