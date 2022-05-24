#include "shaderprogram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdlib.h>
int aspectRatio = 1;
GLuint map_texture;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void windowResizeCallback(GLFWwindow *window, int width, int height);
void initOpenglProgram(GLFWwindow *window);
void freeOpenglProgram(GLFWwindow *window);
void drawScene(GLFWwindow *window);
void initWindow(GLFWwindow *window);
void generateMap();
GLuint loadTexture(const char *filepath);

int main(int argc, char *argv[]) {
  GLFWwindow *window;
  if (!glfwInit()) {
    fprintf(stderr, "Can't initalize GLFW\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(500, 500, "Snake3D", NULL, NULL);
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
    drawScene(window);
    glfwPollEvents();
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
  glfwSetKeyCallback(window, key_callback);
  map_texture = loadTexture("images/map-texture.png");
  return;
}

void freeOpenglProgram(GLFWwindow *window) {
  glDeleteTextures(1, &map_texture);
  glfwDestroyWindow(window);
  return;
}

void drawScene(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  basicShader->use();
  generateMap();
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

void generateMap() {
  // TODO Move definition part of code to the header file
  float vertices[] = {
      1.0f, -1.0f, 0.0f,  1.0f,  -1.0f, 1.0f,
      0.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,

      1.0f, -1.0f, 0.0f,  1.0f,  1.0f,  1.0f,
      0.0f, 1.0f,  -1.0f, 1.0f,  0.0f,  1.0f,
  };

  float tex_coords[] = {
      3.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 3.0f, 0.0f, 3.0f, 3.0f, 0.0f, 3.0f,
  };

  int vertexcount = 6;

  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        vertices);

  glEnableVertexAttribArray(basicShader->attrib("textureCoords"));
  glVertexAttribPointer(basicShader->attrib("textureCoords"), 2, GL_FLOAT,
                        false, 0, tex_coords);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, map_texture);
  glUniform1i(basicShader->uniform("textureMap"), 0);

  glDrawArrays(GL_TRIANGLES, 0, vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("textureCoords"));

  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::rotate(transform, 1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(basicShader->uniform("Transform"), 1, false,
                     glm::value_ptr(transform));
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
  return texture;
}
