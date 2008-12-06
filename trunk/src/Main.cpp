#include "SDL/SDL.h"
#include "Defs.h"

int main( int argc, char* args[] ) { 
	//Start SDL 
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1){
		return 1;
	}

	SDL_Surface *screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 
	//Quit SDL 
	SDL_Quit(); 
	return 0; 
} 