#include "RenderGraphics.h"
#include <stdio.h>

GLFWwindow* CreateGLWindow() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Sonar", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	return window;
}

void closeWindow() {
	glfwTerminate();
}

void drawWindow(GLFWwindow* window, std::vector<Vector3> PointsToRender) {
	if(glfwWindowShouldClose(window)) glfwTerminate();

	//while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClearColor(1.0, 1.0, 1.0, 1.0); //background color
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_LINES);
		glVertex2f(-1.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(0.0f, -1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();

		glPointSize(10);
		for (int i = 0; i < PointsToRender.size(); i++) {
			glBegin(GL_POINTS);
			glVertex2f(PointsToRender[i].x, PointsToRender[i].y);
			glEnd();
		}


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}

int DrawRadar() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Sonar", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}