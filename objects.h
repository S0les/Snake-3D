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

extern GLuint map_texture;
extern GLuint column_texture;
extern GLuint fence_texture;
extern GLuint snake_texture;
extern float snake_rotate_angle;
extern float snake_speed;

GLuint loadTexture(const char *filepath);
void initObjects(void);
void generateObjects(void);
void generateMap(ShaderProgram *basicShader);
void generateSnake(ShaderProgram *basicShader);
void generateFence(ShaderProgram *basicShader, int fenceNumber);
void generateColumn(ShaderProgram *basicShader, int columnNumber);
#endif
