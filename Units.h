#pragma once

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 6
#define BRUSH_PROBABILITY 0.2f
#define EROSION_INVERSE_STRENGTH 1
// #define FPS_LIMIT 10

inline int brush_size = 2;
#define BRUSH_SIZE brush_size

struct Vector2D{
    int x,y;
};