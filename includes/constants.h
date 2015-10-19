#include <SDL.h>
#include <SDL_image.h>
#include "SDL_mixer.h"
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void terr_generation();
void terr_print();
bool overlap_check();
void safe_zone(const int &);
void fill_in(const int &);
bool zone1_check();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

// Game screen Surface
SDL_Surface* gScreenSurface= NULL;

// Music
//Mix_Music *music = NULL; 

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//Sheep image    
SDL_Surface* sheep = NULL;

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;
