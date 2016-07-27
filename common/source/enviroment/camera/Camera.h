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

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
private:
	glm::vec3 upDirection;
	glm::vec3 lookDirection;
	glm::vec3 position;

	glm::vec3 Right;
	glm::vec3 WorldUp;
		
	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
	
	glm::mat4 MVP;
protected:
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 frustrum;
	float width, height;
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: lookDirection(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	void updateCameraVectors();
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	glm::mat4 GetViewMatrix()
	{
		return frustrum;
	}

	void computeFrustrum()
	{
		frustrum = glm::lookAt(this->position, this->position + this->lookDirection, this->upDirection);
	}

	static glm::mat4 getDefaultViewMatrix()
	{
		return glm::lookAt(
			glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

	void setDefaultProjection()
	{
		projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
		model = glm::mat4(1.0f);
	}

	void setWithAndHeight(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	void computeMVP()
	{
		MVP =   projection * getDefaultViewMatrix() * model;;
	}

	glm::mat4 getMVP()
	{
		return projection*getDefaultViewMatrix()*model;
	}
};