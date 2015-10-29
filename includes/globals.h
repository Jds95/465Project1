// Header file to hold all of the global variables
# include <SDL.h>


Asteroid asteroid[100];
SDL_Rect zone1;
SDL_Rect zone2;
SDL_Rect zone3;
SDL_Rect zone4;
SDL_Rect zone5;
SDL_Rect zone6;
SDL_Rect zone7;
SDL_Rect zone8;
int safe = rand() % 8 + 1;
LTimer timer;

// Global Network Variables, 
//IPaddress ip;
//TCPsocket socket = NULL;
//SDLNet_SocketSet set = NULL;
//TCPsocket client = NULL;
TTF_Font *font;
bool clientInit = false;
bool initTerrain = false;

//rects
SDL_Rect SpaceSheep;    // sheep
SDL_Rect ClientSpaceSheep;
SDL_Rect background;    //background
SDL_Rect gameOverScreen; //game over screen
SDL_Rect gameOverText; // gameover text
SDL_Rect scoreText; // score text
SDL_Rect playAgain; // play again text
SDL_Rect border1;   // top
SDL_Rect border2;   // bottom
SDL_Rect border3;   // left
SDL_Rect border4;   // right
//surfaces
SDL_Surface *back;  // background
SDL_Surface *bor1;  // top
SDL_Surface *bor2;  // bottom
SDL_Surface *bor3;  // left
SDL_Surface *bor4;  // right
SDL_Surface *gameOver; // gameover
SDL_Surface *gameOverTxt; // gameover text
SDL_Surface *scoreTxt; // score text
SDL_Surface *play; // play again text
