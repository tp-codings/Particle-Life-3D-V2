#pragma once
#include "Simulation.h"
#include "WindowHandler.h"

class Engine
{
public:
	Engine();
	void run();

private:
	Simulation* ParticleLife;
	WindowHandler* windowHandler;
	GLFWwindow* window;

	void initVariables();
	
	void update();
	void render();
};

