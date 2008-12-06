#include "App.h"


App::App():m_running(true){
}

int App::Run(){
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1){
		return 1;
	}

	if(OnInit() == false)
		return 1;

	SDL_Event event;

	while(m_running){
		while(SDL_PollEvent(&event)) {
            ProcessEvent(&event);
        }
		OnLoop();
		OnRender();
	}

	OnCleanup();
	SDL_Quit();

	return 0;
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

void App::Quit (void){
	m_running = false;
}