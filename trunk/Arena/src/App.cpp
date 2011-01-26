#include "App.h"
#include <cassert>
#include <iostream>


App::App():m_running(true){
}

int App::Run(){
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1){
		return 1;
	}

	if(OnInit() == false)
		return 1;

	PickDrawFunction();

	SDL_Event event;
	m_loops = 0;

	while(m_running){
		while(SDL_PollEvent(&event)) {
            ProcessEvent(&event);
        }
		OnLoop();
		OnRender();
		m_loops++;
		if(m_loops % 100 == 0){
		    double loops_per_tick = (double)m_loops / (double)SDL_GetTicks();
		    
		    std::cout << loops_per_tick << " loops per tick" << std::endl;
		}

	}

	OnCleanup();
	SDL_Quit();

	return 0;
}

void App::PickDrawFunction(){
	SDL_Surface * screen = GetScreen();

	switch(screen->format->BitsPerPixel){
		case 8:
			m_draw_pixel = &App::DrawPixel8;
			break;
		case 16:
			m_draw_pixel = &App::DrawPixel16;
			break;
		case 24:
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
				m_draw_pixel = &App::DrawPixel24_LittleEndian;
			else
				m_draw_pixel = &App::DrawPixel24_BigEndian;
			break;
		case 32:
			m_draw_pixel = &App::DrawPixel32;
			break;
		default:
			assert(0);
	}
}

void App::ProcessEvent(SDL_Event *pEvent){
	if(!OnRawEvent(pEvent)){
		switch(pEvent->type) {
		case SDL_ACTIVEEVENT: {
			switch(pEvent->active.state) {
		case SDL_APPMOUSEFOCUS: {
			if ( pEvent->active.gain )   OnMouseFocus();
			else                OnMouseBlur();

			break;
								}
		case SDL_APPACTIVE: {
			if ( pEvent->active.gain )   OnRestore();
			else                OnMinimize();

			break;
							}
			}
			break;
							  }

		case SDL_KEYDOWN: {
			OnKeyDown(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
						  }

		case SDL_KEYUP: {
			OnKeyUp(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
						}

		case SDL_MOUSEMOTION: {
			OnMouseMove(pEvent->motion.x,pEvent->motion.y);
			break;
							  }

		case SDL_MOUSEBUTTONDOWN: {
			switch(pEvent->button.button) {
		case SDL_BUTTON_LEFT: {
			OnMouseDown(MOUSE_LEFT,pEvent->button.x,pEvent->button.y);
			break;
							  }
		case SDL_BUTTON_RIGHT: {
			OnMouseDown(MOUSE_RIGHT,pEvent->button.x,pEvent->button.y);
			break;
							   }
		case SDL_BUTTON_MIDDLE: {
			OnMouseDown(MOUSE_MIDDLE,pEvent->button.x,pEvent->button.y);
			break;
								}
			}
			break;
								  }

		case SDL_MOUSEBUTTONUP: {
			switch(pEvent->button.button) {
		case SDL_BUTTON_LEFT: {
			OnMouseUp(MOUSE_LEFT,pEvent->button.x,pEvent->button.y);
			break;
							  }
		case SDL_BUTTON_RIGHT: {
			OnMouseUp(MOUSE_RIGHT,pEvent->button.x,pEvent->button.y);
			break;
							   }
		case SDL_BUTTON_MIDDLE: {
			OnMouseUp(MOUSE_MIDDLE,pEvent->button.x,pEvent->button.y);
			break;
								}
			}
			break;
								}
#if 0
		case SDL_JOYAXISMOTION: {
			OnJoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value);
			break;
								}

		case SDL_JOYBALLMOTION: {
			OnJoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel);
			break;
								}

		case SDL_JOYHATMOTION: {
			OnJoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value);
			break;
							   }
		case SDL_JOYBUTTONDOWN: {
			OnJoyButtonDown(Event->jbutton.which,Event->jbutton.button);
			break;
								}

		case SDL_JOYBUTTONUP: {
			OnJoyButtonUp(Event->jbutton.which,Event->jbutton.button);
			break;
							  }
#endif

		case SDL_QUIT: {
			OnExit();
			break;
					   }

		case SDL_SYSWMEVENT: {
			//Ignore
			break;
							 }

		case SDL_VIDEORESIZE: {
			OnResize(pEvent->resize.w,pEvent->resize.h);
			break;
							  }

		case SDL_VIDEOEXPOSE: {
			OnExpose();
			break;
							  }

		default: {
			OnUserEvent(pEvent->user.type,pEvent->user.code,pEvent->user.data1,pEvent->user.data2);
			break;
				 }
		}
	}
}

bool App::OnInit(){

	return false;
}

void App::OnLoop(){
}

void App::OnCleanup(){

}

void App::OnRender(){
}

void App::OnMouseMove(int x, int y){
}

void App::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
}

void App::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode){
}

void App::OnMouseFocus(){
}

void App::OnMouseBlur(){
}

void App::OnMinimize(){
}

void App::OnRestore(){
}

void App::OnResize(int w, int h){
}

void App::OnUserEvent (int type, int code, void* data1, void* data2){
}

bool App::OnRawEvent(SDL_Event *){
	return false;
}

void App::OnExpose(void){
}


void App::OnMouseDown (MouseButton button, int x, int y){
}

void App::OnMouseUp   (MouseButton button, int x, int y){
}


void App::OnExit (void){
}

void App::DrawPixel8(SDL_Surface* screen,Uint32 color, int x, int y){
	Uint8 *bufp;

	bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
	*bufp = color;
}

void App::DrawPixel16(SDL_Surface* screen,Uint32 color, int x, int y){
	Uint16 *bufp;

	bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
	*bufp = color;
}

void App::DrawPixel24_LittleEndian(SDL_Surface* screen,Uint32 color, int x, int y){
	Uint8 *bufp;
	bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;

	bufp[0] = color;
	bufp[1] = color >> 8;
	bufp[2] = color >> 16;	
}

void App::DrawPixel24_BigEndian(SDL_Surface* screen,Uint32 color, int x, int y){
	Uint8 *bufp;
	bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
	bufp[2] = color;
	bufp[1] = color >> 8;
	bufp[0] = color >> 16;
}

void App::DrawPixel32(SDL_Surface* screen,Uint32 color, int x, int y){
	Uint32 *bufp;

	bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
	*bufp = color;
}

void App::DrawPixel(SDL_Surface* screen,Uint32 color, int x, int y){
	if(x>0 && y>0 && x < screen->w && y < screen->h)
		(this->*m_draw_pixel)(screen,color,x,y);
}

void App::DrawSquare(SDL_Surface* screen,Uint32 color, int x, int y, int size){
	int top = y - size/2;
	int left = x - size/2;
	for (int i = 0; i < size; i++) {
		DrawPixel(screen,color,left+i,top); //Top
		DrawPixel(screen,color,left,top+i); //Left
		DrawPixel(screen,color,left+i,top+size); //Bottom
		DrawPixel(screen,color,left+size,top+i); //Right
	}
}

void App::Log(const char*str)
{
#ifndef NDEBUG
    std::cout << str << std::endl;
#endif
}

void App::Quit (void){
	m_running = false;
}