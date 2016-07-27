#pragma once
#include <enviroment/camera/Camera.h>
#include <enviroment/loaderUtility/Texture.h>
#include <enviroment/loaderUtility/ShaderLoader.h>


class Cube
{
private:
	AAssetManager* mgr;
	glm::vec3 cartesianToSpherical(glm::vec3 in)
	{
		float distanceFromOrigin = glm::length(in);
		float phi = atan2f(in.x, in.z);
		float theta = atan2f(hypot(in.x, in.z), in.y);
		return glm::vec3(distanceFromOrigin, theta, phi);
	}

	glm::vec3 sphericalToCartesian(glm::vec3 sphere)
	{
		float x = sphere.x*sin(sphere.y) * cos(sphere.z);
		float y = sphere.x*sin(sphere.y) * sin(sphere.z);
		float z = sphere.x * cos(sphere.y);

		return glm::vec3(x, y, z);
	}
private:
	Camera *myCam;
	Texture textureTools;
	ShaderLoader shaderLoader;
	
	GLuint Texture_;
	GLuint TextureID;
	GLuint uvbuffer;
	

	GLuint vertexbuffer;
	GLuint colorbuffer;

	GLuint programID;
	GLuint MatrixID;

	GLuint vertexPosition_modelspaceID;
	GLuint vertexUVID;
public:
	Cube();

	void loadShape();
	void loadShaders();
	void loadTexture();
	void drawShape();

public:
	void setCamera(int width, int height);
	void setMGR(AAssetManager* mgr);

	void defaultChange()
	{
		glm::vec3 pos = myCam->getPos();
		glm::vec3 look = myCam->getLookDirection();
		glm::vec3 up = myCam->getUpVector();

		glm::vec3 pos_ = cartesianToSpherical(pos + glm::vec3(30,0,0));
		glm::vec3 look_ = cartesianToSpherical(look);
		glm::vec3 up_ = cartesianToSpherical(up);

		pos = sphericalToCartesian(pos_);
		look = sphericalToCartesian(look_);
		up = sphericalToCartesian(up_);
		
		myCam->setLook(look);
		myCam->setUpVector(up);
		myCam->setPosition(pos);
		myCam->updateCameraVectors();
	}

	~Cube();
};

