#include "Life3D_Particles.h"


Life3D_Particles::Life3D_Particles(glm::vec3 pos, glm::vec3 color)
{
	this->position = pos;
	this->model = glm::mat4(1.0f);
	this->model = glm::scale(this->model, glm::vec3(0.08f));
	this->model = glm::translate(this->model, pos);
	this->velocity = glm::vec3(0.0f);
	this->scale = 0.8f;
	this->color = color;
}

glm::vec3 Life3D_Particles::getPos()
{
	return this->position;
}

glm::mat4 Life3D_Particles::getModel()
{
	return this->model;
}

glm::vec3 Life3D_Particles::getColor()
{
	return this->color;
}

glm::vec3 Life3D_Particles::getVelocity()
{
	return this->velocity;
}

void Life3D_Particles::setPos(glm::vec3 pos)
{
	this->position = pos;
}

void Life3D_Particles::setVel(glm::vec3 vel)
{
	this->velocity = vel;
}

void Life3D_Particles::setScale(float fac)
{
	this->scale = fac;
}

void Life3D_Particles::update()
{
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, this->position);
	this->model = glm::scale(this->model, glm::vec3(this->scale));
}


