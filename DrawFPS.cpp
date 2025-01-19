#include "DrawFPS.h"

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