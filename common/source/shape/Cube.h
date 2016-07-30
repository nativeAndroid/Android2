#pragma once
#include <enviroment/camera/Camera.h>
#include <enviroment/loaderUtility/Texture.h>
#include <enviroment/loaderUtility/ShaderLoader.h>


class Cube
{
private:
#ifdef __ANDROID__
	AAssetManager* mgr;
#endif
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
	void resetToDefaultCamera();
#ifdef __ANDROID__
	void setMGR(AAssetManager* mgr);
#endif
	void defaultChange() const;

	~Cube();
};

