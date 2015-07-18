#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL.h>
#include <string>
#include "Math/3DMath.h"
#include <GL/glew.h>
#include "Debugger.h"
#include "Errors.h"
class Window
{
public:
	Window(int width, int height, const std::string& title);
	virtual ~Window();
	
	void Update();
	void SwapBuffers();
	void BindAsRenderTarget() const;

	inline bool IsCloseRequested()          const { return isCloseRequested; }
	inline int GetWidth()                   const { return width; }
	inline int GetHeight()                  const { return height; }
	inline float GetAspect()                const { return (float)width/(float)height; }
	inline const std::string& GetTitle()    const { return title; }
	inline Vector2 GetCenter()             const { return Vector2((float)width/2.0f, (float)height/2.0f); }
	inline SDL_Window* GetSDLWindow()             { return window; }
	void SetFullScreen(bool value);
protected:
private:
	int           width,maxwidth;
	int           height,maxheight;
	std::string   title;
	SDL_Window*   window;
	SDL_GLContext glContext;
	bool          isCloseRequested;
	
	void operator=(const Window& other) {}
};

#endif
