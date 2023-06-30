#include "WindowHandler.h"

//Constructor
WindowHandler::WindowHandler()
{
	//Inits
	this->initWindow();
	this->initVariables();
	this->initCamera();

	//stbi_set_flip_vertically_on_load(true);
}


float WindowHandler::getDeltaTime()
{
	return this->deltaTime;
}

GLFWwindow* WindowHandler::getWindow()
{
	return this->window;
}

glm::vec2 WindowHandler::getWindowSize()
{
	return glm::vec2(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
}

Camera WindowHandler::getCamera()
{
	return this->camera;
}

int WindowHandler::getFPS()
{
	return this->FPS;
}

//Inits------------------------------------------------------------------------------

int WindowHandler::initWindow()
{
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
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	//Load GlAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//define OpenGL Viewport
	glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);

	//Rezising should affect Viewport


	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback_wrapper);
	glfwSetWindowUserPointer(this->window, this);
	glfwSetCursorPosCallback(this->window, mouse_callback_wrapper);


	return 0;
}

void WindowHandler::initCamera()
{
	//Create Camera object and place cursor reference to center of viewport
	this->camera = Camera(glm::vec3(0.0f, 2.2f, 5.0f));
	this->camera.setSpeed(this->cameraSpeed);
	this->lastMouseX = this->WINDOW_WIDTH / 2.0f;
	this->lastMouseY = this->WINDOW_HEIGHT / 2.0f;
	this->firstMouse = true;
}

void WindowHandler::initVariables()
{
	//Settings
	this->cameraSpeed = 600.0f;
	this->viewMode = true;

	//Timing
	this->frameCount = 0;
	this->FPS = 0.0f;
	this->lastTime = (float)glfwGetTime();
}

//Inputhandling------------------------------------------------------------------------------

void WindowHandler::input()
{
	glfwSetCursorPosCallback(window, mouse_callback_wrapper);
	glfwSetScrollCallback(window, scroll_callback_wrapper);
}

//Callbacks------------------------------------------------------------------------------

void WindowHandler::framebuffer_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}

void WindowHandler::framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height)
{
	WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	engine->mouse_callback(width, height);
}

void WindowHandler::mouse_callback(double xpos, double ypos)
{
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
}

void WindowHandler::mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos)
{
	WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	engine->mouse_callback(xpos, ypos);
}

void WindowHandler::scroll_callback(double xoffset, double yoffset)
{
	this->camera.ProcessMouseScroll((float)yoffset);
}

void WindowHandler::scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset)
{
	WindowHandler* engine = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	engine->scroll_callback(xoffset, yoffset);
}

//Updates------------------------------------------------------------------------------

void WindowHandler::update(bool mode, float speed)
{
	this->cameraSpeed = speed;
	this->viewMode = mode;
	this->updateDeltaTime();
	this->input();
	this->updateCamera();
}

void WindowHandler::updateDeltaTime()
{
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
}

void WindowHandler::updateCamera()
{
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
}

