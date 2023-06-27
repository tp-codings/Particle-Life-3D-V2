#include "Engine.h"

Engine::Engine()
{
	this->initVariables();
}

void Engine::run()
{
	//Main loop (Exit on ESC or Cross)
	while (!glfwWindowShouldClose(this->window))
	{
		this->update();
		this->render();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	//ImGUI Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Engine::initVariables()
{
	this->windowHandler = new WindowHandler();
	this->window = this->windowHandler->getWindow();
	this->ParticleLife = new Simulation(this->window, this->windowHandler->getWindowSize().x, this->windowHandler->getWindowSize().y);
}

void Engine::update()
{
	this->ParticleLife->update(this->windowHandler->getDeltaTime(), this->windowHandler->getFPS(), this->windowHandler->getCamera());
	this->windowHandler->update(this->ParticleLife->getViewMode(), this->ParticleLife->getCameraSpeed());
}

void Engine::render()
{
	this->ParticleLife->render();
}