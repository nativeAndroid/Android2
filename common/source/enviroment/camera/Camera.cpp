#ifdef  __ANDROID__
	#include "pch.h"
#endif //  __ANDROID__
#include <enviroment/camera/Camera.h>

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	this->front = glm::normalize(front);
	this->mRight = glm::normalize(glm::cross(this->front, this->WorldUp));
	this->up = glm::normalize(glm::cross(this->mRight, this->front));
}

void Camera::updateCameraVectorsToDefault()
{
	WorldUp = glm::vec3(4, 3, 3);
	position = glm::vec3(0,0,0);
	mRight = glm::vec3(0, 1, 0);
	frustrum = glm::lookAt(WorldUp, position + front, up);
}

void Camera::updateCameraProjection()
{
	frustrum = glm::lookAt(WorldUp, position + front, up);
}

void Camera::updateToDefaultProjection()
{
	model = glm::mat4(1.0f);
	projection = glm::perspective(45.0f, (float)width / (float)height, (float)inf_metric, (float)sup_metric);
}

glm::mat4 Camera::GetViewMatrix()
{
	frustrum = glm::lookAt(
		this->WorldUp,
		this->position + this->front,
		this->up
	);
	return frustrum;
}


glm::vec3 Camera::getPos()
{
	return WorldUp;
}

glm::vec3 Camera::getLookDirection()
{
	return position;
}

glm::vec3 Camera::getUpVector()
{
	return up;
}

void Camera::setLook(glm::vec3 look)
{
	position = look;
}

void Camera::setUpVector(glm::vec3 up)
{
	this->up = up;
}

void Camera::setPosition(glm::vec3 pos)
{
	WorldUp = pos;
}

void Camera::setWithAndHeight(float width, float height)
{
	this->width = width;
	this->height = height;
}

void Camera::computeMVP()
{
	MVP = projection*frustrum*model;
}

glm::mat4 Camera::getMVP()
{
	return  MVP;
}
