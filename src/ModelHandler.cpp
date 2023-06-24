#include "ModelHandler.h"

ModelHandler::ModelHandler(string path)
{
	this->model = new Model(path);
	this->transformation = glm::mat4(1.0f);
	this->translation = glm::vec3(1.0f);
	this->rotationAxis = glm::vec3(1.0f);
	this->rotationAngle = 0.0f;
	this->scale = 0.0f;
}

void ModelHandler::Draw(Shader *s, glm::mat4 projection, glm::mat4 view, glm::vec3 color)
{
	this->Transform();
	s->use();
	s->setMat4("projection", projection);
	s->setMat4("view", view);
	s->setMat4("model", this->transformation);
	s->setVec3("color", color);

	this->model->Draw(*s);
}

void ModelHandler::Translate(glm::vec3 direction)
{
	this->translation = direction;
}

void ModelHandler::Rotate(float angle, glm::vec3 axis)
{
	this->rotationAngle = angle;
	this->rotationAxis = axis;
}

void ModelHandler::Scale(float factor)
{
	this->scale = factor;
}

void ModelHandler::Transform()
{
	this->transformation = glm::mat4(1.0f);
	this->transformation = glm::translate(this->transformation, this->translation);
	this->transformation = glm::rotate(this->transformation, this->rotationAngle, this->rotationAxis);
	this->transformation = glm::scale(transformation, glm::vec3(scale));
}


