#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL.h>
#include <string>
#include "Math/3DMath.h"
#include <GL/glew.h>
#include "Debugger.h"
#include "Errors.h"
#include "Camera3d.h"
#include "SpecComponent.h"

class Window : public ScriptComponentSystem::Listener
{
public:
	Window(int width, int height, const std::string& title);
	void ScriptCreated(SeScript* SeScript);
	class Listener
	{
	public:

		virtual ~Listener() { }

		/**
		* Handles when an camera settings change.
		*
		* @param camera The camera that was changed.
		*/
		virtual void windowChanged(Window* window) = 0;
	};
	void windowChanged();
	/**
	* Adds a camera listener.
	*
	* @param listener The listener to add.
	*/
	void addListener(Window::Listener* listener);

	/**
	* Removes a camera listener.
	*
	* @param listener The listener to remove.
	*/
	void removeListener(Window::Listener* listener);
	~Window();
	//not implemented
	void Update();
	//swaps Buffers
	void SwapBuffers();
	//bind this Window and render in it
	void BindAsRenderTarget() const;
	/*will handle window events
	  will return true if stuff has changed*/
	bool handle(SDL_Event& evnt);
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
	//disable copying because its breaks alot of shit
	void operator=(const Window& other) {}
	std::list<Window::Listener*> _listeners;
};

#endif
