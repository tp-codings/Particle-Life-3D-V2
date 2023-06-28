#pragma once
#include "Simulation.h"
#include "WindowHandler.h"

class Engine
{
public:
	Engine();
	void run();

private:
	Simulation* simulation;
	WindowHandler* windowHandler;
	GLFWwindow* window;

	void initVariables();
	
	void update();
	void render();
};

