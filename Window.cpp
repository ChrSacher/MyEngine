#include "Window.h"




Window::Window(int width, int height, const std::string& title) :
	 width(width),
	 height(height),
	 title(title),
	 isCloseRequested(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	
//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	bool DebugMode = true;
	if(DebugMode)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	}
	//Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(SDL_FALSE); 	
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


	 window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
        fatalError("SDL Window could not be created!");
    }
	glContext = SDL_GL_CreateContext( window);
	if (glContext == nullptr) 
	{
        fatalError("SDL_GL context could not be created!");
    }
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_GL_SetSwapInterval(1);

	//Apparently this is necessary to build with Xcode
	glewExperimental = GL_TRUE;
	
	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}
	SwapBuffers();
}

Window::~Window()
{
	SDL_GL_DeleteContext( glContext);
	SDL_DestroyWindow( window);
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow( window);
}

void Window::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}
void Window::SetFullScreen(bool value)
{
	int mode = 0;
	if(value)
	{
		mode = SDL_WINDOW_FULLSCREEN_DESKTOP ;
	}
	else
	{
		mode = 0;
	}
	SDL_SetWindowFullscreen( window, mode);
}