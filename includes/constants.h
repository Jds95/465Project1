#include <SDL.h>
#include <SDL_image.h>
#include "SDL_mixer.h"
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Port used for networking
const int PORT = 3750;

const int SEND_SHIP = 0;
const int SEND_AST = 1;

void terr_generation();
void terr_print();
bool overlap_check();
void safe_zone(const int &);
void fill_in(const int &);
bool zone1_check();
void coord();
void serverMain();
void clientMain();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

// Game screen Surface
SDL_Surface* gScreenSurface= NULL;

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );


//Current displayed image
SDL_Surface* gStretchedSurface = NULL;
