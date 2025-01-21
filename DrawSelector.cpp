#include "DrawSelector.h"

void DrawSelector(SDL_Renderer* renderer, Vector2D pos, ObjectType type)
{
    SDL_Color color;
    switch(type){
        case Sand:
            color = {0xff, 0xbb, 0x00, 0xff / 2};
            break;
        case Stone:
            color = {0x55, 0x55, 0x55, 0xff / 2};
            break;
        case Air:
            color = {0xff, 0x66, 0xff, 0xff / 2};
            break;

    }
    for(int brush_x = pos.x - BRUSH_SIZE/2; brush_x < pos.x + BRUSH_SIZE/2; brush_x++){
        if(brush_x < 0 || brush_x >= WIDTH) continue;
        for(int brush_y = pos.y - BRUSH_SIZE/2; brush_y < pos.y + BRUSH_SIZE/2; brush_y++){
            if(brush_y < 0 || brush_y >= HEIGHT) continue;
            if(pow(brush_x - pos.x, 2)+pow(brush_y - pos.y, 2) < BRUSH_SIZE * BRUSH_SIZE / 2 / 2)
            {
                Object obj = {
                    .type = Stone,//So that is Drawn
                    .color = color
                };
                obj.Draw(renderer, {brush_x/CELL_SIZE, brush_y/CELL_SIZE});
            }
        }
    }
}