#pragma once
#include <SDL2/SDL.h>

#include "Units.h"

enum ObjectType{
    Sand,
    Stone,
    Air
};

struct Object{
    ObjectType type = Air;
    SDL_Color color;
    void Draw(SDL_Renderer* renderer, Vector2D pos)
    {
        if(type == Air)
            return;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    };
    void Update(Vector2D pos);
    void Fall(Vector2D pos);
    void Slide(Vector2D pos);
    void Erosion(Vector2D pos);
};
