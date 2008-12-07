#include "SDL/SDL.h"
#include "Defs.h"
#include "App.h"

void render(SDL_Surface * screen){
  // Lock surface if needed
  if (SDL_MUSTLOCK(screen)) 
    if (SDL_LockSurface(screen) < 0) 
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
      ((unsigned int*)screen->pixels)[ofs] = i * i + j * j + tick;
    }
    yofs += screen->pitch / 4;
  }

  // Unlock if needed
  if (SDL_MUSTLOCK(screen)) 
    SDL_UnlockSurface(screen);

  // Tell SDL to update the whole screen
  SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
}


int main( int argc, char* args[] ) { 
	//Start SDL 
	App * app = GetApp();

	return app->Run();
} 