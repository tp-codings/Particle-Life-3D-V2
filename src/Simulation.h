#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Shader/shader.h>
#include <Camera/camera.h>
#include <ModelLoader/model.h>
#include <SkyBox/Skybox.h>

#include "Life3D_Particles.h"
#include "TextRenderer.h"
#include "ModelHandler.h"
class Simulation
{
public:
	Simulation(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT);

	void update(float deltaTime, int FPS, Camera camera);
	void render();

	float getCameraSpeed();
	bool getViewMode();

private:
	//Window
	GLFWwindow* window;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	//Text
	TextRenderer* textRenderer;
	int fontSize;

	//Shader
	Shader particleShader;
	Shader screenShader;
	Shader cubeShader;
	Shader sunShader;
	Shader textShader;
	Shader skyboxShader;

	//Matrizen
	glm::mat4 projection;
	glm::mat4 view;

	float attraction[5][5];

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec3> colorData;
	glm::vec3* randomColors;

	//TIMING
	float deltaTime;
	float FPS;

	//Particles
	std::vector<Life3D_Particles*> red;
	std::vector<Life3D_Particles*> yellow;
	std::vector<Life3D_Particles*> green;
	std::vector<Life3D_Particles*> blue;
	std::vector<Life3D_Particles*> white;
	std::vector<std::vector<Life3D_Particles*>> particles;

	glm::vec3 dirLightDirection;
	glm::vec3 dirLightPos;
	float angleHor;
	float angleVer;

	//World objects
	Model* sphere;

	ModelHandler* borderBox;
	ModelHandler* sun;

	Camera camera;

	Skybox *oceanBox;
	std::vector<std::string> ocean
	{
		"resources\\textures\\skybox\\right.jpg",
			"resources\\textures\\skybox\\left.jpg",
			"resources\\textures\\skybox\\top.jpg",
			"resources\\textures\\skybox\\bottom.jpg",
			"resources\\textures\\skybox\\front.jpg",
			"resources\\textures\\skybox\\back.jpg"
	};

	//Friction
	float TIME_STEP;
	float friction;
	float frictionHalfLife;

	//Settings
	int amount;
	int postProcessingChoice;
	int shaderChoice;

	float timeFactor;
	float distanceMax;
	float cubeSize;
	float scale;
	float cameraSpeed;

	bool viewMode;
	bool start;
	bool showBorder;
	bool borders;

	ImVec4 dirLightColor;

	bool startKeyPressed;
	bool settingsKeyPressed;
	bool randomKeyPressed;
	bool randPosKeyPressed;
	bool borderKeyPressed;
	bool shadingTypeKeyPressed;

	//Framebuffer
	unsigned int screenVAO;
	unsigned int screenVBO;
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;

	unsigned int instanceVBO;
	unsigned int colorVBO;

	float* quadVertices;

	//Inits------------------------------------------------------------------------------

	void initVertices();
	void initBuffer();
	void initShader();
	void initVariables();
	void initModels();
	void initParticles();

	//Input------------------------------------------------------------------------------

	void processInput(float deltaTime);

	//Helper------------------------------------------------------------------------------

	std::vector<Life3D_Particles*> create(int number, glm::vec3 color);
	int random(int range, int start);
	float force(float d, float a);
	void randomPosition();
	void randomAttraction();

	//Updates------------------------------------------------------------------------------

	void updateInteraction(std::vector<Life3D_Particles*> particle1, std::vector<Life3D_Particles*>particle2, float attraction);
	void updateBorders(Life3D_Particles* particle);

	//Rendering------------------------------------------------------------------------------

	void DrawScene();
	void DrawSettings();
	void DrawScreen();
	void DrawCube();
	void DrawSkyBox();
	void DrawSun();
	void DrawText();
};

