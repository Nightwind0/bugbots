#include "SDL/SDL.h"
int main( int argc, char* args[] ) { 
	//Start SDL 
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1){
		return 1;
	}

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 
	//Quit SDL 
	SDL_Quit(); 
	return 0; 
} 