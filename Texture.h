#pragma once
#include <map>
#include "stb_image.h"

#include <string>
#include <glew.h>
 
 
#include <GL/GLu.h>
#include <algorithm>
#include <iostream>
#include "Errors.h"
#include <vector>
#include "RenderUtil.h"


class Texture
{
public:
	/*
		param path path to picture
		Will load the texture from path
	*/
	Texture(){ID = -1;}
	Texture(std::string path);
	virtual ~Texture(void);
	/*
		param unit int between 0 and 27
		Will bind the texture on the graphicscard in unit slot
	*/
	void bind(int unit = 0) const;
	GLuint ID;
	int width,height;
	std::string texturepath;
	void drawTexture(bool check) const; //disables or enables Textures in the engine
	void addTexture(std::string path = "res/texture/white.png"); //replaces current Texture with path
	void releaseTexture(); //clears memory in opengl
};

struct TextureAndCount
{
	Texture texture;
	GLuint count; 
	TextureAndCount(Texture newtexture,GLuint Count){count = Count;texture = newtexture;};
};

class TextureCache
{
public:

    static Texture getTexture(std::string texturePath);
	static void lowerCount(std::string texturePath);
	static void lowerCount(GLuint textureID);
	static void deleteCache();
	
private:
    static std::map<std::string, TextureAndCount> textureMap;
};

class NormalCache
{
public:
    static Texture getTexture(std::string texturePath);
	static void lowerCount(std::string texturePath);
	static void lowerCount(GLuint textureID);
	static void deleteCache();
private:
    static std::map<std::string, TextureAndCount> normalMap;
};

class TextureLoader
{
	public:
	static Texture load(std::string filepath);
	static Texture loadNormal(std::string filepath);

private: static void loadTex();
};

class CubemapTexture
{
public:

    CubemapTexture(const std::string& Directory,
        const std::string& PosXFilename,
        const std::string& NegXFilename,
        const std::string& PosYFilename,
        const std::string& NegYFilename,
        const std::string& PosZFilename,
        const std::string& NegZFilename);

	~CubemapTexture(){};
	CubemapTexture(){}
    bool Load();
    void bind(GLuint unit = 0);
	void addFiles(std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz);
	void releaseCubemap();
private:

    std::string fileNames[6];
    GLuint ID;
	
	
};

class TextureAtlas
{
	GLint max_layers;
	GLuint ID;
	GLuint numLayers;
	TextureAtlas()
	{
		glGetIntegerv (GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		/* We require 1 byte alignment when uploading texture data */
	
		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void addTexture(std::vector<std::string> filepaths)
	{
		for(unsigned int i = 0; i < filepaths.size();i++)
		{
			int width,height,numComponents;
			printf("Loading texture %s\n",filepaths[i].c_str());
			char* data = (char*)stbi_load(filepaths[i].c_str(),&width,&height,&numComponents,4);
			if(data ==NULL)
			{
				printf("Couldn't load texture %s\nLoading Backup Texture\n",filepaths[i].c_str());
				data = (char*)stbi_load("res/Texture/white.png",&width,&height,&numComponents,4);
				if(data == NULL)
				{
					printf("Couldn't load backup texture");
					fatalError("Was not able to load basic texture");
				}
			}
		}
		//glTexSubImage2D(GL_TEXTURE_2D, 0, change, change, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

	}
	~TextureAtlas() 
	{
		glDeleteTextures(1, &ID);
	}
};