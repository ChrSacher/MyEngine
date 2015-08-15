#pragma once

#include <glew.h>
#include <GL/GLu.h>
#include "Math/3DMath.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera3d.h"
#include "Text.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <SDL/SDL_events.h>

enum SE_ButtonType
{
	UP,
	LEFT,
	RIGHT,
	DOWN,
	RIGHTUP,
	LEFTUP,
	RIGHTDOWN,
	LEFTDOWN,
	CENTER
};//describes the way the button is from 2D Point
class SE_UIButton
{
public:
	SE_UIButton(Vector2 start,Vector2 size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text = "",SE_ButtonType type = LEFTDOWN);
	Vector2 start;
	Vector2 size;
	Vector2 rot;
	Vector4 color;
	Texture texture;
	std::string name;
	std::string text;
	SE_ButtonType type;
	int ID;
	static int IDCounter;
	bool render;
	bool operator<(SE_UIButton &other);
};

class UIrenderer
{
public:
	UIrenderer();
	~UIrenderer();
	Shader *shader;
	std::vector<SE_UIButton> buttons;
	void addButton(SE_UIButton& newbutton);
	void addButton(Vector2 Start,Vector2 Size,Vector4 Color,bool Render,std::string texturepath,std::string Name,std::string text = "",SE_ButtonType type = LEFTDOWN);
	void draw();
	Matrix4 ortho;
	void updateOrtho(float width,float height);
	GLuint vao;
	enum
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
	/*
		param Directory there place where the files are stored Example: res/texture/skybox/standard/
		param Rest invidual names
		Will load the skybox with Vetices Texture and Shader
	*/
   void loadSkybox(std::string Directory, std::string posx = "posx.png", std::string negx = "negx.png", std::string posy = "posy.png", std::string negy = "negy.png", std::string posz = "posz.png", std::string negz= "negz.png"); 
  /*
		param Directory there place where the files are stored Example: res/texture/skybox/standard/
		param Rest invidual names
		Will set the cube Texture
	*/
   void setSkyboxTexture(std::string Directory, std::string posx = "posx.png", std::string negx = "negx.png", std::string posy = "posy.png", std::string negy = "negy.png", std::string posz = "posz.png", std::string negz= "negz.png"); 
   /* Render The Skybox with its own Shader and matrices and textures*/
   void renderSkybox(); 
   /*Make Skybox and attach Camera to it and give it a color(use other than white for different shaded sky)*/
   Skybox(Camera3d *Camera =  new Camera3d(),Vector4 Color = Vector4(1,1,1,1));
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


struct TimeRay
{

	TimeRay(Ray r = Ray(Vector3(),Vector3()),float ttl = 0,GLuint off = 0){ray = r; TTL = ttl;offset = off;}
	Ray ray;
	float TTL;
	GLuint offset;
};
class LineRenderer
{
public:
	LineRenderer();
	~LineRenderer();
	/* 
	param pos start of Ray
	param pos2 end of Ray
	Will load Line into Buffer and render it on next render call
	*/
	GLuint addLine(Vector3 pos,Vector3 pos2);
	/* 
		param pos start of Ray
		param pos2 end of Ray
		param time time in second
		Will load Line into Buffer and render it on next render call after time in seconds have passed the line will be removed from the buffer
	*/
	GLuint addTimedLine(Vector3 pos,Vector3 pos2,float time);
	/*
		param camera Camera3d to render with
		Render lines in 3d with Camera viewProjection
	*/
	void render(Camera3d* camera);
	/*
		param timeinms time in ms which have passed
		Will update the timed line Buffer
	*/
	void update(float timeinms);
private:

	GLuint vab[2],vao[2];
	GLuint last;
	Shader* shader;
	std::map<GLuint, Ray> lines;
	std::map<GLuint,TimeRay> timedLines;
	GLuint lineCount;
	GLuint timedLineCount;
};





#pragma once



    class GUI
	{
    public:
        void init(const std::string& resourceDirectory);
        void destroy();
		GUI(){context = nullptr;root = nullptr; lastTime= 0;}
        void draw();
        void update(float delta);

        void setMouseCursor(const std::string& imageFile);
        void showMouseCursor();
        void hideMouseCursor();

        void onSDLEvent(SDL_Event& evnt);

        void loadScheme(const std::string& schemeFile);
        void setFont(const std::string& fontFile);
        CEGUI::Window* createWidget(const std::string& type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string& name = "");
        static void setWidgetDestRect(CEGUI::Window* widget, const Vector4& destRectPerc, const Vector4& destRectPix);

        // Getters
        static CEGUI::OpenGL3Renderer* getRenderer() { return renderer; }
        const CEGUI::GUIContext* getContext() { return context; }
    private:
        static CEGUI::OpenGL3Renderer* renderer;
        CEGUI::GUIContext* context ;
        CEGUI::Window* root ;
        unsigned int lastTime ;
    };