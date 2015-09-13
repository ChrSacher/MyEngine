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
#include "ParticleSystem.h"
#include "UIrenderer.h"
#include "Shadow.h"
#include "Debugger.h"
#include "Scene.h"
#include "Window.h"
#include "config.h"
#include "Text.h"
#include "Audio.h"
#include "MusicPlayer.h"
#include "AudioListener.h"
#include "Engine.h"
#include "Component.h"
#include "Time.h"
struct GameState
{
	bool playing;
	bool paused;
	bool mainmenu;
	bool escapemenu;
	bool drawgrid;
	bool cameramove;
	bool ray;
};

class Maingame
{
	public:
		Maingame(void);
		~Maingame(void);

		//bool initGL();//matrizen initialisieren + farbe
		void handleKeys(); //Eingabe 
		void update(float delta);//Pro frame update
		void updateFrame(float delta);
		void render();//Objecte rendern
		void close(); //Schliessen von SDL + Resourcen
		void run();
		void gameloop();
		void init();
		void createObjects();
		void initShaders();
		void generateCommand(Command* command);
		void executeCommands();
		//funktionen
		

		//verschiede eingabenhandler für übersichtlichkeit
		void resizefull();
		void checkinput();

		//Ende von handler

		//setters

		
private:
		SDL_Event event;
		Window *window;
		GUI gui;
		UIrenderer *ui;
		GameState gamestate;
		FpsLimiter fpsLimiter;
		InputHandler input;
		Scene* scene;
		MusicPlayer* music;
		LineRenderer* line;
		int __screenW;
		int __screenH;
		float fps;
		float _time;
		float maxFPS;
		ConfigFile cfg;
		float delta;
		long int start, end;
		GLuint counter;
		std::vector<Command*> command_queue;
		std::vector<Command*> executedCommands;
		std::vector<Entity*> entities;
};

