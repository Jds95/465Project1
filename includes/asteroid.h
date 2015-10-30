#include <SDL.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#ifndef ASTEROID_H
#define ASTEROID_H

class Asteroid
{
public:
    Asteroid()
        : speed(3.5), screen(false)
    {
        asteroid = SDL_LoadBMP("includes/asteroid.bmp");
        ast.x = 680;
        ast.y = rand() % 480;
        ast.w = 32;
        ast.h = 32;
    }
    void left()
    {
        if (ast.x < 5)
        {
            screen = false;
            ast.x = 640;
            ast.y = rand() % 480;
        }
        ast.x -= speed;
        
    }
    void print()
    {
        if (screen)
        {
           
            left();
            SDL_BlitSurface(asteroid, NULL,
                                gScreenSurface, &ast);
        }
    }
    bool overlap_check(Asteroid & asteroid)
    {
        bool flag = true;
        
        // Find edges of rect1
        int right1 = ast.x + ast.w;
        int bottom1 = ast.y + ast.h;
        
        // Find edges of rect2
        int right2 = asteroid.ast.x+ asteroid.ast.w;
        int bottom2 = asteroid.ast.y + asteroid.ast.h;
        if (ast.x == asteroid.ast.x && right1 == right2 &&
            ast.y == asteroid.ast.y && bottom1 == bottom2)
        {
            flag = false;
        }
        // Check edges
        if ( ast.x > right2 )// Left 1 is right of right 2
            flag = false; // No collision
        
        if ( right1 < asteroid.ast.x ) // Right 1 is left of left 2
            flag = false; // No collision
        
        if ( ast.y > bottom2 ) // Top 1 is below bottom 2
            flag = false; // No collision
        
        if ( bottom1 < asteroid.ast.y) // Bottom 1 is above top 2 
            flag = false; // No collision


    
        return flag;
    }

    
public:
    SDL_Rect ast;
    double speed;
    bool screen;
    SDL_Surface * asteroid;
};

#endif
