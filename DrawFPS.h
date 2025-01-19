#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Units.h"

#define MAX_NAME_LENGTH 15

void DrawFPS(Vector2D pos, TTF_Font *font, SDL_Renderer *renderer, SDL_Color color, float deltaTime);