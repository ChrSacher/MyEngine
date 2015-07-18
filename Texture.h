#pragma once
#include <map>


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
	Texture(){ID = 0;}
	Texture(std::string path);
	
	virtual ~Texture(void);
	void bind(int unit = 0) const;
	GLuint ID;
	int width,height;
	std::string texturepath;
	void drawTexture(bool check) const; //disables or enables Textures in the engine
	void addTexture(std::string path); //replaces current Texture with path
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