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

struct Snake {
  float snake_coords[2];
  float snake_rotate_angle;
  float snake_pos_old[13][2];
  float snake_rotate_angle_old[13];
  GLuint snake_texture;
  float snake_history[10];
};

extern int snake_total;
extern float apple_coords[2];
extern Snake SnakeData[1024];
extern GLuint map_texture;
extern GLuint column_texture;
extern GLuint fence_texture;
extern float snake_speed;

GLuint loadTexture(const char *filepath);
void initObjects(void);
void reset_snake(void);
void check_collision(void);
void update_snake_coords(void);
void snake_save_old_angle(void); 
void generateObjects(void);
void generateRing(ShaderProgram *basicShader);
void generateMap(ShaderProgram *basicShader);
void generateSnake(ShaderProgram *basicShader, Snake curr_snake);
void generateFence(ShaderProgram *basicShader, int fenceNumber);
void generateColumn(ShaderProgram *basicShader, int columnNumber);
#endif
