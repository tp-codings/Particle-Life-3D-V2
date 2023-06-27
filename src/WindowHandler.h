#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Life3D_Particles.h"
#include "TextRenderer.h"
#include "ModelHandler.h"
#include "Simulation.h"

class WindowHandler
{
public:
	WindowHandler();

	void update(bool mode, float speed);

	float getDeltaTime();
	GLFWwindow* getWindow();
	glm::vec2 getWindowSize();
	Camera getCamera();
	int getFPS();

private:

	//Window
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int windowPosX;
	int windowPosY;
	
	bool viewMode;

	GLFWwindow* window;

	//Timing
	float deltaTime;
	float lastFrame;
	float lastTime;
	float currentFrame;
	float FPS;

	int frameCount;

	//Camera
	Camera camera;

	float lastMouseX;
	float lastMouseY;
	bool firstMouse;

	float cameraSpeed;

	//Inits------------------------------------------------------------------------------

	int initWindow();

	void initCamera();
	void initVariables();

	//Input------------------------------------------------------------------------------

	void input();

	//Callbacks------------------------------------------------------------------------------

	void framebuffer_size_callback(int width, int height);
	static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height);

	void mouse_callback(double xpos, double ypos);
	static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);

	void scroll_callback(double xoffset, double yoffset);
	static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

	//Updates------------------------------------------------------------------------------
	
	void updateDeltaTime();
	void updateCamera();

};

