#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "myMath.h"

int DrawRadar();
void closeWindow();
void drawWindow(GLFWwindow* window, std::vector<Vector3> NearbyPlayersInfo);
GLFWwindow* CreateGLWindow();