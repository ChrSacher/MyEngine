﻿#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"


std::map<std::string, TextureAndCount> TextureCache::textureMap;
std::map<std::string, TextureAndCount> NormalCache::normalMap;

Texture::Texture(std::string path)
{
	*this = TextureCache::getTexture(path);
	bind();
}

void Texture::addTexture(std::string path)
{
	TextureCache::lowerCount(ID);
	*this = TextureCache::getTexture(path);
	bind();
}


Texture::~Texture(void)
{
}

void Texture::bind(int unit)  const
{
	if(unit < 0 || unit > 30) fatalError("Texture unit is too large/too low"); //opengl only has units between 0 - 31
	util.bindTexture(ID,unit);
}


void Texture::drawTexture(bool check)  const
{
	if(check)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{		
		glDisable(GL_TEXTURE_2D);
	}
}



Texture TextureCache::getTexture(const std::string &texturePath) 
{

    //lookup the texture and see if its in the map
    auto mit = textureMap.find(texturePath);
    
    //check if its not in the map
    if (mit == textureMap.end()) 
	{
        //Load the texture
		 Texture newTexture = TextureLoader::load(texturePath);
		 
        //Insert it into the map
		textureMap.insert(make_pair(texturePath, TextureAndCount(newTexture,1)));

        return newTexture;
    }
	mit->second.count ++;
    return mit->second.texture;
}

Texture NormalCache::getTexture(const std::string &texturePath) 
{

    //lookup the texture and see if its in the map
    auto mit = normalMap.find(texturePath);
    
    //check if its not in the map
    if (mit == normalMap.end()) 
	{
        //Load the texture
		 Texture newTexture = TextureLoader::loadNormal(texturePath);
		 
        //Insert it into the map
		normalMap.insert(make_pair(texturePath, TextureAndCount(newTexture,1)));

        return newTexture;
    }
	mit->second.count ++;
    return mit->second.texture;
}

Texture TextureLoader::load(std::string filepath)
{
	Texture texture;
	int width,height,numComponents;
	printf("Loading texture %s\n",filepath.c_str());
	char* data = (char*)stbi_load(filepath.c_str(),&width,&height,&numComponents,4);
	texture.texturepath=filepath.c_str();
	if(data ==NULL)
	{
		printf("Couldn't load texture %s\nLoading Backup Texture\n",filepath.c_str());
		data = (char*)stbi_load("res/Texture/white.png",&width,&height,&numComponents,4);

		texture.texturepath="res/Texture/white.png";
		if(data == NULL)
		{
			printf("Couldn't load backup texture");
			fatalError("Was not able to load basic texture");
		}
	}
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16); //anisotropy filtering for better quality but more workload
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,data); //mipmapping
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	texture.width=width;
	texture.height=height;
	stbi_image_free(data);
	
	return texture;
}

Texture TextureLoader::loadNormal(std::string filepath)
{
	Texture texture;
	int width,height,numComponents;
	printf("Loading normalMap %s\n",filepath.c_str());
	char* data = (char*)stbi_load(filepath.c_str(),&width,&height,&numComponents,4);
	texture.texturepath=filepath.c_str();
	if(data ==NULL)
	{
		std::cout << "Couldn't load normalMap" << filepath.c_str() << "\n Loading Backup normalMap\n";
		data = (char*)stbi_load("res/Texture/normal_up.jpg",&width,&height,&numComponents,4);
		texture.texturepath="res/Texture/normal_up.jpg";
		if(data == NULL)
		{
			fatalError("Was not able to load basic normalMap\n");
		}
	}
	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16); //anisotropy filtering for better quality but more workload
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,data); //mipmapping
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	texture.width=width;
	texture.height=height;
	stbi_image_free(data);
	
	return texture;
}

void TextureCache::lowerCount(const std::string &texturePath)
{
	auto mit = textureMap.find(texturePath);
    
    //check if its not in the map
    if (mit == textureMap.end()) 
	{
       std::cout << "Cannot lower count on non existing texture path "<< texturePath << std::endl;
		return;
    }
	mit->second.count --;
	if(mit->second.count < 1) //erase Texture if noone is using it
	{
		mit->second.texture.releaseTexture();
		textureMap.erase(mit);
	}
	return;
}

void TextureCache::lowerCount(const GLuint &textureID)
{
	for (std::map<std::string,TextureAndCount>::iterator &mit = textureMap.begin(); mit != textureMap.end(); ++mit)
	{
		if(mit->second.texture.ID == textureID)
		{
			mit->second.count -- ;
			if(mit->second.count < 1) //release Texture if it is not used
			{
				mit->second.texture.releaseTexture();
				textureMap.erase(mit);
			}
			return;
		};
	};
	std::cout<<"couldn't find Texture ID "<<textureID<<std::endl;
}

void NormalCache::lowerCount(const std::string &texturePath)
{
	auto mit = normalMap.find(texturePath);
    
    //check if its not in the map
    if (mit == normalMap.end()) 
	{
       std::cout << "Cannot lower count on non existing texture path "<< texturePath << std::endl;
		return;
    }
	mit->second.count --;
	if(mit->second.count < 1) //erase Texture if noone is using it
	{
		mit->second.texture.releaseTexture();
		normalMap.erase(mit);
	}
	return;
}

void NormalCache::lowerCount(const GLuint &textureID)
{
	for (std::map<std::string,TextureAndCount>::iterator mit = normalMap.begin(); mit != normalMap.end(); ++mit)
	{
		if(mit->second.texture.ID == textureID)
		{
			mit->second.count -- ;
			if(mit->second.count < 1) //release Texture if it is not used
			{
				mit->second.texture.releaseTexture();
				normalMap.erase(mit);
			}
			return;
		};
	};
	std::cout<<"couldn't find Texture ID "<<textureID<<std::endl;
}

bool CubemapTexture::Load()
{
	
	if(ID > 0.1)
	{
		util.unbindTexture(ID);
		glDeleteTextures(1,&ID);  //if there allready is a cubemap replace it
	}
    glGenTextures(1, &ID);
	glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	GLenum types[6];
	types[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	types[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	types[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	types[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	types[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	types[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    for (unsigned int i = 0 ; i < 6 ; i++) 
	{
		int width,height,numComponents;
			std::cout << "Loading Cube Texture" <<fileNames[i]<<std::endl;
		char* data = (char*)stbi_load(fileNames[i].c_str(),&width,&height,&numComponents,4);
        if(data ==NULL)
		{
			std::cout << "Couldn't load Cube Texture" <<fileNames[i]<<std::endl;
			data = (char*)stbi_load("res/Texture/white.png",&width,&height,&numComponents,4);
			if(data == NULL)
			{
				fatalError("Was not able to load basic texture\n");
				return false;
			}
		}
		glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
    }
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	bind();


    return true;
}

void CubemapTexture::bind(GLuint unit)
{
	if(unit < 0 || unit > 32)
	{
			fatalError("Texture unit is too large/too low");
			return;
	}
	util.bindTexture(ID,unit,GL_TEXTURE_CUBE_MAP); 
}

void CubemapTexture::addFiles(std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz)
{
	fileNames[0] =posx;
	fileNames[1] = negx;
	fileNames[2] =  posy;
	fileNames[3] = negy;
	fileNames[4] =  posz;
	fileNames[5] = negz;
}

CubemapTexture::CubemapTexture( const std::string& PosXFilename, const std::string& NegXFilename,
							   const std::string& PosYFilename, const std::string& NegYFilename,  const std::string& PosZFilename, const std::string& NegZFilename)

{
	addFiles(PosXFilename,NegXFilename,PosYFilename,NegYFilename,PosZFilename, NegZFilename);
	Load();
}
void CubemapTexture::releaseCubemap()
{
	glDeleteTextures(1,&ID);
}


void TextureCache::deleteCache()
{
	for (std::map<std::string,TextureAndCount>::iterator it = textureMap.begin(); it != textureMap.end(); ++it)
	{
		it->second.texture.releaseTexture();
	}	
	textureMap.clear();
}

void NormalCache::deleteCache()
{
	for (std::map<std::string,TextureAndCount>::iterator it = normalMap.begin(); it != normalMap.end(); ++it)
	{
		it->second.texture.releaseTexture();
	}	
	normalMap.clear();
}

void Texture::releaseTexture()
{
	util.unbindTexture(ID);
	glDeleteTextures(1,&ID);
}

