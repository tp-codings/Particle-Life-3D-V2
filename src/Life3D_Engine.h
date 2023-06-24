#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader/shader.h>
#include <Camera/camera.h>
#include <ModelLoader/model.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Life3D_Particles.h"
#include "TextRenderer.h"
#include "ModelHandler.h"
#include "Simulation.h"

class Life3D_Engine
{
public:
	Life3D_Engine();
	void run();

private:

	Simulation *ParticleLife;

	//Window
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int windowPosX;
	int windowPosY;
	
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
	//void processInput(GLFWwindow* window, Shader reflectionShader);

	//Callbacks------------------------------------------------------------------------------

	void framebuffer_size_callback(int width, int height);
	static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height);

	void mouse_callback(double xpos, double ypos);
	static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);

	void scroll_callback(double xoffset, double yoffset);
	static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

	//Updates------------------------------------------------------------------------------

	void update();
	void updateDeltaTime();
	void updateCamera();

	//Rendering------------------------------------------------------------------------------

	void Draw();
};

