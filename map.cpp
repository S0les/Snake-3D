#include "map.h"

float map_vertices[] = {
    10.0f, -10.0f, 0.0f,   1.0f,   -10.0f, 10.0f,
    0.0f,  1.0f,   -10.0f, -10.0f, 0.0f,   1.0f,

    10.0f, -10.0f, 0.0f,   1.0f,   10.0f,  10.0f,
    0.0f,  1.0f,   -10.0f, 10.0f,  0.0f,   1.0f,
};

float map_tex_coords[] = {
    8.0f, 0.0f, 0.0f, 8.0f, 0.0f, 0.0f, 8.0f, 0.0f, 8.0f, 8.0f, 0.0f, 8.0f,
};

int map_vertexcount = 6;
