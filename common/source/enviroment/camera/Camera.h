#pragma once
#ifndef TRANSFORM_H
	#include <glm/gtx/transform.hpp>
#endif

/*
	please take a look here http://stackoverflow.com/questions/30690348/make-camera-lookdirection-look-front-face/30691275#30691275
*/

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 front;
	
	glm::vec3 up;
	glm::vec3 mRight;
	glm::vec3 WorldUp;
private:
	float perspective_angle = 45.0f;
	float inf_metric = 0.1f;
	float sup_metric = 100.0f;

protected:
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 MVP;
	glm::mat4 frustrum;
	float width, height;
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : front(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		this->position = position;
		this->WorldUp = up;
		this->up = up;
		this->GetViewMatrix();
		updateCameraVectorsToDefault();
	}

	glm::mat4 GetViewMatrix();

	glm::vec3 getPos();
	glm::vec3 getLookDirection();
	glm::vec3 getUpVector();

	void setLook(glm::vec3 look);
	void setUpVector(glm::vec3 up);
	void setPosition(glm::vec3 pos);

	void updateCameraVectors();
	void updateCameraVectorsToDefault();
	void updateCameraProjection();

	void updateToDefaultProjection();
	
	void setWithAndHeight(float width, float height);
	void computeMVP();
	glm::mat4 getMVP();
private:

};