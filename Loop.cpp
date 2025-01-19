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
            if(e.key.keysym.sym == SDLK_1){
                typeSelected = Sand;
            }
            else if(e.key.keysym.sym == SDLK_2){
                typeSelected = Stone;
            }
            else if(e.key.keysym.sym == SDLK_BACKQUOTE){
                typeSelected = Air;
            }else if(e.key.keysym.sym == SDLK_0){
                for(int i = 0; i < numObjects; i++){
                    objects[i] = (Object){
                        .type = Air,
                        .color = (SDL_Color){0x00, 0x00, 0x00, 0x00}
                    };
                }
            }
        }
    }
    return true;
}