#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

#include "Objects.h"
#include "Units.h"
#include "Loop.h"
#include "DrawFPS.h"

#define MAX_NAME_LENGTH 15

bool mouse_pressed = false;
const int numObjects = (WIDTH / CELL_SIZE) * (HEIGHT / CELL_SIZE);
ObjectType typeSelected = Sand;
Object* objects;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Falling Sand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    srand(time(NULL));

    TTF_Init();
    TTF_Font* font;
    font = TTF_OpenFont("LiberationMono-Regular.ttf", 20);
    if ( !font ) {
        printf("Failed to load font: %s\n", TTF_GetError());
    }
    
    objects = (Object*)malloc(numObjects * sizeof(Object));

    
    for(int i = 0; i < numObjects; i++){
        objects[i] = (Object){
            .type = Air,
            .color = {0x00, 0x00, 0x00, 0x00}
        };
    }

    float start_time, end_time, deltaTime;
    start_time = (float)clock() / CLOCKS_PER_SEC;
    deltaTime = 0;
    while(Loop())
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);

        if(mouse_pressed){
            int x, y, mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            if(mouse_x >= 0 && mouse_y >= 0 && mouse_x < WIDTH && mouse_y < HEIGHT)
            {
                x = mouse_x/CELL_SIZE;
                y = mouse_y/CELL_SIZE;
                for(int brush_x = x - BRUSH_SIZE/2; brush_x < x + BRUSH_SIZE/2; brush_x++)
                {
                    if(brush_x < 0 || brush_x >= WIDTH/CELL_SIZE) continue;
                    for(int brush_y = y - BRUSH_SIZE/2; brush_y < y + BRUSH_SIZE/2; brush_y++)
                    {
                        if(brush_y < 0 || brush_y >= HEIGHT/CELL_SIZE) continue;
                        if(pow(brush_x - x, 2)+pow(brush_y - y, 2) < BRUSH_SIZE * BRUSH_SIZE / 2 / 2)
                        {
                            switch (typeSelected)
                            {
                                case Sand:
                                    if(objects[brush_x + (brush_y * (WIDTH/CELL_SIZE))].type == Air){
                                        if(rand() > RAND_MAX * BRUSH_PROBABILITY){
                                            objects[brush_x + (brush_y * (WIDTH/CELL_SIZE))] = (Object){
                                                .type = Sand,
                                                .color = SDL_Color{0xFF, (unsigned char)((((float)rand() / RAND_MAX)*(0xFF-0xAA)) + 0xAA), 0x00, 0xFF}
                                            };
                                        }
                                    }
                                    break;
                                case Stone:
                                    if(objects[brush_x + (brush_y * (WIDTH/CELL_SIZE))].type != Stone){
                                        unsigned char brightness = (unsigned char)((((float)rand() / RAND_MAX)*(0x96 - 0x64)) + 0x64);
                                        objects[brush_x + (brush_y * (WIDTH/CELL_SIZE))] = (Object){
                                            .type = Stone,
                                            .color = SDL_Color{brightness, brightness, brightness, 0xFF}
                                        };
                                    }
                                    break;
                                case Air:
                                    objects[brush_x + (brush_y * (WIDTH/CELL_SIZE))] = (Object){
                                        .type = Air,
                                        .color = SDL_Color{0x00, 0x00, 0x00, 0x00}
                                    };
                                    break;       
                            // printf("Added sand at: %d %d\n", x, y);
                            }
                        }
                    }
                }
            }
        }

        for(int i = 0; i < numObjects; i++){
            objects[i].Draw(renderer, {i % (WIDTH/CELL_SIZE), i / (WIDTH/CELL_SIZE)});
        }

        for(int i = numObjects - 1; i > - 1; i--){
            objects[i].Fall({i % (WIDTH/CELL_SIZE), i / (WIDTH/CELL_SIZE)});
        }

        for(int row = HEIGHT/CELL_SIZE - 1; row > -1; row--){
            switch(rand() > RAND_MAX/2){
                case false:
                    for(int column = 0; column < WIDTH/CELL_SIZE; column++){
                        objects[column + (row*WIDTH/CELL_SIZE)].Slide({column, row});
                    }
                    break;
                case true:
                    for(int column = WIDTH/CELL_SIZE - 1; column > -1; column--){
                        objects[column + (row*WIDTH/CELL_SIZE)].Slide({column, row});
                    }
                    break;
            }
        }

        bool* eroded = (bool*)malloc(sizeof(bool)*numObjects);
        for(int i = 0; i < numObjects; i++) eroded[i] = false;
        for(int row = HEIGHT/CELL_SIZE - 1; row > -1; row--){
            switch(rand() > RAND_MAX/2){
                case false:
                    for(int column = 0; column < WIDTH/CELL_SIZE; column++){
                        objects[column + (row*WIDTH/CELL_SIZE)].Erosion({column, row}, eroded);
                    }
                    break;
                case true:
                    for(int column = WIDTH/CELL_SIZE - 1; column > -1; column--){
                        objects[column + (row*WIDTH/CELL_SIZE)].Erosion({column, row}, eroded);
                    }
                    break;
            }
        }
        free(eroded);

        DrawFPS({0,0}, font, renderer, {0x00, 0xff, 0x00, 0xff}, deltaTime);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(win);

        end_time = (float)clock() / CLOCKS_PER_SEC;
        deltaTime = end_time - start_time;
        start_time = end_time;
    }

    free(objects);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}