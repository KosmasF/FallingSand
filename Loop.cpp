#include "Loop.h"

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
        else if(e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    typeSelected = Sand;
                    break;
                case SDLK_2:
                    typeSelected = Stone;
                    break;
                case SDLK_BACKQUOTE:
                    typeSelected = Air;
                    break;
                case SDLK_0:
                    for(int i = 0; i < numObjects; i++){
                        objects[i] = (Object){
                            .type = Air,
                            .color = (SDL_Color){0x00, 0x00, 0x00, 0x00}
                        };
                    }
                    break;
                case SDLK_EQUALS:
                    brush_size++;
                    break;
                case SDLK_UNDERSCORE:
                    if(brush_size>0)brush_size--;
                    break;
            }
        }
    }
    return true;
}