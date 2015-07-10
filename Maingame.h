#pragma once


#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLu.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>


#include "Math/3DMath.h"
#include "InputManager.h"
#include "timing.h"
#include "Errors.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera3d.h"
#include <math.h>
#include "Material.h"
#include "Lighting.h"
#include "Object.h"
#include "ParticleSystem.h"
#include "UIrenderer.h"
#include "Shadow.h"
#include "Debugger.h"
#include "Scene.h"
//check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


struct GameState
{
	bool playing;
	bool paused;
	bool mainmenu;
	bool escapemenu;
	bool drawgrid;
	bool cameramove;
};

class Maingame
{
	public:
		Maingame(void);
		~Maingame(void);

		//bool initGL();//matrizen initialisieren + farbe
		void handleKeys(); //Eingabe 
		void update();//Pro frame update
		void render();//Objecte rendern
		void close(); //Schliessen von SDL + Resourcen
		void run();
		void gameloop();
		void init();
		void createObjects();
		void initShaders();
		//funktionen


		//verschiede eingabenhandler für übersichtlichkeit
		void resizefull();
		void checkinput();

		//Ende von handler

		//setters

		float fps;
		float _time;
		int __screenW,__oldScreenW;
		int __screenH,__oldScreenH;
		float maxFPS;
private:
		SDL_Event event;
		SDL_GLContext glContext;//OpenGL context
		SDL_Window* _window;
		UIrenderer *ui;
		GameState gamestate;
		FpsLimiter fpsLimiter;
		InputManager input;
		Scene* scene;

};

