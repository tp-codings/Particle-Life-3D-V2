#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera/camera.h>
#include <stb_image/stb_image.h>

class WindowHandler
{
public:
	WindowHandler(){
	//Inits
		this->initWindow();
		this->initVariables();
		this->initCamera();
	};

	void update(bool mode, float speed){
		this->cameraSpeed = speed;
		this->viewMode = mode;
		this->updateDeltaTime();
		this->input();
		this->updateCamera();
	};

	float getDeltaTime(){
		return this->deltaTime;
	};
	GLFWwindow* getWindow(){
		return this->window;
	};
	glm::vec2 getWindowSize(){
		return glm::vec2(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	};
	Camera getCamera(){
		return this->camera;
	};
	int getFPS(){
		return this->FPS;
	};

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

	float cameraSpeed;
	float lastMouseX;
	float lastMouseY;

	bool firstMouse;

	//Inits------------------------------------------------------------------------------

	int initWindow(){
		//GLFW Setup
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Create window and context
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		this->WINDOW_WIDTH = mode->width;
		this->WINDOW_HEIGHT = mode->height;
		this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, "Life3D", monitor, NULL);
		glfwMakeContextCurrent(this->window);

		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Error query
		if (this->window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}

		//Load GlAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		//define OpenGL Viewport
		glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);

		//Rezising should affect Viewport


		glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback_wrapper);
		glfwSetWindowUserPointer(this->window, this);
		glfwSetCursorPosCallback(this->window, mouse_callback_wrapper);


		return 0;
	};

	void initCamera(){
		//Create Camera object and place cursor reference to center of viewport
		this->camera = Camera(glm::vec3(0.0f, 2.2f, 5.0f));
		this->camera.setSpeed(this->cameraSpeed);
		this->lastMouseX = this->WINDOW_WIDTH / 2.0f;
		this->lastMouseY = this->WINDOW_HEIGHT / 2.0f;
		this->firstMouse = true;
	};
	void initVariables(){
		//Settings
		this->cameraSpeed = 6.0f;
		this->viewMode = true;

		//Timing
		this->frameCount = 0;
		this->FPS = 0.0f;
		this->lastTime = (float)glfwGetTime();
	};

	//Input------------------------------------------------------------------------------

	void input(){
		glfwSetCursorPosCallback(window, mouse_callback_wrapper);
		glfwSetScrollCallback(window, scroll_callback_wrapper);
	};

	//Callbacks------------------------------------------------------------------------------

	void framebuffer_size_callback(int width, int height){
		glViewport(0, 0, width, height);
	};
	static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height){
		WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
		engine->mouse_callback(width, height);
	};

	void mouse_callback(double xpos, double ypos){
		if (this->firstMouse)
		{
			this->lastMouseX = (float)xpos;
			this->lastMouseY = (float)ypos;
			this->firstMouse = false;
		}

		float xoffset = (float)xpos - this->lastMouseX;
		float yoffset = this->lastMouseY - (float)ypos;

		this->lastMouseX = (float)xpos;
		this->lastMouseY = (float)ypos;

		if (this->viewMode)
		{
			this->camera.ProcessMouseMovement(xoffset, yoffset);
		}
	};
	static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos){
		WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
		engine->mouse_callback(xpos, ypos);
	};

	void scroll_callback(double xoffset, double yoffset){
		this->camera.ProcessMouseScroll((float)yoffset);
	};
	static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset){
		WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
		engine->scroll_callback(xoffset, yoffset);
	};

	//Updates------------------------------------------------------------------------------
	
	void updateDeltaTime(){
		//Calculates time between frames
		this->frameCount++;
		this->currentFrame = (float)glfwGetTime();
		this->deltaTime = this->currentFrame - this->lastFrame;
		this->lastFrame = this->currentFrame;

		//Calculates FPS
		float delta = this->currentFrame - this->lastTime;
		if (delta >= 1.0)
		{
			this->FPS = static_cast<float>(frameCount) / delta;
			frameCount = 0;
			lastTime = currentFrame;
		}
	};
	void updateCamera(){
		//Keyboard input for camera movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
		this->camera.setSpeed(this->cameraSpeed); 
	};

};

