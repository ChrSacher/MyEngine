
#include "Maingame.h"


Maingame::Maingame(void)
{
	__screenH = 480;
	__screenW = 640;
	gamestate.playing=true;
	gamestate.paused=false;
	maxFPS=60;
}


Maingame::~Maingame(void)
{

}

void Maingame::init()
{
	bool DebugMode = true;
	if(DebugMode)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	}
	//Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4); //4x multisampling     weiß nicht warum aber das muss vor create window gemacht werden obwohl es anderes vorgeschrieben wir
			
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	if (glDebugMessageCallbackARB != NULL && DebugMode) 
	{

		float i;
		glGetFloatv(GL_MAJOR_VERSION,&i);
		if(i >= 4.0f)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(Core::DebugOutput::myCallback, NULL);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DONT_CARE, 0, NULL, GL_TRUE);
		}
		else
		{
			glEnable(GL_ARB_debug_output);
			glDebugMessageCallback(Core::DebugOutput::myCallback, NULL);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DONT_CARE, 0, NULL, GL_TRUE);
		};
	}
    //Open an SDL window
    _window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, __screenW, __screenH, SDL_WINDOW_OPENGL);
    if (_window == nullptr)
	{
        fatalError("SDL Window could not be created!");
    }

    //Set up our OpenGL context
	glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr) 
	{
        fatalError("SDL_GL context could not be created!");
    }

    //Set up glew (optional but recommended)
	glewExperimental = GL_TRUE;
    GLenum error = glewInit();
	
    if (error != GLEW_OK)
	{
        fatalError("Could not initialize glew!");
    }
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));
    //Tell SDL that we want a double buffered window so we dont get
    //any flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set VSYNC
    SDL_GL_SetSwapInterval(1);

	SDL_ShowCursor(SDL_FALSE); 
	util.initGraphics();
	SDL_GL_SwapWindow(_window);
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
				SDL_WarpMouseInWindow(_window,__screenW /2,__screenH /2);	
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
						__oldScreenW = __screenW;
						__oldScreenH = __screenH;
						__screenW = event.window.data1;
						__screenH = event.window.data2 ;
						glViewport(0, 0, __screenW, __screenH);
						scene->getCamera()->updatePerspectiveMatrix(90.0f,__screenW,__screenH,scene->getCamera()->getZ().x,scene->getCamera()->getZ().y);
					};break;
					case SDL_WINDOWEVENT_MAXIMIZED:
					{
						SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						
					};break;
					case SDL_WINDOWEVENT_RESTORED:
					{
						
							SDL_SetWindowFullscreen(_window, SDL_FALSE);
						
							
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
		scene->addObject("test","models/box.obj",scene->getCamera()->getPos() + Vector3(1,1,1));
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
			SDL_MaximizeWindow(_window);
			windowed = false;
		}
		else
		{
			SDL_RestoreWindow(_window);
			windowed = true;
		}
	}
	return;
}

void Maingame::update()
{
	scene->update();
}

void Maingame::render()
{
	//Color buffer leer machen	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	scene->renderScene();
	ui->draw();
	//shader->emptyBatch(); //currently optimizing
	SDL_GL_SwapWindow(_window);

}
void Maingame::gameloop()
{
	SDL_StartTextInput(); //text eingabe aktivieren
	long int start = 0, end = SDL_GetTicks();
	float delta = 0;
	float frames = 1 / maxFPS * 1000;
	while( gamestate.playing )//Solange es nicht beended ist
	{ 
		start = SDL_GetTicks();
		handleKeys();

		delta+=(float)(start - end);
		while (delta >= frames) 
		{
			
			update();
			delta -= frames;
		}
		
		render();	
		end = start;
	}
	SDL_StopTextInput();	//Text Eingabe anhalten
	close(); //SDL beenden und Resourcen freigeben

	return;
}



void Maingame::close()
{
	delete(scene,ui);
	TextureCache::deleteCache();
	ModelCache::deleteCache();
	scene->saveFile("Scenes/example.sc");
	SDL_DestroyWindow( _window);
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	_window = NULL;

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
	
	scene = new Scene(__screenH,__screenW,"Scenes/example.sc");
	scene->getLightingCache()->addLight(AmbientLight(Vector3(0.2f,0.2f,0.2f)));
	scene->getLightingCache()->addLight( DirectionalLight(BaseLight(Vector3(1,0.9f,0.8f),0.8f),Vector3(1.0f,0.3f,0.2f)));
	/*scene->addObject("box","models/box.obj",Vector3(0.0f,0.0f,0.0f),Vector3(0.0f,0.0f,0.0f),Vector3(1.0f,1.0f,1.0f),"",Vector3(1,1,1));
	scene->addObject("box","models/box.obj",Vector3(0.0f,3.0f,3.0f),Vector3(0.0f,0.0f,0.0f),Vector3(1.0f,1.0f,1.0f),"",Vector3(1,1,1));
	scene->addObject("box","models/box.obj",Vector3(0.0f,1.5f,1.5f),Vector3(0.0f,0.0f,0.0f),Vector3(1.0f,1.0f,1.0f),"",Vector3(0,0,1));
	scene->addObject("box","models/box.obj",Vector3(0,-4,0),Vector3(0.0f,0.0f,0.0f),Vector3(1.0f,1.0f,1.0f),"",Vector3(1,0,1));
	for(unsigned int i = 0; i < 20 ;i++)
	{
		scene->addObject("box","models/box.obj",Vector3(0,-4,0),Vector3(0.0f,0.0f,0.0f),Vector3(1.0f,1.0f,1.0f),"",Vector3(1,0,1));
	}
	scene->addLight(PointLight(Vector3(0,0,0),BaseLight(Vector3(1,0,0),2.0f),Attenuation(0,0,1),100));
	scene->addFog(Fog(0.05f,Vector4(0.5f,0.5f,0.5f,0.5f),400,500,false));*/
	scene->getLightingCache()->addLight(SpotLight(PointLight(Vector3(0,10,1),BaseLight(Vector3(1,0,1),1),Attenuation(1,0,0),10),Vector3(0,1,0),0.2f));
	ui = new UIrenderer();
	ui->addButton(UIButton(Vector2(0,0),Vector2(50,50),Vector4(1,0,0,1),false,""));
	ui->addButton(UIButton(Vector2(50,50),Vector2(50,50),Vector4(0,0,1,0.1f),true,""));
	
}

