#include "Life3D_Engine.h"
#include <thread>
#include <random>

// Base Colors
#define RED glm::vec3(1.0f, 0.0f, 0.0f)
#define GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define BLUE glm::vec3(0.0f, 0.0f, 1.0f)
#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define BLACK glm::vec3(0.0f, 0.0f, 0.0f)

// Custom Colors
#define ORANGE glm::vec3(1.0f, 0.5f, 0.0f)
#define PURPLE glm::vec3(0.5f, 0.0f, 0.5f)
#define YELLOW glm::vec3(1.0f, 1.0f, 0.0f)
#define CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define MAGENTA glm::vec3(1.0f, 0.0f, 1.0f)


// Pastell Colors
#define PASTEL_PINK glm::vec3(0.86f, 0.63f, 0.69f)
#define PASTEL_YELLOW glm::vec3(0.95f, 0.95f, 0.58f)
#define PASTEL_BLUE glm::vec3(0.62f, 0.77f, 0.87f)
#define PASTEL_GREEN glm::vec3(0.64f, 0.87f, 0.68f)
#define PASTEL_PURPLE glm::vec3(0.73f, 0.62f, 0.81f)

// Metallic Colors
#define GOLD glm::vec3(1.0f, 0.84f, 0.0f)
#define SILVER glm::vec3(0.75f, 0.75f, 0.75f)
#define BRONZE glm::vec3(0.8f, 0.5f, 0.2f)
#define COPPER glm::vec3(0.85f, 0.55f, 0.4f)
#define STEEL glm::vec3(0.6f, 0.6f, 0.67f)

// Rainbow Colors
#define VIOLET glm::vec3(0.5f, 0.0f, 1.0f)
#define INDIGO glm::vec3(0.29f, 0.0f, 0.51f)
#define BLUE_GREEN glm::vec3(0.0f, 0.5f, 0.5f)
#define YELLOW_GREEN glm::vec3(0.6f, 0.8f, 0.2f)
#define YELLOW_ORANGE glm::vec3(1.0f, 0.71f, 0.0f)
#define RED_ORANGE glm::vec3(1.0f, 0.27f, 0.0f)

// Earth Tones
#define BROWN glm::vec3(0.65f, 0.16f, 0.16f)
#define SAND glm::vec3(0.76f, 0.7f, 0.5f)
#define OLIVE glm::vec3(0.5f, 0.5f, 0.0f)
#define MOSS_GREEN glm::vec3(0.55f, 0.64f, 0.45f)
#define SLATE_GRAY glm::vec3(0.44f, 0.5f, 0.56f)

// Neon Colors
#define NEON_PINK glm::vec3(1.0f, 0.07f, 0.55f)
#define NEON_YELLOW glm::vec3(1.0f, 0.95f, 0.0f)
#define NEON_GREEN glm::vec3(0.29f, 0.95f, 0.29f)
#define NEON_BLUE glm::vec3(0.29f, 0.59f, 0.95f)
#define NEON_PURPLE glm::vec3(0.67f, 0.29f, 0.95f)


//Constructor
Life3D_Engine::Life3D_Engine()
{
	//Inits
	this->initWindow();
	this->initVariables();
	this->initCamera();

	this->ParticleLife = new Simulation(this->window, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);

	stbi_set_flip_vertically_on_load(true);
}

//Main Loop
void Life3D_Engine::run()
{
	//Main loop (Exit on ESC or Cross)
	while (!glfwWindowShouldClose(this->window))
	{
		this->update();
		this->Draw();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	//ImGUI Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

//Inits------------------------------------------------------------------------------

int Life3D_Engine::initWindow()
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
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(this->window, mouse_callback_wrapper);


	return 0;
}

void Life3D_Engine::initCamera()
{
	//Create Camera object and place cursor reference to center of viewport
	this->camera = Camera(glm::vec3(0.0f, 2.2f, 5.0f));
	this->camera.setSpeed(this->cameraSpeed);
	this->lastMouseX = this->WINDOW_WIDTH / 2.0f;
	this->lastMouseY = this->WINDOW_HEIGHT / 2.0f;
	this->firstMouse = true;
}

void Life3D_Engine::initVariables()
{
	//Settings
	this->cameraSpeed = 600.0f;

	//Timing
	this->frameCount = 0;
	this->FPS = 0.0f;
	this->lastTime = (float)glfwGetTime();
}

//Inputhandling------------------------------------------------------------------------------

void Life3D_Engine::input()
{
	glfwSetCursorPosCallback(window, mouse_callback_wrapper);
	glfwSetScrollCallback(window, scroll_callback_wrapper);
}

//Callbacks------------------------------------------------------------------------------

void Life3D_Engine::framebuffer_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Life3D_Engine::framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height)
{
	Life3D_Engine* engine = static_cast<Life3D_Engine*>(glfwGetWindowUserPointer(window));
	engine->mouse_callback(width, height);
}

void Life3D_Engine::mouse_callback(double xpos, double ypos)
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

	if (this->ParticleLife->getViewMode())
	{
		this->camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void Life3D_Engine::mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos)
{
	Life3D_Engine* engine = static_cast<Life3D_Engine*>(glfwGetWindowUserPointer(window));
	engine->mouse_callback(xpos, ypos);
}

void Life3D_Engine::scroll_callback(double xoffset, double yoffset)
{
	this->camera.ProcessMouseScroll((float)yoffset);
}

void Life3D_Engine::scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset)
{
	Life3D_Engine* engine = static_cast<Life3D_Engine*>(glfwGetWindowUserPointer(window));
	engine->scroll_callback(xoffset, yoffset);
}

//Updates------------------------------------------------------------------------------

void Life3D_Engine::update()
{
	this->updateDeltaTime();
	this->input();
	this->updateCamera();

	this->ParticleLife->update(this->deltaTime, this->FPS, this->camera);
}

void Life3D_Engine::updateDeltaTime()
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

void Life3D_Engine::updateCamera()
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
	this->camera.setSpeed(this->ParticleLife->getCameraSpeed()); 
}

//Rendering------------------------------------------------------------------------------

void Life3D_Engine::Draw()
{
	this->ParticleLife->render();
}

//main
int main()
{
	srand(static_cast<unsigned>(time(0)));

	Life3D_Engine Life;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Life.run();

	return 0;
}