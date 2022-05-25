#ifndef MAP_H
#define MAP_H
float map_vertices[] = {
    10.0f, -10.0f, 0.0f, 1.0f,
    -10.0f, 10.0f, 0.0f, 1.0f,
    -10.0f, -10.0f, 0.0f, 1.0f,

    10.0f, -10.0f, 0.0f, 1.0f,
    10.0f,  10.0f, 0.0f, 1.0f,
    -10.0f, 10.0f,  0.0f, 1.0f,
};

float map_tex_coords[] = {
    3.0f, 0.0f, 0.0f,
    3.0f, 0.0f, 0.0f,
    3.0f, 0.0f, 3.0f,
    3.0f, 0.0f, 3.0f,
};

int map_vertexcount = 6;
#endif
