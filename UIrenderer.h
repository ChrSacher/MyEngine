#pragma once

#include <glew.h>
#include <GL/GLu.h>
#include "Math/3DMath.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera3d.h"
#include "Text.h"

class UIButton
{
public:
	UIButton(Vector2 start,Vector2 size,Vector4 Color,bool Render,std::string texturepath,std::string Name);
	Vector2 start;
	Vector2 size;
	Vector2 rot;
	Vector4 color;
	Texture texture;
	std::string name;
	int ID;
	static int IDCounter;
	bool render;
	bool operator<(UIButton &other);
};	
class UIrenderer
{
public:
	UIrenderer();
	~UIrenderer();
	Shader *shader;
	std::vector<UIButton> buttons;
	void addButton(UIButton& newbutton);
	void addButton(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name);
	void draw();
	Matrix4 ortho;
	void updateOrtho(float width,float height);
	GLuint vao;enum
	{
		POSITIONVB,
		TEXTUREVB,
		NUMBUFFERS
	};
	Text text;
	GLuint vab[NUMBUFFERS];
	void loadBuffer();
};

class Skybox 
{ 
public: 
   void loadSkybox(std::string Directory, std::string posx = "posx.png", std::string negx = "negx.png", std::string posy = "posy.png", std::string negy = "negy.png", std::string posz = "posz.png", std::string negz= "negz.png"); 
   void setSkyboxTexture(std::string Directory, std::string posx = "posx.png", std::string negx = "negx.png", std::string posy = "posy.png", std::string negy = "negy.png", std::string posz = "posz.png", std::string negz= "negz.png"); 
   void renderSkybox(); 
   Skybox(Camera3d &Camera = Camera3d(),Vector4 Color = Vector4(1,1,1,1));
   ~Skybox();
   void setCamera(Camera3d* Camera);
   void setColor(Vector4 Color);
   void setPos(Vector3 Pos);
   void setRot(Vector3 Rot);
   void setScale(Vector3 Scale);
private: 
	Shader* shader;
	Camera3d *camera;
	GLuint vao,vbo;
	CubemapTexture cube;
	Transform transform;
	Vector4 color;
};





