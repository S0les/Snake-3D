#ifndef OBJECTS_H
#define OBJECTS_H
#include "column.h"
#include "cube.h"
#include "fence.h"
#include "map.h"
#include "shaderprogram.h"
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SnakeInfo {
  float rotate_angle;
  float snake_coords[2];
  GLuint texture;
  int index;
  int index_old;
};

extern GLuint map_texture;
extern GLuint column_texture;
extern GLuint fence_texture;
extern GLuint snake_texture;
extern SnakeInfo snakeData[];
extern int state;

GLuint loadTexture(const char *filepath);
void initObjects(void);
void generateMap(ShaderProgram *basicShader);
void generateSnake(ShaderProgram *basicShader, int total_snake);
void generateFence(ShaderProgram *basicShader, int fenceNumber);
void generateColumn(ShaderProgram *basicShader, int columnNumber);
void update_direction(float angle, int total_snake, float distance);
#endif
