#pragma once

#ifdef __ANDROID__
	#include <pch.h>
#elif _WIN32
//#ifndef _glfw3_h_
//#include <GLFW/glfw3.h>
//#endif
//#ifndef __glew_h__
//	#include <GL/GLEW.h>
//#endif//__glew_h__
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#endif
class ShaderLoader
{
#ifdef __ANDROID__
	AAssetManager* mgr;
#endif
protected:
	//following method should be separated into another class 
	inline GLuint createShader(GLenum type, const char *shaderSrc); //create shader
	inline const char* decompose(const char * filename);//read shader from  assets
public:
	GLuint loadShaders(const char * vertex_path, const char * fragment_path);
#ifdef __ANDROID__
	void setMGR(AAssetManager* mgr);
#endif

};
#ifdef __ANDROID__

inline void ShaderLoader::setMGR(AAssetManager* mgr)
{
	this->mgr = mgr;
}

#endif // __ANDROID__

inline GLuint ShaderLoader::createShader(GLenum type, const char* shaderSrc)
{
	GLuint shader;
	GLint compiledStatus;
	shader = glCreateShader(type);
	if (shader == 0)
		return 0;
	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);
	// Compile the shader
	glCompileShader(shader);
	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);

	if (!compiledStatus)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			#ifdef __ANDROID__
			LOGW("Bred");
			LOGW(infoLog);
			free(infoLog);
			#elif _WIN32
			//errpr
			#endif
			
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

#ifdef __ANDROID__
inline const char* ShaderLoader::decompose(const char* filename)
{
	AAsset* file = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);

	assert(file != NULL);

	size_t size = AAsset_getLength(file);
	char* buffer = (char*)malloc(sizeof(char)*size + 1);
	int err = AAsset_read(file, buffer, size);
	buffer[size] = '\0';

	if (err < 0) {
		LOGW("WARNING SHADER ");
		return "";
	}

	AAsset_close(file);
	return buffer;
}
#elif _WIN32
inline const char* ShaderLoader::decompose(const char* filename)
{
	FILE* ptrShaderFile = fopen(filename,"r");
	int fileSize = 0;
	char* shaderCode = NULL;

	fseek(ptrShaderFile, 0, SEEK_END);
	fileSize = ftell(ptrShaderFile);
	rewind(ptrShaderFile);

	shaderCode = (char*)malloc(sizeof(char)*(fileSize + 1));
	memset(shaderCode, 0, fileSize);
	fread(shaderCode, sizeof(char), fileSize, ptrShaderFile);
	shaderCode[fileSize] = '\0';
	fclose(ptrShaderFile);
	
	return shaderCode;
}
#endif


inline GLuint ShaderLoader::loadShaders(const char* vertex_path, const char* fragment_path)
{
	#ifdef __ANDROID__
	assert(mgr != NULL);
	#endif

	const GLchar* vertexShaderSrc = decompose(vertex_path);
	const GLchar* fragmentShaderSrc = decompose(fragment_path);

	GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	assert(vertexShader != NULL);
	assert(fragmentShader != NULL);

	GLuint shprog = glCreateProgram();

	assert(shprog != 0);

	glAttachShader(shprog, vertexShader);
	glAttachShader(shprog, fragmentShader);
	glLinkProgram(shprog);

	GLint linked;

	glGetProgramiv(shprog, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(shprog, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(shprog, infoLen, NULL, infoLog);
			#ifdef __ANDROID__
			LOGW(infoLog);
			free(infoLog);
			#endif
		}
		glDeleteProgram(shprog);
		#ifdef __ANDROID__
		LOGW("SHADER CREATING PROGRAM");
		#endif
		return 0;
	}
	// Store the program object
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	return shprog;
}
