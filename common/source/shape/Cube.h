#pragma once
#include <enviroment/camera/Camera.h>
#include <enviroment/loaderUtility/Texture.h>

class Cube
{
private:
	AAssetManager* mgr;

	inline GLuint createShader(GLenum type, const char *shaderSrc); //create shader
	inline char* decompose(const char * filename);//read shader from  assets
	GLuint loadShaders(const char * vertex_path, const char * fragment_path);
	void setMGR(AAssetManager* mgr);
	void setWidthAndHeigh(int width, int height);

private:
	Camera *myCam;
	Texture textureTools;
	
	GLuint Texture;
	GLuint TextureID;
	GLuint uvbuffer;
	
	int width, height;

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

	~Cube();
};

