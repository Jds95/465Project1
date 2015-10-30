#ifndef SCORE_H
#define SCORE_H

class Score
{
public:
    Score()    
    {
        rect.x = 550;
        rect.y = 30;
        rect.w = 50;
        rect.h = 50;
    }

    SDL_Surface* surface;
    SDL_Rect rect;
};

#endif
