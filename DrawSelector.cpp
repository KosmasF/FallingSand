#include "DrawSelector.h"

void DrawSelector(SDL_Renderer* renderer, Vector2D pos, ObjectType type)
{
    switch(type){
        case Sand:
            SDL_SetRenderDrawColor(renderer, 0xff, 0xbb, 0x00, 0xff / 2);
            break;
        case Stone:
            SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xff / 2);
            break;
        case Air:
            SDL_SetRenderDrawColor(renderer, 0xff, 0x66, 0xff, 0xff / 2);
            break;

    }
    for(int brush_x = pos.x - BRUSH_SIZE/2; brush_x < pos.x + BRUSH_SIZE/2; brush_x++){
        if(brush_x < 0 || brush_x >= WIDTH/CELL_SIZE) continue;
        for(int brush_y = pos.y - BRUSH_SIZE/2; brush_y < pos.y + BRUSH_SIZE/2; brush_y++){
            if(brush_y < 0 || brush_y >= HEIGHT/CELL_SIZE) continue;
            if(pow(brush_x - pos.x, 2)+pow(brush_y - pos.y, 2) < BRUSH_SIZE * BRUSH_SIZE / 2 / 2)
            {
                SDL_RenderDrawPoint(renderer, brush_x, brush_y);
            }
        }
    }
}