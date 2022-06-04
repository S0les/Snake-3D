#include "Camera.h"
#include "cube.h"
#include "fence.h"
#include "map.h"
#include "shaderprogram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdlib.h>
int aspectRatio = 1;
int state = 1;
int coord_index = 1;
float distance = -0.05f;
float rotate_angle = 0.f;
float snake_coords[2] = {0.f, 0.f};
GLuint map_texture;
GLuint fence_texture;
GLuint snake_texture;

const GLuint WIDTH = 1080, HEIGHT = 800;
bool keys[1024];

// Camera , function LookAt
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void lookAt();
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void windowResizeCallback(GLFWwindow *window, int width, int height);
void initOpenglProgram(GLFWwindow *window);
void freeOpenglProgram(GLFWwindow *window);
void drawScene(GLFWwindow *window);
void initWindow(GLFWwindow *window);
void generateMap(void);
void generateSnake(void);
void generateFence(int fenceNumber);
void do_movement(void);
void update_direction(float angle);

GLuint loadTexture(const char *filepath);

int main(int argc, char *argv[]) {
  GLFWwindow *window;
  if (!glfwInit()) {
    fprintf(stderr, "Can't initalize GLFW\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(WIDTH, HEIGHT, "Snake3D", NULL, NULL);
  initWindow(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (int err = glewInit() != GLEW_OK) {
    fprintf(stderr, "Can't initalize GLEW: %s\n", glewGetErrorString(err));
    return -1;
  }

  initShaders();

  initOpenglProgram(window);

  while (!glfwWindowShouldClose(window)) {

    lookAt();
    drawScene(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwPollEvents();
    do_movement();
  }

  freeShaders();
  freeOpenglProgram(window);
  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if ((key == GLFW_KEY_D) && action == GLFW_PRESS) {
    if (state == 1)
      distance *= -1.0f;
    update_direction(1.5708f);
  }

  if ((key == GLFW_KEY_A) && action == GLFW_PRESS) {
    if (state == 0)
      distance *= -1.0f;
    update_direction(-1.5708f);
  }

  //  if (key >= 0 && key < 1024) {
  //    if (action == GLFW_PRESS)
  //      keys[key] = true;
  //    else if (action == GLFW_RELEASE)
  //      keys[key] = false;
  // }
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {
  if (height == 0)
    return;
  aspectRatio = (float)width / (float)height;
  glViewport(0, 0, width, height);
}

void initOpenglProgram(GLFWwindow *window) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glfwSetWindowSizeCallback(window, windowResizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetKeyCallback(window, key_callback);
  map_texture = loadTexture("images/map-texture.png");
  fence_texture = loadTexture("images/bricks.png");
  snake_texture = loadTexture("images/snake.jpg");
  return;
}

void freeOpenglProgram(GLFWwindow *window) {
  glDeleteTextures(1, &map_texture);
  glDeleteTextures(1, &fence_texture);
  glfwDestroyWindow(window);
  return;
}

void drawScene(GLFWwindow *window) {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  basicShader->use();
  generateMap();
  for (int i = 0; i < 4; i++)
    generateFence(i);
  snake_coords[coord_index] += distance;
  generateSnake();
  for (int i = 0; i < 2; i++) {
    if (snake_coords[i] > 9.49f || snake_coords[i] < -10.12f) {
      snake_coords[0] = 0.f;
      snake_coords[1] = 0.f;
      coord_index = 1;
      state = 1;
      rotate_angle = 0.f;
      distance = -0.05f;
    }
  }
  glfwSwapBuffers(window);
  return;
}

void load_favicon(GLFWwindow *window) {
  GLFWimage icons[1];
  icons[0].pixels = SOIL_load_image("images/favicon.png", &icons[0].width,
                                    &icons[0].height, 0, SOIL_LOAD_RGBA);
  glfwSetWindowIcon(window, 1, icons);
  SOIL_free_image_data(icons[0].pixels);
  return;
};

void initWindow(GLFWwindow *window) {
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  load_favicon(window);
  return;
}

void generateMap(void) {
  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        map_vertices);

  glEnableVertexAttribArray(basicShader->attrib("texCoord"));
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        map_tex_coords);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, map_texture);
  glUniform1i(basicShader->uniform("textureSampler"), 0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-0.311f, 0.0f, -0.311f));
  model = glm::rotate(model, 1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                     glm::value_ptr(model));

  glDrawArrays(GL_TRIANGLES, 0, map_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void generateFence(int fenceNumber) {
  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        fence_vertices);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fence_texture);
  glUniform1i(basicShader->uniform("textureSampler"), 0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, fencePositions[fenceNumber]);
  if (fenceNumber > 1)
    model = glm::rotate(model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(10.0f, 0.5F, 0.1f));
  glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                     glm::value_ptr(model));

  glEnableVertexAttribArray(basicShader->attrib("texCoord"));
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        fence_tex_coords);

  glDrawArrays(GL_TRIANGLES, 0, fence_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void generateSnake(void) {
  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        cube_vertices);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, snake_texture);
  glUniform1i(basicShader->uniform("textureSampler"), 0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model,
                         glm::vec3(snake_coords[0], 0.311f, snake_coords[1]));
  model = glm::scale(model, glm::vec3(0.311f, 0.311f, 0.311f));
  model = glm::rotate(model, -rotate_angle, glm::vec3(0.f, 1.f, 0.f));
  glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                     glm::value_ptr(model));

  glEnableVertexAttribArray(basicShader->attrib("texCoord"));
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        cube_tex_coords);

  glDrawArrays(GL_TRIANGLES, 0, cube_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void lookAt() {
  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 view = glm::mat4(1.0f);
  view = camera.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));
  view = glm::rotate(view, rotate_angle, glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view,
                        glm::vec3(-snake_coords[0], -1.2f, -snake_coords[1]));
  // view = glm::rotate(view, 1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
  // view = glm::translate(view, glm::vec3(0.0f, -20.0f, 0.0f));
  projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT,
                                0.1f, 100.0f);

  // TODO Refactoring
  GLint modelLoc = glGetUniformLocation(basicShader->shaderProgram, "model");
  GLint viewLoc = glGetUniformLocation(basicShader->shaderProgram, "view");
  GLint projLoc =
      glGetUniformLocation(basicShader->shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void do_movement() {
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

GLuint loadTexture(const char *filepath) {
  GLuint texture;
  int img_width, img_height;
  unsigned char *image =
      SOIL_load_image(filepath, &img_width, &img_height, 0, SOIL_LOAD_RGB);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
  return texture;
}

void update_direction(float angle) {
  if (snake_coords[coord_index] >= 0) {
    snake_coords[coord_index] =
        (ceil(snake_coords[coord_index] / 0.625f)) * 0.625f;
  } else {
    snake_coords[coord_index] =
        (floor(snake_coords[coord_index] / 0.625f)) * 0.625f;
  }
  coord_index = (coord_index + 1) % 2;
  state = (state + 1) % 2;
  rotate_angle += angle;
};
