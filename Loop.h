#pragma once
#include <SDL2/SDL.h>

#include "Units.h"
#include "Objects.h"

extern bool mouse_pressed;
extern ObjectType typeSelected;
extern Object* objects;
extern const int numObjects;

bool Loop();