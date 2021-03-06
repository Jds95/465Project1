//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "asteroid.h"
#include "score.h"
#include "LTimer.h"
#include "menu.h"
#include <sstream>
#include <iostream>
#include "globals.h"

// The idea to use enum protocal came from David and Dennis, Thanks!
enum Protocal { SHEEP, ASTEROIDS, DONT_SEND_AST};

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
	
	//Initialize SDL_net.
	if (SDLNet_Init() == -1)
	{
		printf("SDLNet_Init: %s", SDLNet_GetError());
	}
    
    
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    
    gWindow = SDL_CreateWindow( "SpaceSheep Adventures", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Get window surface
        gScreenSurface = SDL_GetWindowSurface( gWindow );
    }
}



void terr_generation()
{
    for (int i = 0; i < 100; ++i)
    {
        if (rand() % 250 == 0)
        {
            asteroid[i].screen = true;
          
        }
    }
}

void close()
{
    //Free loaded image
    SDL_FreeSurface( gStretchedSurface );
    gStretchedSurface = NULL;

    SDL_FreeSurface (gScreenSurface);
    gScreenSurface = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    // Quit SDL_Net
    SDLNet_Quit();

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL subsystems
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        //optimizedSurface = SDL_ConvertSurface( loadedSurface, sheep->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimizedSurface;
}

void terr_print()
{
    // Scans the array of asteroids
    for (int i = 0; i < 100; ++i)
    {
        // If the asteroid is on screen, performs an overlap check
        if (asteroid[i].screen)
        {
            for (int l = 0; l < 100; ++l)
            {
                if (asteroid[l].screen)
                {
                    // If the asteroid overlaps, it is given new x/y values
                    // and taken off the screen
                    if (asteroid[i].overlap_check(asteroid[l]))
                    {
                        asteroid[i].screen = false;
                        asteroid[i].ast.x = 680;
                        asteroid[i].ast.y = rand() % 480;
                    } 
                }
            }
        }
    }
}
    
void paint()
{
    // For loop to scan both asteroid in and asteroid array
    for (int i = 0; i < 100; ++i)
    {
        asteroid[i].print();   
    }
}

void safe_zone(const int & safe)
{
    //********************************************************************
    // Function takes in safe zone, then scans the array of asteroids
    // then deletes any asteroids if they are within the active safe zones
    //********************************************************************
    int zone = safe - 1;
    int left1 = 640;
    int right1 = 672;
    int top1 = zone * 60;
    int bottom1 = zone * 60 + 60;
    
    // Find edges of rect2
    for (int i = 0; i < 100; ++i)
    {
        bool flag = true;
        
        int left2 = asteroid[i].ast.x;
        int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
        int top2 = asteroid[i].ast.y;
        int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
        if (left1 == left2 && right1 == right2 &&
            top1 == top2 && bottom1 == bottom2)
        {
            flag = false;
        }
        // Check edges
        if ( left1 > right2 )// Left 1 is right of right 2
            flag = false; // No collision
        
        if ( right1 < left2 ) // Right 1 is left of left 2
            flag = false; // No collision
        
        if ( top1 > bottom2 ) // Top 1 is below bottom 2
            flag = false; // No collision
        
        if ( bottom1 < top2 ) // Bottom 1 is above top 2 
            flag = false; // No collision
        
        if (flag == true)
        {
            asteroid[i].screen = false;
            asteroid[i].ast.x = 680;
            asteroid[i].ast.y = rand() % 480;
        }
    }
}

void fill_in(const int & safe)
{
    //*******************************************************************
    // Function looks at safe zone, if it isn't the safe zone, it should
    // put an asteroid inside the frame of the zone, so not zone 1 if any
    // y value between 0 to 60.
    //*******************************************************************
    if (safe != 1)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60;
                break;
            }
        }
    }
    if (safe != 2)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 60;
                break;
            }
        }
    }
    if (safe != 3)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 120;
                break;
            }
        }
    }
    if (safe != 4)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 180;
                break;
            }
        }
    }
    if (safe != 5)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 240;    
                break;
            }
        }
    }
    if (safe != 6)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 300;
                break;
            }
        }   
    }
    if(safe != 7)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 360;
                break;
            }
        }
    }
    if (safe != 8)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (asteroid[i].screen == false)
            {
                asteroid[i].screen = true;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 60 + 420;
                break;
            }
        }
    }
}

// On clean up after project, this can be put inside of sheep class.
bool collision_check(SDL_Rect & rect)
{
    // Takes the rect of the sheep
    int right2 = rect.x + rect.w;
    int bottom2 = rect.y + rect.h;
    bool check = false;
    
    // Find edges of rect for the asteroid array if it is on the screen
    for (int i = 0; i < 100; ++i)
    {
        if (asteroid[i].screen)
        {
            bool flag = true;
            
            int left1 = asteroid[i].ast.x + 4;
            int right1 = asteroid[i].ast.x + asteroid[i].ast.w - 2;
            int top1 = asteroid[i].ast.y - 4;
            int bottom1 = asteroid[i].ast.y + asteroid[i].ast.h - 2;
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < rect.x ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < rect.y ) // Bottom 1 is above top 2 
                flag = false; // No collision
            if (flag == true)
            {
                check = true;
                return check;
            }
        }
    }
    return check;
}

void coord()
{
    back = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    bor1 = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    bor2 = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    bor3 = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    bor4 = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    gameOver = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
    // Safe Zone Coordinates and dimensions
    zone1.x = 640;
    zone1.y = 0;
    zone1.w = 60;
    zone1.h = 60;
    
    zone2.x = 640;
    zone2.y = 60;
    zone2.w = 60;
    zone2.h = 60;
    
    zone3.x = 640;
    zone3.y = 120;
    zone3.w = 60;
    zone3.h = 60;
    
    zone4.x = 640;
    zone4.y = 180;
    zone4.w = 60;
    zone4.h = 60;
    
    zone5.x = 640;
    zone5.y = 240;
    zone5.w = 60;
    zone5.h = 60;
    
    zone6.x = 640;
    zone6.y = 300;
    zone6.w = 60;
    zone6.h = 60;
    
    zone7.x = 640;
    zone7.y = 360;
    zone7.w = 60;
    zone7.h = 60;
    
    zone8.x = 640;
    zone8.y = 420;
    zone8.w = 60;
    zone8.h = 60;
    
    //border dimentions
    border1.x = 0;
    border1.y = 0;
    border1.w = 32;
    border1.h = 480;
    
    border2.x = 608;
    border2.y = 0;
    border2.w = 32;
    border2.h = 480;
    
    border3.x = 0;
    border3.y = 0;
    border3.w = 640;
    border3.h = 32;
    
    border4.x = 0;
    border4.y = 448;
    border4.w = 640;
    border4.h = 32;
    
    //background dimensions
    background.x = 0;
    background.y = 0;
    background.w = 640;
    background.h = 480;
    
    //gameover dimensions
    gameOverScreen.x = 0;
    gameOverScreen.y = 0;
    gameOverScreen.w = 640;
    gameOverScreen.h = 480;
    clientgameOverScreen.x = 0;
    clientgameOverScreen.y = 0;
    clientgameOverScreen.w = 640;
    clientgameOverScreen.h = 480;
    
    //gameover text position
    gameOverText.x = 255;
    gameOverText.y = 150;
    clientgameOverText.x = 255;
    clientgameOverText.y = 150;
    
    //score text position
    scoreText.x = 450;
    scoreText.y = 30;
    clientscoreText.x = 450;
    clientscoreText.y = 30;
    
    // play text position
    playAgain.x = 225;
    playAgain.y = 250;
    
    //color rects
    SDL_FillRect(back, NULL, SDL_MapRGB(back->format, 0, 0, 0));
    SDL_FillRect(bor1, NULL, SDL_MapRGB(bor1->format, 255, 0, 0));
    SDL_FillRect(bor2, NULL, SDL_MapRGB(bor2->format, 255, 0, 0));
    SDL_FillRect(bor3, NULL, SDL_MapRGB(bor3->format, 255, 0, 0));
    SDL_FillRect(bor4, NULL, SDL_MapRGB(bor4->format, 255, 0, 0));
    SDL_FillRect(gameOver, NULL, SDL_MapRGB(gameOver->format, 0, 0, 0));
    
}

void send_asteroid(TCPsocket client, int x, int y, int astindex, bool screen,
    Protocal protocal)
{
    // Send current protocal to Client
    int sent = SDLNet_TCP_Send(client, &protocal, sizeof(protocal));
    if (sent != sizeof(client, protocal))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
    // Send the index of the asteroid from the array
    sent = SDLNet_TCP_Send(client, &astindex, sizeof(astindex));
    if (sent != sizeof(client, astindex))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
    // Send the x coordinate for that asteroid[astindex]
    sent = SDLNet_TCP_Send(client, &x, sizeof(x));
    if (sent != sizeof(client, x))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
    // Send the y coordinate for that asteroid[asteindex]
    sent = SDLNet_TCP_Send(client, &y, sizeof(y));
    if (sent != sizeof(client, y))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
    // Send the bool value for that asteroid[astindex] if it's on screen
    sent = SDLNet_TCP_Send(client, &screen, sizeof(screen));
    if (sent != sizeof(client, screen))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
}
void send_sheep(TCPsocket socket, int x, int y)
{
    // Set protocal to Sheep so client/server knows what it is receiving
    Protocal protocal = SHEEP;
    int sent;
    // Send the protocal
    sent = SDLNet_TCP_Send(socket, &protocal,
                               sizeof(protocal));
    if (sent != sizeof(socket, protocal))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl; 
    }
    // Send the x coordinate of the sheep
    sent = SDLNet_TCP_Send(socket, &x, sizeof(x));
    if (sent != sizeof(socket, x))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
    // Send the y coordinate of the sheep
    sent = SDLNet_TCP_Send(socket, &y, sizeof(y));
    if (sent != sizeof(socket, y))
    {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
    }
}


void serverMain()
{
    //Sheep image
    // Create local surface object for sheep
    SDL_Surface* sheep = NULL;
    // Create local surface object for clientsheep
    SDL_Surface* clientsheepclone = NULL;
    
    // Load the images to the surfaces
    sheep = SDL_LoadBMP("images/sheep.bmp");
    if( sheep == NULL )
    {
        printf( "Failed to load stretching image!\n" );
    }
    clientsheepclone = SDL_LoadBMP("images/sheep.bmp");
    if( clientsheepclone == NULL )
    {
        printf( "Failed to load stretching image!\n" );
    }
    
    
    // Connection information
    IPaddress ip;
    
    // Resolve the argument into an IPaddress
    if (SDLNet_ResolveHost(&ip, NULL, PORT) == -1)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return;
    }
    
    // Open the socket to listen for connections from the client
    TCPsocket listner = SDLNet_TCP_Open(&ip);
    if(!listner)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return;
    }
    // Set up the client
    TCPsocket client = 0;
    
    // Prepare a SocketSet so we can check for messages from the client
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
    
    // Load font for game
    font = TTF_OpenFont("includes/game_over.ttf",60);
    
    if (!menu(gScreenSurface, font))
    {
        
        //Main loop flag
        bool quit = false;
        
        //Event handler
        SDL_Event e;
        SDL_Rect ClientSpaceSheepClone;
        double sheepSpeedX = 3;         //sheep speed, duh
        double sheepSpeedY = 3.5; 
        bool sheep_screen = true;   //is sheep alive
        SDL_Rect SpaceSheep;    // sheep
        Protocal protocal;
        
        int ogclock = 0;
        int ogclock2 = 0;
        bool hit1 = false;
        bool hit2 = false;
        bool part1dead = false;
        bool part2dead = false;
        bool superdead = false;
        bool red_sq = false;
        
        //sheep starting dimensions
        SpaceSheep.x = 300;
        SpaceSheep.y = 200;
        SpaceSheep.w = 25;
        SpaceSheep.h = 25;
        
        ClientSpaceSheepClone.x = 300;
        ClientSpaceSheepClone.y = 200;
        ClientSpaceSheepClone.w = 25;
        ClientSpaceSheepClone.h = 25;   
        
        
        
        // initialize score counter, create score object, and create score timer
        int scoreCount = 0;
        Score score;
        
        LTimer scoreTimer;
        
        //While application is running
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ))
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }                
            }
            //std::cout << "In the game loop... \n";
            if (!scoreTimer.isStarted())
            {
                scoreTimer.start();
            }
            // update the score counter by 1 point every 1/10 of a second
            if (scoreTimer.getTicks() >= 100)
            {
                ++scoreCount;
                scoreTimer.stop();
            }
            //movement
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_W])          //up
            {
                if (SpaceSheep.y <= 35);    //if hitting the edge, don't move
                
                else
                {
                    SpaceSheep.y -= sheepSpeedY; // else move
                }
                
            }
            
            if (state[SDL_SCANCODE_S])          //down
            {
                if (SpaceSheep.y >= 422);
                
                else
                {  
                    SpaceSheep.y += sheepSpeedY;
                }
            }
            
            if (state[SDL_SCANCODE_A])          //left
            {
                if (SpaceSheep.x <= 33);
                
                else
                {
                    SpaceSheep.x -= sheepSpeedX;
                }
            }
            
            if (state[SDL_SCANCODE_D])          //right
            {
                if (SpaceSheep.x >= 580);
                
                else
                {
                    SpaceSheep.x += sheepSpeedX;
                }
            }
            
            if( !timer.isStarted() )
            {
                timer.start();
            }

            SDL_BlitScaled(back, NULL, gScreenSurface, &background);
            
            terr_generation(); // Random generate asteroids
            fill_in(safe); // Fill in array 
            terr_print(); // Does final overlap check through array
            paint(); // If they are on screen paint
           
            
            
            SDL_BlitScaled(bor1, NULL, gScreenSurface, &border1);
            SDL_BlitScaled(bor2, NULL, gScreenSurface, &border2);
            SDL_BlitScaled(bor3, NULL, gScreenSurface, &border3);
            SDL_BlitScaled(bor4, NULL, gScreenSurface, &border4);

            safe_zone(safe);
            protocal = ASTEROIDS; // TIme to update client terrain
            
        
            // Conditional to swap Safe Zones
            if (timer.getTicks() % 750 > 500)
            {
                // Random number for switch to change zones
                int random = rand() % 10;
                
                switch (random % 2)
                {
                    case 0 :
                        if (safe == 1)
                        {
                            safe += 1;
                        }
                        else
                        {
                            safe -= 1;
                        }
                        break;
                    case 1:
                        if (safe == 8)
                        {
                            safe -= 1;
                        }
                        else 
                        {
                            safe += 1;
                        }
                        break;
                }
            }

            // Check to see if sheep(s) hits any asteroids
            if (collision_check(SpaceSheep)) 
            {
                part1dead = true;
                sheep = SDL_CreateRGBSurface(0, 25, 25, 32, 0, 0, 0, 0);
                SDL_FillRect(sheep, NULL, SDL_MapRGB(sheep->format, 255, 0, 0));
                hit1 = true;         
            }
            
            if (hit1)
                ogclock++;
            
            if (ogclock >= 75)
            {
                sheep = SDL_LoadBMP("images/sheep.bmp");
                ogclock = 0;
                hit1 = false;
                part1dead = false;
            }
            
            if (client != 0)
            {
                if (collision_check(ClientSpaceSheepClone)) 
                {
                    part2dead = true;
                    clientsheepclone = SDL_CreateRGBSurface(0, 25, 25, 32, 0, 0, 0, 0);
                    SDL_FillRect(clientsheepclone, NULL, SDL_MapRGB(clientsheepclone->format, 255, 0, 255));
                    hit2 = true;
                }

                if (hit2)
                    ogclock2++;

                if (ogclock2 >= 75)
                {
                    clientsheepclone = SDL_LoadBMP("images/sheep.bmp");
                    ogclock2 = 0;
                    hit2 = false;
                    part2dead = false;
                }

                if (part1dead == true && part2dead == true)
                    superdead = true;

                if (superdead == true)
                {
                    sheep_screen = false;
                    scoreTimer.stop();
                    quit = true;
                }
            }
            else
                if (part1dead)
                {
                    sheep_screen = false;
                    scoreTimer.stop();
                    quit = true;
                }

            if (sheep_screen)
            {
                // finally blit sheep
                SDL_BlitScaled(sheep, NULL, gScreenSurface, &SpaceSheep);   
            }
            if (client != 0)
            {
                SDL_BlitScaled(clientsheepclone, NULL, gScreenSurface,
                               &ClientSpaceSheepClone);
            }
            
            // Blit the score onto the screen
            SDL_Color score_color = {255, 0, 0};
            score.surface = TTF_RenderText_Solid(font, std::to_string(scoreCount).c_str(), score_color);
            SDL_BlitSurface(scoreTxt, NULL, gScreenSurface, &scoreText);
            SDL_BlitSurface(score.surface, NULL, gScreenSurface, &score.rect);
            
            //-------------------------------------------------------------------
            // NETWORKING
            //-------------------------------------------------------------------
            
            
            //If the client socket is still NULL, no one has connected yet
            //Check to see if someone wants to connect
            if (client == NULL)
            {
                client = SDLNet_TCP_Accept(listner);
                //If it isn't zero anymore, the client socket is now connected
                //Add it to the SocketSet so that we can check it for data later
                if (client != NULL)
                {
                    if (SDLNet_TCP_AddSocket(set, client) == -1)
                    {
                        printf("SDLNet_AddSocket: %s", SDLNet_GetError());
                    }
                }	
            }
            // If there is a client, we need to send data to the client
            if (client != 0)
            {
                // If protocal is asteroids, send array of active asteroids 
                if (protocal == ASTEROIDS)
                {
                    for (int i = 0; i < 100; ++i)
                    {
                        if (asteroid[i].screen)
                        {
                            protocal = ASTEROIDS;
                            int index = i;
                            send_asteroid(client, asteroid[i].ast.x,
                                          asteroid[i].ast.y, index,
                                          asteroid[i].screen, protocal);
                        }
                    }
                }
                // Send the Server sheep to the client's screen
                send_sheep(client, SpaceSheep.x, SpaceSheep.y);
                // Loop check to see if the client sends any information
                while (SDLNet_CheckSockets(set, 0))
                {
                    int got = SDLNet_TCP_Recv(client, &protocal,
                                              sizeof(protocal));
                    if (got <= 0)
                    {
                        std::cerr << "Reciving protocal from client failed";
                        quit = true;
                        break;
                    }
                    if (protocal == SHEEP)
                    {
                        SDLNet_TCP_Recv(client,&ClientSpaceSheepClone.x,
                                        sizeof(ClientSpaceSheepClone.x));
                        SDLNet_TCP_Recv(client, &ClientSpaceSheepClone.y,
                                        sizeof(ClientSpaceSheepClone.y));
                    }  
                }
                protocal = DONT_SEND_AST;
            }
            
            // GAME OVER screen
            while (quit)
            {         
                bool closeGame = false;
                //Handle events on queue
                if( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        closeGame = true;
                    }     
                }
                
                
                
                gameOverTxt = TTF_RenderText_Solid(font, "Game Over", {255,0,0});
                scoreTxt = TTF_RenderText_Solid(font, "Score:", {255,0,0});
                play = TTF_RenderText_Solid(font, "Thanks for Playing!", {255,0,0});
                SDL_BlitScaled(gameOver, NULL, gScreenSurface, &gameOverScreen);
                
                // Position score in center of screen
                score.rect.x = 350;
                score.rect.y = 200;
                SDL_BlitSurface(gameOverTxt, NULL, gScreenSurface, &gameOverText);
                scoreText.x = 240;
                scoreText.y = 200;
                SDL_BlitSurface(scoreTxt, NULL, gScreenSurface, &scoreText);
                SDL_BlitSurface(score.surface, NULL, gScreenSurface, &score.rect);
                SDL_BlitSurface(play, NULL, gScreenSurface, &playAgain);
                
                
                // Close program
                if ( closeGame ) break;
                
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
            }
            
            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(20); 
            
        }             
    }
    SDLNet_TCP_Close(listner);
    SDLNet_TCP_Close(client);
    
    SDL_FreeSurface (sheep);
    sheep = NULL;
    
    SDL_FreeSurface (clientsheepclone);
    clientsheepclone = NULL;
    close();   
}

Asteroid asteroidCL[100];

bool client_collision_check(SDL_Rect & rect)
{
    // Takes the rect of the sheep
    int right2 = rect.x + rect.w;
    int bottom2 = rect.y + rect.h;
    bool check = false;
    
    // Find edges of rect for the asteroid array if it is on the screen
    for (int i = 0; i < 100; ++i)
    {
        if (asteroidCL[i].screen)
        {
            bool flag = true;
            
            int left1 = asteroidCL[i].ast.x + 4;
            int right1 = asteroidCL[i].ast.x + asteroidCL[i].ast.w - 2;
            int top1 = asteroidCL[i].ast.y - 4;
            int bottom1 = asteroidCL[i].ast.y + asteroidCL[i].ast.h - 2;
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < rect.x ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < rect.y ) // Bottom 1 is above top 2 
                flag = false; // No collision
            if (flag == true)
            {
                check = true;
                return check;
            }
        }
    }
    return check;
}


void clientMain(const char * serverName)
{
    //Sheep image    
    SDL_Surface* sheepclone = NULL;
    SDL_Surface* clientsheep = NULL;
    sheepclone = SDL_LoadBMP("images/sheep.bmp");
    if( sheepclone == NULL )
    {
        printf( "Failed to load stretching image!\n" );
    }
    clientsheep = SDL_LoadBMP("images/sheep.bmp");
    if( clientsheep == NULL )
    {
        printf( "Failed to load stretching image!\n" );
    }
 
    // Connection information
    IPaddress ip;
    
    // Resolve the argument into an IPaddress
    if (SDLNet_ResolveHost(&ip, serverName, PORT) == -1)
    {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return;
    }

    // Open a socket to communicate with the server
    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if(!socket)
    {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return;
    }

    // Create a socket set;
    SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
    if(!set)
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
    else
    {
        if (SDLNet_TCP_AddSocket(set, socket) == -1)
            std::cerr << "SDLNet_AddSocket: " << SDLNet_GetError() << std::endl;
    }
    
    if (socket == NULL)
    {
        std::cout << "\nHost is not operating" << std::endl;
        return;
    }
    // Load font for game
    font = TTF_OpenFont("includes/game_over.ttf",60);
    
    //Main loop flag
    bool clientquit = false;
    
    //Event handler
    SDL_Event e;

    double ClientsheepSpeedX = 3;         //sheep speed, duh
    double ClientsheepSpeedY = 3.5; 
    bool clientsheep_screen = true;
    // initialize score counter, create score object, and create score timer
    int clientscoreCount = 0;
    Score clientscore;
    LTimer clientscoreTimer;
    SDL_Rect ClientSpaceSheep;
    SDL_Rect SpaceSheep;
    int astindex;
    Protocal protocalRecv;
    bool sheep_screen = true;

    int ogclock = 0;
    int ogclock2 = 0;

    bool hit1 = false;
    bool hit2 = false;
    bool part1dead = false;
    bool part2dead = false;
    bool superdead = false;

    SpaceSheep.x = 300;
    SpaceSheep.y = 200;
    SpaceSheep.w = 25;
    SpaceSheep.h = 25;

    ClientSpaceSheep.x = 300;
    ClientSpaceSheep.y = 200;
    ClientSpaceSheep.w = 25;
    ClientSpaceSheep.h = 25;
    
    //While application is running
    while( !clientquit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                clientquit = true;
            }                
        }
        // Client score time isn't started? Start it already. Geez.
        if (!clientscoreTimer.isStarted())
        {
            clientscoreTimer.start();
        }
       
        // update the score counter by 1 point every 1/10 of a second
        if (clientscoreTimer.getTicks() >= 100)
        {
            ++clientscoreCount;
            clientscoreTimer.stop();
        }
        
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W])          //up
        {
            if (ClientSpaceSheep.y <= 35);    //if hitting the edge, don't move
            
            else
            {
                ClientSpaceSheep.y -= ClientsheepSpeedY;// else move
            }   
        }
        
        if (state[SDL_SCANCODE_S])          //down
        {
            if (ClientSpaceSheep.y >= 422);
            
            else
            {
                ClientSpaceSheep.y += ClientsheepSpeedY;
            }
        }
        
        if (state[SDL_SCANCODE_A])          //left
        {
            if (ClientSpaceSheep.x <= 33);
            
            else
            {
                ClientSpaceSheep.x -= ClientsheepSpeedX;
            }
        }
        
        if (state[SDL_SCANCODE_D])          //right
        {
            if (ClientSpaceSheep.x >= 580);
            
            else
            {
                ClientSpaceSheep.x += ClientsheepSpeedX;
            }
        }


        if (client_collision_check(ClientSpaceSheep)) 
        {
            part1dead = true;
            clientsheep = SDL_CreateRGBSurface(0, 25, 25, 32, 0, 0, 0, 0);
            SDL_FillRect(clientsheep, NULL, SDL_MapRGB(clientsheep->format, 255, 0, 0));
            hit1 = true;
            
        }
        
        if (hit1)
            ogclock++;
        
        if (ogclock >= 75)
        {
            clientsheep = SDL_LoadBMP("images/sheep.bmp");
            ogclock = 0;
            hit1 = false;
            part1dead = false;
        }
        
        if (client_collision_check(SpaceSheep)) 
        {
            part2dead = true;
            sheepclone = SDL_CreateRGBSurface(0, 25, 25, 32, 0, 0, 0, 0);
            SDL_FillRect(sheepclone, NULL, SDL_MapRGB(sheepclone->format, 255, 0, 255));
            hit2 = true;
        }
        
        if (hit2)
            ogclock2++;
        
        if (ogclock2 >= 75)
        {
            sheepclone = SDL_LoadBMP("images/sheep.bmp");
            ogclock2 = 0;
            hit2 = false;
            part2dead = false;
        }
        
        if (part1dead == true && part2dead == true)
            superdead = true;
        
        if (superdead == true)
        {
            clientquit = true;
        }

        // Client is open, send the information for client sheep to server
        send_sheep(socket, ClientSpaceSheep.x, ClientSpaceSheep.y);

        // Loop check to see if the server has sent any information
        while (SDLNet_CheckSockets(set, 0))
        {
            int got = SDLNet_TCP_Recv(socket, &protocalRecv, sizeof(protocalRecv));
            if (got <= 0)
            {
                std::cerr << "Failed to get protocal" << std::endl;
            }
            if (protocalRecv == SHEEP)
            {
                SDLNet_TCP_Recv(socket, &SpaceSheep.x, sizeof(SpaceSheep.x));
                SDLNet_TCP_Recv(socket, &SpaceSheep.y, sizeof(SpaceSheep.y));
             
            }
            else if (protocalRecv == ASTEROIDS)
            {
                SDLNet_TCP_Recv(socket, &astindex, sizeof(astindex));
                int x;
                SDLNet_TCP_Recv(socket, &x, sizeof(x));
                int y;
                SDLNet_TCP_Recv(socket, &y, sizeof(y));
                
                bool asteroidscreen;
                SDLNet_TCP_Recv(socket, &asteroidscreen, sizeof(asteroidscreen));
                
                asteroidCL[astindex].screen = asteroidscreen;
                asteroidCL[astindex].ast.x = x;
                asteroidCL[astindex].ast.y = y;
            }
        }
        SDL_BlitScaled(back, NULL, gScreenSurface, &background); 

        
        for (int i = 0; i < 100; ++i)
        {
            SDL_BlitScaled(asteroidCL[i].asteroid, NULL,
                            gScreenSurface, &asteroidCL[i].ast);
        }

    
        SDL_BlitScaled(bor1, NULL, gScreenSurface, &border1);       // blit borders ontop of everything
        SDL_BlitScaled(bor2, NULL, gScreenSurface, &border2);
        SDL_BlitScaled(bor3, NULL, gScreenSurface, &border3);
        SDL_BlitScaled(bor4, NULL, gScreenSurface, &border4);

       
        // finally blit sheep
        SDL_BlitScaled(sheepclone, NULL, gScreenSurface, &SpaceSheep); 
        SDL_BlitScaled(clientsheep, NULL, gScreenSurface,
                               &ClientSpaceSheep);
            
        
        
    
        // Display the client score on the client (obviously)
        SDL_Color clientscore_color = {255, 0, 0}; // Sets color of score
        clientscore.surface = TTF_RenderText_Solid(font, std::to_string(clientscoreCount).c_str(), clientscore_color);
        SDL_BlitSurface(clientscoreTxt, NULL, gScreenSurface, &clientscoreText);
        SDL_BlitSurface(clientscore.surface, NULL, gScreenSurface, &clientscore.rect);
        
     	
        
        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay(20);
    }
    
    // Display score and other text/stuff when game has ended
    while (clientquit)
    {
    	bool closeGame = false;
        //Handle events on queue
        if( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                closeGame = true;
            }     
        }

        
		clientgameOverTxt = TTF_RenderText_Solid(font, "Game Over", {255,0,0});
        clientscoreTxt = TTF_RenderText_Solid(font, "Score:", {255,0,0});
        play = TTF_RenderText_Solid(font, "Thanks for Playing!", {255,0,0});
        SDL_BlitScaled(gameOver, NULL, gScreenSurface, &gameOverScreen);
        
        // Position score in center of screen
        clientscore.rect.x = 350;
        clientscore.rect.y = 200;
        SDL_BlitSurface(clientgameOverTxt, NULL, gScreenSurface, &clientgameOverText);
        clientscoreText.x = 240;
        clientscoreText.y = 200;
        SDL_BlitSurface(clientscoreTxt, NULL, gScreenSurface, &clientscoreText);
        SDL_BlitSurface(clientscore.surface, NULL, gScreenSurface, &clientscore.rect);
        SDL_BlitSurface(play, NULL, gScreenSurface, &playAgain);
        
        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay(20);
        
        // Close program
        if ( closeGame ) break;
    }
    
	
    
    SDL_FreeSurface (sheepclone);
    sheepclone = NULL;
    
    SDL_FreeSurface (clientsheep);
    clientsheep = NULL;
    
    
    SDLNet_TCP_Close(socket);
    close();
}


int main(int argc, char* args[] )
{
    coord();

    // Is this program the host, or the client?
    bool operating_as_host;
    
    // Check the command line arguments.
    if (argc == 1)
    {
        operating_as_host = true;
        std::cout << "Operating as host: clients connect to me." << std::endl;
    }
    else if (argc == 2)
    {
        operating_as_host = false;
        std::cout << "Operating as client, connecting to " << args[1] << std::endl;
    }
    else
    {
        std::cerr << "Too many command line arguments" <<std::endl;
        return 1;
    }    
    init();
	//If no arguments, this is the server
	if (operating_as_host == true)
	{
        serverMain();
	}
	else
	{
		clientMain(args[1]);
	}
	return 0;
}

