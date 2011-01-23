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
	
	virtual void Log(const char*str);

protected:
	void Quit();
	void PickDrawFunction();
	void DrawPixel8(SDL_Surface*,Uint32 color, int x, int y);
	void DrawPixel16(SDL_Surface*,Uint32 color, int x, int y);
	void DrawPixel24_LittleEndian(SDL_Surface*,Uint32 color, int x, int y);
	void DrawPixel24_BigEndian(SDL_Surface*,Uint32 color, int x, int y);
	void DrawPixel32(SDL_Surface*,Uint32 color, int x, int y);
	void DrawPixel(SDL_Surface*,Uint32 color,int x, int y);
	void DrawSquare(SDL_Surface*,Uint32 color, int x, int y, int size);
	virtual SDL_Surface * GetScreen()const=0;
private:
	typedef void (App::*DrawPixelFunc)(SDL_Surface*, Uint32 color, int x, int y);
	DrawPixelFunc m_draw_pixel;
	bool m_running;

};

App* GetApp();

#define LOG(x) GetApp()->Log(x)

#endif