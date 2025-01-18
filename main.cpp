#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 6

#define MAX_NAME_LENGTH 15

bool mouse_pressed = false;

bool Loop()
{
	SDL_Event e;

	while( SDL_PollEvent( &e ) != 0 ) {
        if(e.type == SDL_QUIT)
        {
            return false;
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            mouse_pressed = true;
        }
        else if(e.type == SDL_MOUSEBUTTONUP)
        {
            mouse_pressed = false;
        }
    }
    return true;
}

const int numObjects = (WIDTH / CELL_SIZE) * (HEIGHT / CELL_SIZE);

enum ObjectType{
    Sand,
    Stone,
    Air
};

struct Vector2D{
    int x,y;
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
    void Update(Vector2D pos)
    {
        switch(type)
        {
            case Stone:
                break;
            case Sand:
                Object* below = this + (WIDTH/CELL_SIZE);
                if(below->type == Air){
                    if(pos.y != (HEIGHT/CELL_SIZE) - 1){
                        below->type = type;
                        below->color = color;
                        type = Air;
                    }
                    return;
                }
                else{
                    bool right = (below + 1)->type == Air;
                    bool left = (below - 1)->type == Air;
                    if(pos.x == (WIDTH/CELL_SIZE) - 1){
                        right = false;
                    }
                    if(pos.x == 0){
                        left = false;
                    }
                    switch (right | left << 1)
                    {
                        case 0:
                            return;
                        case 1:
                            case_1:
                            (below + 1)->type = type;
                            (below + 1)->color = color;
                            type = Air;
                            return;
                        case 2:
                            case_2:
                            (below - 1)->type = type;
                            (below - 1)->color = color;
                            type = Air;
                            return;
                        case 3:
                            bool will_go_right = rand() > RAND_MAX / 2;
                            if(will_go_right){
                                goto case_1;
                            }
                            else{
                                goto case_2;
                            }
                    }
                }
                break;
        }
    }
};

void DrawFPS(Vector2D pos, TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, float deltaTime)
{
    float fps = 1/deltaTime;

    SDL_Surface* text;
    char* name = (char*)malloc(MAX_NAME_LENGTH);
    snprintf(name, MAX_NAME_LENGTH, "%.2f FPS", fps);
    text = TTF_RenderText_Solid(font, name, color );
    if ( !text ) {
        printf("Failed to render text: %s\n", TTF_GetError());
    }
    free(name);

    SDL_Texture* text_texture;
    text_texture = SDL_CreateTextureFromSurface( renderer, text );
    SDL_Rect dest = { pos.x, pos.y, text->w, text->h };
    SDL_RenderCopy( renderer, text_texture, NULL, &dest );
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
}

Object objects[numObjects];

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Falling Sand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, NULL);
    srand(time(NULL));

    TTF_Init();
    TTF_Font* font;
    font = TTF_OpenFont("LiberationMono-Regular.ttf", 20);
    if ( !font ) {
        printf("Failed to load font: %s\n", TTF_GetError());
    }
    

    
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
            if(mouse_x >= 0 && mouse_y >= 0 && mouse_x <= WIDTH && mouse_y <= HEIGHT)
            {
                x = mouse_x/CELL_SIZE;
                y = mouse_y/CELL_SIZE;
                if(objects[x + (y * (WIDTH/CELL_SIZE))].type == Air){
                    objects[x + (y * (WIDTH/CELL_SIZE))] = (Object){
                        .type = Sand,
                        .color = SDL_Color{0xFF, (unsigned char)((((float)rand() / RAND_MAX)*(0xFF-0xAA)) + 0xAA), 0x00, 0xFF}
                    };
                    printf("Added sand at: %d %d\n", x, y);
                }
            }
        }

        for(int i = 0; i < numObjects; i++){
            objects[i].Draw(renderer, {i % (WIDTH/CELL_SIZE), i / (WIDTH/CELL_SIZE)});
        }

        for(int i = 0; i < numObjects; i++){
            objects[i].Update({i % (WIDTH/CELL_SIZE), i / (WIDTH/CELL_SIZE)});
        }

        DrawFPS({0,0}, font, renderer, {0x00, 0xff, 0x00, 0xff}, deltaTime);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(win);

        end_time = (float)clock() / CLOCKS_PER_SEC;
        deltaTime = end_time - start_time;
        start_time = end_time;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}