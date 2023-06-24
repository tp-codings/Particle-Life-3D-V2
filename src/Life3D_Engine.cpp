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

	this->initShader();
	this->initVertices();
	this->initVariables();
	this->initCamera();

	//Initialize Models
	this->sphere = new Model(".\\resources\\models\\sphere\\sphere.obj");

	this->borderBox = new ModelHandler(".\\resources\\models\\cube\\cube.obj");
	this->sun = new ModelHandler(".\\resources\\models\\sphere\\sphere.obj");

	//Create Particles
	this->red = this->create(amount, RED);
	this->green = this->create(amount, GREEN);
	this->blue = this->create(amount, BLUE);
	this->yellow = this->create(amount, YELLOW);
	this->white = this->create(amount, WHITE);

	//Init Buffer after all
	this->initBuffer();

	//Randomize first attraction matrix
	this->randomAttraction();

	stbi_set_flip_vertically_on_load(true);

	//ImGUI Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
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

void Life3D_Engine::initVertices()
{
	//Screen quad
	float qVertices[] = {
		// positions // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	this->quadVertices = new float[6 * 4];

	std::memcpy(this->quadVertices, qVertices, sizeof(qVertices));
}

void Life3D_Engine::initBuffer()
{
	//Screen Buffer
	glGenVertexArrays(1, &this->screenVAO);
	glGenBuffers(1, &this->screenVBO);
	glBindVertexArray(this->screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->screenVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, this->quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Screen Framebuffer
	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	//create a color attachment texture
	glGenTextures(1, &this->texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texColorBuffer, 0);

	//create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &this->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo); 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Instanced Rendering Buffer
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * this->amount * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

	for (unsigned int i = 0; i < this->sphere->meshes.size(); i++)
	{
		unsigned int VAO = this->sphere->meshes[i].VAO;
		glBindVertexArray(VAO);
		//Every coloumn of the instance matrix gets its own Attribute Pointer
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	//Color Buffer
	glGenBuffers(1, &this->colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
	glBufferData(GL_ARRAY_BUFFER, this->colorData.size() * sizeof(glm::vec3), &this->colorData[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < this->sphere->meshes.size(); i++)
	{
		unsigned int VAO = this->sphere->meshes[i].VAO;
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(7); 
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(7, 1);

		glBindVertexArray(0);
	}
}

void Life3D_Engine::initShader()
{
	//Create Shader objects for several shader units
	this->screenShader = Shader("Shader/screen.vs", "Shader/screen.fs");
	this->particleShader = Shader("Shader/particles.vs", "Shader/particles.fs");
	this->cubeShader = Shader("Shader/cube.vs", "Shader/cube.fs");
	this->sunShader = Shader("Shader/sun.vs", "Shader/sun.fs");
	this->textShader = Shader("Shader/text.vs", "Shader/text.fs");
}

void Life3D_Engine::initVariables()
{
	//Settings
	this->amount = 2000;
	this->timeFactor = 0.7f;
	this->postProcessingChoice = 7;
	this->shaderChoice = 1;
	this->distanceMax = 150.0f;
	this->scale = 0.5f;
	this->cubeSize = 250.0f;
	this->cameraSpeed = 600.0f;

	//Textrendering
	this->textRenderer = new TextRenderer(10, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->fontSize = 10;

	//Friction @Tom Mohr
	this->TIME_STEP = 0.2f;
	this->frictionHalfLife = 0.04f;
	this->friction = pow(0.5, this->TIME_STEP / this->frictionHalfLife);

	//Settingbooleans
	this->start = false;
	this->showBorder = false;
	this->borders = true;
	this->viewMode = true;

	//Timing
	this->frameCount = 0;
	this->FPS = 0.0f;
	this->lastTime = (float)glfwGetTime();

	//Directional Light Shading 
	this->dirLightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->dirLightDirection = glm::vec3(1.0, 1.0, 1.0);
	this->angleHor = 0.0f;
	this->angleVer = 1.0f;
	this->dirLightPos = glm::vec3(this->dirLightDirection * this->cubeSize*4.f);

	//Projection matrices
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);

	//Key pressed booleans
	this->startKeyPressed = false;
	this->settingsKeyPressed = false;
	this->randomKeyPressed = false;
	this->randPosKeyPressed = false;
	this->borderKeyPressed = false;
	this->shadingTypeKeyPressed = false;
}

//Inputhandling------------------------------------------------------------------------------

void Life3D_Engine::input()
{
	processInput(this->window, this->particleShader);
	glfwSetCursorPosCallback(window, mouse_callback_wrapper);
	glfwSetScrollCallback(window, scroll_callback_wrapper);
}

void Life3D_Engine::processInput(GLFWwindow* window, Shader reflectionShader)
{
	//Keyboard shortcuts
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !this->settingsKeyPressed)
	{
		this->viewMode = !this->viewMode;
		this->settingsKeyPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		this->settingsKeyPressed = false;
	}
	if (!this->viewMode) {
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->angleHor += this->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->angleHor -= this->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->angleVer += this->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->angleVer -= this->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !this->startKeyPressed)
	{
		this->start = !this->start;
		this->startKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		this->startKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !this->randomKeyPressed)
	{
		this->randomAttraction();
		this->randomKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
	{
		this->randomKeyPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !this->randPosKeyPressed)
	{
		this->randomPosition();
		this->randPosKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		this->randPosKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !this->borderKeyPressed)
	{
		this->borders = !this->borders;
		this->borderKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		this->borderKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !this->shadingTypeKeyPressed)
	{
		this->shaderChoice = ++this->shaderChoice %2;
		this->shadingTypeKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
	{
		this->shadingTypeKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		this->postProcessingChoice = 7;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		this->postProcessingChoice = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		this->postProcessingChoice = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		this->postProcessingChoice = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		this->postProcessingChoice = 4;
	}

	//Kamerabewegung
	this->updateCamera();
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

	if (this->viewMode)
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

//Helper------------------------------------------------------------------------------

std::vector<Life3D_Particles*> Life3D_Engine::create(int number, glm::vec3 color)
{
	//Create particles for a specific type at a random position inside the border box
	std::vector<Life3D_Particles*> group;
	for (int i = 0; i < number; i++)
	{
		int posX = random((int)this->cubeSize * 2, -(int)this->cubeSize);
		int posY = random((int)this->cubeSize * 2, -(int)this->cubeSize);
		int posZ = random((int)this->cubeSize * 2, -(int)this->cubeSize);
		group.push_back(new Life3D_Particles(glm::vec3(posX, posY, posZ), color));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::scale(model, glm::vec3(this->scale));
		this->modelMatrices.push_back(model);
		this->colorData.push_back(color);
	}
	this->particles.push_back(group);
	return group;
}

int Life3D_Engine::random(int range, int start)
{
	return rand() % range + start;
}

float Life3D_Engine::force(float d, float a)
{
	//Force function to prevent particles from collapsing into singularity @Tom Mohr
	const float beta = 0.3f;
	if (d < beta)
	{
		return d / beta - 1;
	}
	else if (beta < d && d < 1)
	{
		return a * (1 - abs(2 * d - 1 - beta) / (1 - beta));
	}
	else {
		return 0.0f;
	}
}

void Life3D_Engine::randomPosition()
{
	//sets all particles to a random position
	int p = 0;
	for (int i = 0; i < this->particles.size(); i++)
	{
		for (auto* particle : this->particles[i])
		{
			int posX = random((int)this->cubeSize * 2, -(int)this->cubeSize);
			int posY = random((int)this->cubeSize * 2, -(int)this->cubeSize);
			int posZ = random((int)this->cubeSize * 2, -(int)this->cubeSize);
			particle->setPos(glm::vec3(posX, posY, posZ));
			p++;
		}
	}
}

void Life3D_Engine::randomAttraction()
{
	//sets the attraction matrix to random values
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++) {
			this->attraction[i][j] = (float)random(200, -100) / 100;
		}
	}
}

//Updates------------------------------------------------------------------------------

void Life3D_Engine::update()
{
	this->updateDeltaTime();
	this->input();

	if (this->start)
	{
		//Multithreading for parallel computing on different CPU Kernels
		std::vector<std::thread> threads;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto threadFunc = [this, i, j]() {
					this->updateInteraction(this->particles[i], this->particles[j], this->attraction[i][j]);
				};

				threads.emplace_back(threadFunc);
			}
		}

		//Wait until all threads are finished
		for (auto& thread : threads)
		{
			thread.join();
		}
		//for (int i = 0; i < 5; i++)
		//{
		//	for (int j = 0; j < 5; j++)
		//	{
		//		this->updateInteraction(this->particles[i], this->particles[j], this->attraction[i][j]);
		//	}
		//}
	}

	//Update all particles
	for (int i = 0; i < this->particles.size(); i++)
	{
		for (auto* particle : this->particles[i])
		{
			particle->update();
		}
	}


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
	this->camera.setSpeed(this->cameraSpeed);
}

void Life3D_Engine::updateInteraction(std::vector<Life3D_Particles*> particle1, std::vector<Life3D_Particles*> particle2, float attraction)
{
	//Each particle receives a force vector from each other (That would be a perfect possibility to parallelize that on the GPU)
	for (int i = 0; i < particle1.size(); i++)
	{
		float fx = 0;	//f hier gleichzusetzen mit a, da die Massen aller Teilchen 1 sind
		float fy = 0;
		float fz = 0;

		//Calculation of the force vectors	
		for (int j = 0; j < particle2.size(); j++)
		{
			float dx = particle2[j]->getPos().x - particle1[i]->getPos().x;
			float dy = particle2[j]->getPos().y - particle1[i]->getPos().y;
			float dz = particle2[j]->getPos().z - particle1[i]->getPos().z;
			float distance = sqrt(dx * dx + dy * dy + dz * dz);

			if (distance > 0 && distance < this->distanceMax)
			{
				const float f = this->force(distance / this->distanceMax, attraction);

				fx += f * dx / distance;
				fy += f * dy / distance;
				fz += f * dz / distance;

			}
		}

		fx *= this->distanceMax;
		fy *= this->distanceMax;
		fz *= this->distanceMax;

		//Update particle velocity and position
		particle1[i]->setVel(glm::vec3(
			particle1[i]->getVelocity().x * this->friction + fx * this->deltaTime * this->timeFactor,
			particle1[i]->getVelocity().y * this->friction + fy * this->deltaTime * this->timeFactor,
			particle1[i]->getVelocity().z * this->friction + fz * this->deltaTime * this->timeFactor
		));

		particle1[i]->setPos(glm::vec3(
			particle1[i]->getPos() + particle1[i]->getVelocity() * this->deltaTime/((float)this->amount / 1000) * this->timeFactor
		));

		//Borders
		this->updateBorders(particle1[i]);
	}
}

void Life3D_Engine::updateBorders(Life3D_Particles* particle)
{
	//x -
	if (this->borders)
	{
		if (particle->getPos().x <= -cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x * -1,
				particle->getVelocity().y,
				particle->getVelocity().z
			));

			particle->setPos(glm::vec3(
				-cubeSize + 1,
				particle->getPos().y,
				particle->getPos().z
			));
		}
		//x +
		if (particle->getPos().x >= cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x * -1,
				particle->getVelocity().y,
				particle->getVelocity().z
			));

			particle->setPos(glm::vec3(
				cubeSize - 5,
				particle->getPos().y,
				particle->getPos().z
			));
		}

		//y -
		if (particle->getPos().y <= -cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x,
				particle->getVelocity().y * -1,
				particle->getVelocity().z
			));

			particle->setPos(glm::vec3(
				particle->getPos().x,
				-cubeSize + 1,
				particle->getPos().z
			));
		}
		//y +
		if (particle->getPos().y >= cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x,
				particle->getVelocity().y * -1,
				particle->getVelocity().z
			));

			particle->setPos(glm::vec3(
				particle->getPos().x,
				cubeSize - 5,
				particle->getPos().z
			));
		}

		//z -
		if (particle->getPos().z <= -cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x,
				particle->getVelocity().y,
				particle->getVelocity().z * -1
			));

			particle->setPos(glm::vec3(
				particle->getPos().x,
				particle->getPos().y,
				-cubeSize + 1
			));
		}
		//z +
		if (particle->getPos().z >= cubeSize)
		{
			particle->setVel(glm::vec3(
				particle->getVelocity().x,
				particle->getVelocity().y,
				particle->getVelocity().z * -1
			));

			particle->setPos(glm::vec3(
				particle->getPos().x,
				particle->getPos().y,
				cubeSize - 5
			));
		}
	}
}

//Rendering------------------------------------------------------------------------------

void Life3D_Engine::Draw()
{
	//Draw scene to framebuffer which will be rendered as a texture for post processing purposes
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glEnable(GL_DEPTH_TEST);  
	//Sets Backgroundcolor to dimmed color of the directional light color
	glClearColor(this->dirLightColor.x*0.1f, this->dirLightColor.y * 0.1f, this->dirLightColor.z * 0.1f, 1.0f);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render
	this->DrawScene();

	this->DrawScreen();

	if (this->showBorder)
	{
		this->DrawCube();
	}
	//Draw sun
	if (this->shaderChoice == 0)
	{
		this->DrawSun();
	}
	
	this->DrawText();
	
	this->DrawSettings();

	ImGui::EndFrame();
}

void Life3D_Engine::DrawScene()
{
	//Update view and projection matrix
	this->projection = glm::perspective(glm::radians(this->camera.Zoom), (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 10000.0f);
	this->view = camera.GetViewMatrix();

	//Update uniforms in particleShader
	this->particleShader.use();
	this->particleShader.setMat4("projection", this->projection);
	this->particleShader.setMat4("view", this->view);

	this->particleShader.setVec3("dirLightColor", glm::vec3(this->dirLightColor.x, this->dirLightColor.y, this->dirLightColor.z));
	this->particleShader.setVec3("dirLightDir", -this->dirLightPos);
	this->particleShader.setVec3("lightColor", glm::vec3(BLACK));
	this->particleShader.setVec3("lightPos", glm::vec3(0.0f, 0.0f,0.0f));
	this->particleShader.setVec3("viewPos", camera.Position);

	this->particleShader.setInt("shininess", 512);
	this->particleShader.setInt("shaderChoice", this->shaderChoice);

	//Update instanced matrix (transformation matrix)
	int k = 0;
	for (int i = 0; i < this->particles.size(); i++)
	{
		for (auto* particle : this->particles[i])
		{
			this->modelMatrices[k] = particle->getModel();
			k++;
		}
	}

	//Batchupdates for transformation matrices
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 5 * this->amount * sizeof(glm::mat4), &modelMatrices[0]);

	for (unsigned int i = 0; i < this->sphere->meshes.size(); i++)
	{
		glBindVertexArray(this->sphere->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(this->sphere->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, 5 * this->amount);
		glBindVertexArray(0);
	}

	//Update Particle Scale
	int p = 0;
	for (int i = 0; i < this->particles.size(); i++)
	{
		for (auto* particle : this->particles[i])
		{
			particle->setScale(this->scale);
			p++;
		}
	}
	glBindVertexArray(0);
}

void Life3D_Engine::DrawSettings()
{
	//Tell ImGUI its a new Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (!viewMode)
	{
		ImGui::Begin("Steuerung", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImVec2 pos(5, 50);
		ImGui::SetWindowPos(pos);

		// Init Settingswindow
		ImVec2 size((float)this->WINDOW_WIDTH / 4, (float)this->WINDOW_HEIGHT * 0.9f);
		ImGui::SetWindowSize(size);

		//Settings
		ImGui::Text("Settings");
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Timefactor", &this->timeFactor, 0.0f, 2.0f);
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Distance", &this->distanceMax, 0.0f, 700.0f);
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Scale", &this->scale, 0.00f, 2.0f);
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Boxsize", &this->cubeSize, 1.0f, 700.0f);
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Camspeed", &this->cameraSpeed, 1.0f, 1000.0f);

		//Simulation control
		if (ImGui::Button("Random")) {
			this->randomAttraction();
		}

		const char* play = "Start";
		if (this->start){
			play = "Stopp";
		}
		ImGui::SameLine();
		if (ImGui::Button(play))
		{
			this->start = !this->start;
		}

		ImGui::SameLine();
		if (ImGui::Button("RandomPos"))
		{
			this->randomPosition();
		}
	
		ImGui::SameLine();
		if (ImGui::Button("Show Border"))
		{
			if (showBorder) {
				this->showBorder = false;
			}
			else
				this->showBorder = true;
		}
		ImGui::SameLine();

		const char* borderChoice = "Borders";
		if (this->borders)
			borderChoice = "No Borders";
		if (ImGui::Button(borderChoice))
		{
			this->borders = !this->borders;
		}

		//Postprocessing
		ImGui::Text("Postprocessing");
		if (ImGui::Button("Blur"))
		{
			this->postProcessingChoice = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Edges"))
		{
			this->postProcessingChoice = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Inversion"))
		{
			this->postProcessingChoice = 3;
		}
		ImGui::SameLine();
		if (ImGui::Button("Grayscale"))
		{
			this->postProcessingChoice = 4;
		}
		ImGui::SameLine();
		if (ImGui::Button("Normal"))
		{
			this->postProcessingChoice = 7;
		}

		//Shading Choice
		ImGui::Text("Shaderwahl");
		const char* shading = "DirLightShading";
		if (this->shaderChoice == 0)
			shading = "NormalShading";
		if (ImGui::Button(shading))
		{
			this->shaderChoice = ++this->shaderChoice % 2;
		}	

		ImGui::SameLine();
		if (ImGui::Button("RandomColors"))
		{
			//Random number generator [0;1]
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(0.0f, 1.0f);

			glm::vec3* rColors = new glm::vec3[this->amount * 5];

			for (int i = 0; i < this->amount * 5 - 1; i++) {
				rColors[i] = glm::vec3(dis(gen), dis(gen), dis(gen));
			}

			this->randomColors = new glm::vec3[this->amount * 5];
			std::memcpy(this->randomColors, rColors, sizeof(glm::vec3) * (this->amount * 5));
			delete[] rColors;

			//Update colorVBO
			glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 5*amount *sizeof(glm::vec3), &this->randomColors[0]);
		}		
		ImGui::SameLine();
		if (ImGui::Button("NormalColors"))
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->colorVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 5*amount *sizeof(glm::vec3), &this->colorData[0]);
		}

		//Slider (kann man bestimmt schöner mit Dictionarys lösen)
		ImGui::Text("Rot");
		ImGui::SliderFloat("rr", &this->attraction[0][0], -1.0f, 1.0f);
		ImGui::SliderFloat("rg", &this->attraction[0][1], -1.0f, 1.0f);
		ImGui::SliderFloat("rb", &this->attraction[0][2], -1.0f, 1.0f);											
		ImGui::SliderFloat("ry", &this->attraction[0][3], -1.0f, 1.0f);
		ImGui::SliderFloat("rw", &this->attraction[0][4], -1.0f, 1.0f);

		ImGui::Text("Gruen");
		ImGui::SliderFloat("gr", &this->attraction[1][0], -1.0f, 1.0f);
		ImGui::SliderFloat("gg", &this->attraction[1][1], -1.0f, 1.0f);
		ImGui::SliderFloat("gb", &this->attraction[1][2], -1.0f, 1.0f);
		ImGui::SliderFloat("gy", &this->attraction[1][3], -1.0f, 1.0f);
		ImGui::SliderFloat("gw", &this->attraction[1][4], -1.0f, 1.0f);

		ImGui::Text("Blau");
		ImGui::SliderFloat("br", &this->attraction[2][0], -1.0f, 1.0f);
		ImGui::SliderFloat("bg", &this->attraction[2][1], -1.0f, 1.0f);
		ImGui::SliderFloat("bb", &this->attraction[2][2], -1.0f, 1.0f);
		ImGui::SliderFloat("by", &this->attraction[2][3], -1.0f, 1.0f);
		ImGui::SliderFloat("bw", &this->attraction[2][4], -1.0f, 1.0f);

		ImGui::Text("Gelb");
		ImGui::SliderFloat("yr", &this->attraction[3][0], -1.0f, 1.0f);
		ImGui::SliderFloat("yg", &this->attraction[3][1], -1.0f, 1.0f);
		ImGui::SliderFloat("yb", &this->attraction[3][2], -1.0f, 1.0f);
		ImGui::SliderFloat("yy", &this->attraction[3][3], -1.0f, 1.0f);
		ImGui::SliderFloat("yw", &this->attraction[3][4], -1.0f, 1.0f);

		ImGui::Text("Weiss");
		ImGui::SliderFloat("wr", &this->attraction[4][0], -1.0f, 1.0f);
		ImGui::SliderFloat("wg", &this->attraction[4][1], -1.0f, 1.0f);
		ImGui::SliderFloat("wb", &this->attraction[4][2], -1.0f, 1.0f);
		ImGui::SliderFloat("wy", &this->attraction[4][3], -1.0f, 1.0f);
		ImGui::SliderFloat("ww", &this->attraction[4][4], -1.0f, 1.0f);
		ImGui::ColorPicker3("DirLight", (float*)&this->dirLightColor, ImGuiColorEditFlags_InputRGB);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void Life3D_Engine::DrawScreen()
{
	//Draw Scene texture to standard Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->screenShader.use();
	this->screenShader.setInt("screenTexture", 0);
	this->screenShader.setInt("choice", this->postProcessingChoice);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->screenVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Life3D_Engine::DrawCube()
{
	float scaleFactor = this->cubeSize; 
	this->borderBox->Translate(glm::vec3(1.0f));
	this->borderBox->Scale(scaleFactor);
	this->borderBox->Draw(&this->cubeShader, this->projection, this->view, BLUE_GREEN);
}

void Life3D_Engine::DrawSun()
{
	float lichtBahnRadius = this->cubeSize * 4.f;
	float sinAngleHor = sin(angleHor);
	float cosAngleHor = cos(angleHor);
	float sinAngleVer = sin(angleVer);
	float cosAngleVer = cos(angleVer);

	//Calculate Sun position
	float lightX = sinAngleHor * sinAngleVer * lichtBahnRadius;
	float lightY = cosAngleVer * lichtBahnRadius;
	float lightZ = cosAngleHor * sinAngleVer * lichtBahnRadius;

	this->dirLightPos = glm::vec3(lightX, lightY, lightZ);

	this->sun->Translate(this->dirLightPos);
	this->sun->Scale(40.0f);
	this->sun->Draw(&this->sunShader, this->projection, this->view, glm::vec3(this->dirLightColor.x, this->dirLightColor.y, this->dirLightColor.z));
}

void Life3D_Engine::DrawText()
{
	this->textRenderer->Draw(this->textShader, "FPS: " + std::to_string((int)this->FPS), 0.0f, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->textRenderer->Draw(this->textShader, "Pos: " + std::to_string(this->camera.Position.x) + ", " + std::to_string(this->camera.Position.y) + ", " + std::to_string(this->camera.Position.z), 0.0f, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->textRenderer->Draw(this->textShader, "CameraView: " + std::to_string(this->camera.Front.x) + ", " + std::to_string(this->camera.Front.y) + ", " + std::to_string(this->camera.Front.z), 0.0f, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->textRenderer->Draw(this->textShader, "DeltaTime: " + std::to_string(this->deltaTime), 0.0f, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	this->textRenderer->Draw(this->textShader, "Start: " + std::to_string(this->start), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->textRenderer->Draw(this->textShader, "Borders: " + std::to_string(this->borders), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	std::string shading = "NormalShading";
	if (this->shaderChoice == 0)
		shading = "DirLightShading";
	this->textRenderer->Draw(this->textShader, "Shading: " + shading, this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	this->textRenderer->Draw(this->textShader, "Amount Particles: " + std::to_string(this->amount * 5), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
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