#include "Game.h"
#include "Defs.h"

bool Game::OnInit(){
	m_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF  ); 
	return true;
}

void Game::OnLoop(){
}

void Game::OnRender(){
	// Lock surface if needed
  if (SDL_MUSTLOCK(m_screen)) 
    if (SDL_LockSurface(m_screen) < 0) 
      return;

  // Ask SDL for the time in milliseconds
  int tick = SDL_GetTicks();

  // Declare a couple of variables
  int i, j, yofs, ofs;

  // Draw to screen
  yofs = 0;
  for (i = 0; i < SCREEN_HEIGHT; i++)
  {
    for (j = 0, ofs = yofs; j < SCREEN_WIDTH; j++, ofs++)
    {
      ((unsigned int*)m_screen->pixels)[ofs] = i * i + j * j + tick;
    }
    yofs += m_screen->pitch / 4;
  }

  // Unlock if needed
  if (SDL_MUSTLOCK(m_screen)) 
    SDL_UnlockSurface(m_screen);

  // Tell SDL to update the whole screen
  SDL_UpdateRect(m_screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
}

void Game::OnMouseMove(int x, int y){
}

void Game::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
}

void Game::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode){
}

void Game::OnMouseFocus(){
}

void Game::OnMouseBlur(){
}

void Game::OnMinimize(){
}

void Game::OnRestore(){
}

void Game::OnResize(int w, int h){
}

void Game::OnUserEvent (int type, int code, void* data1, void* data2){
}



void Game::OnExpose(void){
}


void Game::OnMouseDown (MouseButton button, int x, int y){
}

void Game::OnMouseUp   (MouseButton button, int x, int y){
}


void Game::OnExit (void){
	Quit();
}


void Game::OnCleanup (void){
}



App * GetApp(){
	static Game game;
	return &game;
}