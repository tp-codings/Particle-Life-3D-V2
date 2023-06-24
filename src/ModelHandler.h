#pragma once
#include <ModelLoader/model.h>

class ModelHandler
{
public: 
	ModelHandler(string path);
	void Draw(Shader *s, glm::mat4 projection, glm::mat4 view, glm::vec3 color);
	void Translate(glm::vec3 direction);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(float factor);

private:

	Model* model;

	glm::mat4 transformation;

	glm::vec3 translation;
	glm::vec3 rotationAxis;

	float rotationAngle;
	float scale;

	void Transform();
};

