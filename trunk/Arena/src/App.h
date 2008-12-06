#ifndef _H_APP_H_
#define _H_APP_H_

#include "SDL/SDL.h"

class App{
public:
	App();
	virtual ~App(){}

	void ProcessEvent(SDL_Event* event);
	int  Run();

	virtual bool OnInit();
	virtual void OnLoop();
	virtual void OnRender();
	virtual void OnMouseMove(int x, int y);
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnMouseFocus();
	virtual void OnMouseBlur();
	virtual void OnMinimize();
	virtual void OnRestore();
	virtual void OnResize(int w, int h);
	virtual void OnUserEvent (int type, int code, void* data1, void* data2);
	virtual bool OnRawEvent(SDL_Event *);
	virtual void OnExpose(void);


	enum MouseButton{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

	virtual void OnMouseDown (MouseButton button, int x, int y);
	virtual void OnMouseUp   (MouseButton button, int x, int y);

	virtual void OnExit (void);
	virtual void OnCleanup (void);
protected:
	void Quit();
private:
	bool m_running;

};

App* GetApp();

#endif