
#include "Maingame.h"

Maingame::Maingame(void):cfg(std::string("res/config.cfg"))
{
}
void write(std::string r)
{
	std::cout << r << std::endl;
}

Maingame::~Maingame(void)
{
	//scene->saveFile("res/Scenes/test.sc");
	Scene::deleteScene(scene);
	for(unsigned int i = 0;i < entities.size();i++)
	{
		delete(entities[i]);
	}
	if(ui) delete(ui);
	if(line) delete(line);
	gui.destroy();

	util.~RenderUtil();
	Engine::shutDown();
	if(window) delete(window);
	for (unsigned int i = 0; i < executedCommands.size(); i++)
	{
		delete(executedCommands[i]);
	}
	SDL_Quit();
	
	

}

void Maingame::init()
{
	
	__screenH = cfg.getValueOfKey<int>("height",480);
	__screenW = cfg.getValueOfKey<int>("width",640);
	GameState::state.playing=true;
	GameState::state.paused=false;
	GameState::state.cameramove = true;
	GameState::state.ray = false;
	maxFPS=60;
	ui = NULL;
	scene = NULL;
	line = NULL;
	window = new Window(__screenW,__screenH,"My Engine");
	Engine::startUp();
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));
	util.initGraphics();	
	createObjects();
	fpsLimiter.init(maxFPS); 
	return ;
}

void Maingame::handleKeys()
{

	unsigned char key=NULL;
	InputHandler::get().update();
	while( SDL_PollEvent( &event ) != 0 ) //Eingaben kontrolieren
	{
		InputHandler::get().handle(event);
		if(window->handle(event)) 
		{
			scene->getCamera()->updateProjectionMatrix(scene->getCamera()->getFov(),window->GetWidth(),window->GetHeight(),scene->getCamera()->getZ().x,scene->getCamera()->getZ().y);
			__screenW = window->GetWidth();
			__screenH = window->GetHeight();
		}
		gui.onSDLEvent(event);
		switch(event.type)
		{
			case SDL_QUIT://Fenster wird geschlossen
			{
				GameState::state.playing=false;
			};break;
			case SDL_MOUSEMOTION:
			{
				if(GameState::state.cameramove)
				{
					//scene->getCamera()->OnMouse(event.motion.x,event.motion.y);
					
					
				}
				 
			};break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
			{
				bool alt = true;
				while (alt)
				{
					SDL_PollEvent(&event);
					if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) alt = false;
				};
			}; break;
		}
	}
	InputHandler::get().generate_input(command_queue);
	if (GameState::state.ray && InputHandler::get().isKeyPressed(1))
	{
		scene->pick(InputHandler::get().getMouseCoords().x, InputHandler::get().getMouseCoords().y);

	}


	if(InputHandler::get().isKeyPressed(SDLK_F3)) generateCommand(new PlayMusic(music));
	if(InputHandler::get().isKeyPressed(SDLK_RIGHT)) generateCommand(new NextMusic(music));
	if(InputHandler::get().isKeyPressed(SDLK_LEFT)) generateCommand(new PreviousMusic(music));
	if(InputHandler::get().isKeyPressed(SDLK_1)) generateCommand(new switchRender());

	if(InputHandler::get().isKeyPressed(SDLK_RETURN))
	{
		generateCommand(new SceneAddObject(scene));
		
	}
	if (InputHandler::get().isKeyPressed(SDLK_h))
	{
		scene->getCamera()->setScript("res/Scripts/Camera.chai");
	}
	if(InputHandler::get().isKeyPressed(SDLK_F9))
	{
			generateCommand(new WindowFullScreen(window));
	}
	if(InputHandler::get().isKeyDown(SDLK_ESCAPE))
	{
		GameState::state.playing=false;
	}
	if(InputHandler::get().isKeyPressed(SDLK_SPACE))
	{
		static bool windowed = true;
		if(windowed)
		{
			windowed = false;
			GameState::state.cameramove = false;
			GameState::state.ray = true;
			gui.showMouseCursor();
		}
		else
		{
			windowed = true;
			GameState::state.cameramove = true;
			gui.hideMouseCursor();
			GameState::state.ray = false;
		}
	}
	if(GameState::state.cameramove) SDL_WarpMouseInWindow(window->GetSDLWindow(), __screenW / 2, __screenH / 2);
	executeCommands();
	return;

}

void Maingame::update()
{
	scene->update();
	music->update();
	Engine::update();
	gui.update();
}


void Maingame::render()
{
	//Color buffer leer machen	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	gui.draw();
	
	if(line) line->render(scene->getCamera());
	if(scene) scene->renderScene();
	if(ui) ui->draw();
	
	static std::string fps = "60";
	if(Time::counter%60 == 0) fps = std::to_string((int)(1/(Time::delta + 0.0001)));
	ServiceLocator::getText().renderText("FPS " + fps,890,0,100,30,Vector3(1,1,1));
	ServiceLocator::getText().renderText("Frametime " + std::to_string(Time::delta) + " ms",890,30,100,30,Vector3(1,1,1));
	ServiceLocator::getText().renderText("# Objects " + std::to_string(scene->getEntityDrawCount()),890,60,100,30,Vector3(1,1,1));
	ServiceLocator::getText().renderText("Time " + std::to_string(SDL_GetTicks()/1000),890,90,100,30,Vector3(1,1,1));
	std::string temp = std::to_string(music->getSongNumber()) + " " +  music->getCurrentTitle();
	ServiceLocator::getText().renderText(temp,convertSTT(__screenW,__screenW) - 155.0f,convertSTT(__screenW,__screenW) - 30.0f,140.0f,30.0f,Vector3(1,1,1));
	if(GameState::state.ray) gui.draw();
	window->SwapBuffers();

}
void Maingame::gameloop()
{
	SDL_StartTextInput(); //text eingabe aktivieren
	
	float mainDelta = 0;
	Time::begin(maxFPS);
	while( GameState::state.playing )//Solange es nicht beended ist
	{ 
		Time::startFrame();
		handleKeys();
		mainDelta += Time::delta;

		if (!GameState::state.paused && (mainDelta > 1 / maxFPS))
		{
			mainDelta -= 1/maxFPS;
			update();
		}
		render();	
		Time::endFrame();
	}
	SDL_StopTextInput();	//Text Eingabe anhalten
	close(); //SDL beenden und Resourcen freigeben

	return;
}



void Maingame::close()
{
	GameState::state.playing = false;
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
	
	scene = Scene::createScene(__screenH,__screenW,"res/Scenes/test.sc");
	//scene->saveFile("res/Scenes/test.sc");
	AudioListener::getInstance()->setCamera(scene->getCamera());
	auto it  = scene->getEntityVector();
	entities.insert(entities.begin(),it.begin(),it.end());
	ui = new UIrenderer();
	line = new LineRenderer();
	//ui->addOverlay(Vector2(0,0),Vector2(100,100),Vector4(1,0,1,1),true,"","Button","Text",RIGHTUP);
	music = new MusicPlayer("res/Sound/Music/Music.mf");
	gui.init("res/GUI");
	gui.loadScheme("TaharezLook.scheme");
    gui.setFont("Jura-13");
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(gui.createWidget("TaharezLook/Button", Vector4(0.5f, 0.5f, 0.1f, 0.05f), Vector4(), "TestButton"));
    testButton->setText("Hello World!");
    CEGUI::Combobox* TestCombobox = static_cast<CEGUI::Combobox*>(gui.createWidget("TaharezLook/Combobox", Vector4(0.2f, 0.2f, 0.1f, 0.05f), Vector4(), "TestCombobox"));
	CEGUI::Listbox* listbox = static_cast<CEGUI::Listbox*>(gui.createWidget("TaharezLook/Listbox",Vector4(0.0f,0.7f,0.3f,0.3f),Vector4(),"test"));
	CEGUI::ListboxItem* listboxi = new CEGUI::ListboxTextItem("text",0);
	listboxi->setSelectionColours(CEGUI::ColourRect(CEGUI::Colour(1.0f,0.5f,0.5f,1.0f)));
	listbox->addItem(listboxi);
	listbox->addItem(new CEGUI::ListboxTextItem("text22",1));
    gui.setMouseCursor("TaharezLook/MouseArrow");
	ServiceLocator::getCM().addScriptListener(window);
	scene->getCamera()->setScript("res/Scripts/Camera.chai");
}

void Maingame::generateCommand(Command* command)
{
	command_queue.push_back(command);
	//should be read from File
}
void Maingame::executeCommands()
{
	while (!command_queue.empty()) 
	{
        command_queue.back()->execute();
		executedCommands.push_back(command_queue.back());
		if(executedCommands.size() > 50) executedCommands.erase(executedCommands.begin());
        command_queue.pop_back();
	}
}