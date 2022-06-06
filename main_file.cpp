#include "Camera.h"
#include "objects.h"
#include "shaderprogram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdlib.h>
int aspectRatio = 1;
const GLuint WIDTH = 1080, HEIGHT = 800;
bool keys[1024];

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
void do_movement(void);

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
  if (key == GLFW_KEY_ESCAPE && action != GLFW_RELEASE)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if ((key == GLFW_KEY_D) && action != GLFW_RELEASE) {
    snake_rotate_angle += 0.0523599;
  }

  if ((key == GLFW_KEY_A) && action != GLFW_RELEASE) {
    snake_rotate_angle -= 0.0523599;
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
  initObjects();
  return;
}

void freeOpenglProgram(GLFWwindow *window) {
  glDeleteTextures(1, &map_texture);
  glDeleteTextures(1, &fence_texture);
  glDeleteTextures(1, &column_texture);
  glfwDestroyWindow(window);
  return;
}

void drawScene(GLFWwindow *window) {
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  basicShader->use();
  generateObjects();
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

void lookAt() {
  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 view = glm::mat4(1.0f);
  view = camera.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));
  view = glm::rotate(view, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, snake_rotate_angle, glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.f, -1.5f, 0.f));
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
