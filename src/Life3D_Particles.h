#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Life3D_Particles
{
public:
	Life3D_Particles(glm::vec3 pos, glm::vec3 color);

	glm::vec3 getPos();
	glm::mat4 getModel();
	glm::vec3 getColor();
	glm::vec3 getVelocity();

	void setPos(glm::vec3 pos);
	void setVel(glm::vec3 vel);
	void setScale(float fac);

	void update(); //Modelupdate

private:
	glm::vec3 position; 
	glm::vec3 velocity; 
	glm::mat4 model; 
	glm::vec3 color;   
	float scale;
	
};

