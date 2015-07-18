
#include "Maingame.h"

Maingame::Maingame(void)
{
	cfg = new ConfigFile("res/config.cfg");
}


Maingame::~Maingame(void)
{

}

void Maingame::init()
{
	__screenH = cfg->getValueOfKey<float>("height",480);
	__screenW = cfg->getValueOfKey<float>("width",640);
	gamestate.playing=true;
	gamestate.paused=false;
	maxFPS=60;
	ui = NULL;
	scene = NULL;
	window = new Window(__screenW,__screenH,"My Engine");
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));
	util.initGraphics();	
	createObjects();
	fpsLimiter.init(maxFPS); //using frame rater limiter since when do not have time dependent yet
	return ;
}

void Maingame::handleKeys()
{

	unsigned char key=NULL;
	input.update();
	while( SDL_PollEvent( &event ) != 0 ) //Eingaben kontrolieren
	{
		switch(event.type)
		{
			case SDL_QUIT://Fenster wird geschlossen
			{
				gamestate.playing=false;
			};break;
			case SDL_MOUSEMOTION:
			{
				input.setMouseCoords(event.motion.x,event.motion.y);
				scene->getCamera()->OnMouse(event.motion.x,event.motion.y);
				SDL_WarpMouseInWindow(window->GetSDLWindow(),__screenW /2,__screenH /2);	
			};break;
			case SDL_KEYUP:
			{
				input.releaseKey(event.key.keysym.sym);
			}break;
			case SDL_KEYDOWN:
			{
				input.pressKey(event.key.keysym.sym);
			}break;
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
						SDL_GetWindowSize(window->GetSDLWindow(),&__screenW,&__screenH);
						std::cout << __screenH << " " << __screenW<<std::endl;
						glViewport(0, 0, __screenW, __screenH);
						scene->getCamera()->updatePerspectiveMatrix(scene->getCamera()->getFov(),__screenW,__screenH,scene->getCamera()->getZ().x,scene->getCamera()->getZ().y);
						SDL_WarpMouseInWindow(window->GetSDLWindow(),__screenW /2,__screenH /2);	
					};break;
				}
 
			};break;
			case SDL_MOUSEBUTTONDOWN:
			{
				input.pressKey(event.button.button);
			};break;
			case SDL_MOUSEBUTTONUP:
			{
				input.releaseKey(event.button.button);
			};break;
		}
	}
	if(input.isKeyDown(SDLK_w))
	{
		scene->getCamera()->moveforward();
	}
	if(input.isKeyPressed(SDLK_RETURN))
	{
		scene->addObject("test","res/models/box.obj",scene->getCamera()->getPos() + Vector3(1.0f,1.0f,1.0f),Vector3(),Vector3(1.0f,1.0f,1.0f),"res/texture/white.png",Vector3(1.0f,1.0f,1.0f),"res/texture/normal_map.jpg");
	}
	if(input.isKeyDown(SDLK_s))
	{
		scene->getCamera()->movebackward();
	}
	if(input.isKeyDown(SDLK_q))
	{
		scene->getCamera()->raise();
	}
	if(input.isKeyDown(SDLK_e))
	{
		scene->getCamera()->sink();
	}
	if(input.isKeyDown(SDLK_a))
	{
		scene->getCamera()->strafeleft();
	}
	if(input.isKeyDown(SDLK_d))
	{
		scene->getCamera()->straferight();
	}
	if(input.isKeyDown(SDLK_ESCAPE))
	{
		gamestate.playing=false;
	}
	if(input.isKeyPressed(SDLK_1))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if(input.isKeyPressed(SDLK_2))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if(input.isKeyPressed(SDLK_UP))
	{
		scene->getCamera()->setFov(scene->getCamera()->getFov() + 1);
	}
	if(input.isKeyPressed(SDLK_DOWN))
	{
		scene->getCamera()->setFov(scene->getCamera()->getFov() - 1);
	}
	if(input.isKeyPressed(SDLK_F9))
	{
		static bool windowed = true;
		if(windowed)
		{
			window->SetFullScreen(true);
			windowed = false;
		}
		else
		{
			window->SetFullScreen(false);
			windowed = true;
		}
	}
	return;
}

void Maingame::update(float delta)
{
	scene->update(delta);
}

void Maingame::render()
{
	//Color buffer leer machen	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
	if(scene) scene->renderScene();
	if(ui) ui->draw();
	static std::string fps = "60";
	if(counter%60 == 0) fps = std::to_string((int)(1000/(start-end + 0.0001)));
	text::get().update(start-end);
	text::get().RenderText("FPS " + fps,890,0,100,30,Vector3(1,1,1));
	text::get().RenderText("Frametime " + std::to_string(start-end) + " ms",890,30,100,30,Vector3(1,1,1));
	text::get().RenderText("# Objects " + std::to_string(scene->getCount()),890,60,100,30,Vector3(1,1,1));
	text::get().RenderText("Time " + std::to_string(SDL_GetTicks()/1000),890,90,100,30,Vector3(1,1,1));
	text::get().RenderText("Dir " + std::to_string(scene->getCamera()->getDir().x) + " "
								  + std::to_string(scene->getCamera()->getDir().y) + " "  
								  +	std::to_string(scene->getCamera()->getDir().z ),890,120,100,30,Vector3(1,1,1));
	text::get().RenderText("Pos " + std::to_string(scene->getCamera()->getPos().x) + " "
								  + std::to_string(scene->getCamera()->getPos().y) + " "  
								  +	std::to_string(scene->getCamera()->getPos().z ),890,150,100,30,Vector3(1,1,1));
	window->SwapBuffers();

}
void Maingame::gameloop()
{
	SDL_StartTextInput(); //text eingabe aktivieren
	start = 0;
	end = SDL_GetTicks();
	counter  = 0;
	delta = 0;
	float frames = 1 / maxFPS * 1000;
	while( gamestate.playing )//Solange es nicht beended ist
	{ 
		start = SDL_GetTicks();
		
		
		delta+=(float)(start - end);
		while (delta >= frames) 
		{
			handleKeys();
			update(delta);
			delta -= frames;
		}
		
		render();	
		end = start;
		counter ++;
	}
	SDL_StopTextInput();	//Text Eingabe anhalten
	close(); //SDL beenden und Resourcen freigeben

	return;
}



void Maingame::close()
{
	scene->saveFile("res/Scenes/example.sc");
	delete(scene,ui,window);
	scene = NULL;
	ui = NULL;
	window = NULL;
	TextureCache::deleteCache();
	ModelCache::deleteCache();
	
	SDL_Quit();

	exit(0);
}

void Maingame::run()
{
	init();
	gameloop();
	close(); //SDL beenden
	return;
}

void Maingame::createObjects()
{
	
	scene = new Scene(__screenH,__screenW,"res/Scenes/example.sc");
	scene->getLightingCache()->addLight(AmbientLight(Vector3(0.2f,0.2f,0.2f)));
	scene->getLightingCache()->addLight( DirectionalLight(BaseLight(Vector3(1,0.9f,0.8f),0.8f),Vector3(1.0f,1.0f,0.2f)));
	scene->getLightingCache()->addLight(SpotLight(PointLight(Vector3(10,10,10),BaseLight(Vector3(1,1,1),1.f),Attenuation(1,29,64),1000),Vector3(1,1,0),0.1f));
	scene->getLightingCache()->addLight(SpotLight(PointLight(Vector3(10,100,10),BaseLight(Vector3(1,1,1),1.f),Attenuation(1,29,64),1000),Vector3(1,1,0),0.1f));
	ui = new UIrenderer();
	ui->addButton(Vector2(0,0),Vector2(100,100),Vector4(1,0,1,1),true,"","Button");
	
}

