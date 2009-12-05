#ifndef _H_BBA_GAME_H_
#define _H_BBA_GAME_H_

#include "App.h"
#include "Defs.h"

class Game : public App{
public:
	Game();
	virtual ~Game();
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
	virtual void OnExpose(void);

	virtual void OnMouseDown (MouseButton button, int x, int y);
	virtual void OnMouseUp   (MouseButton button, int x, int y);

	virtual void OnExit (void);
	virtual void OnCleanup (void);
protected:
	virtual SDL_Surface * GetScreen()const{
		return m_screen;
	}

private:
	SDL_Surface* m_screen;
	QTNode m_quadtree;
};

#endif