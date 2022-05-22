#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

int aspectRatio = 1;
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void windowResizeCallback(GLFWwindow *window, int width, int height);

void initOpenglProgram(GLFWwindow *window);
void freeOpenglProgram(GLFWwindow *window);
void drawScene(GLFWwindow *window);

int main(int argc, char *argv[]) {
  GLFWwindow *window;
  if (!glfwInit()) {
    fprintf(stderr, "Can't initalize GLFW\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(500, 500, "Snake3D", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (int err = glewInit() != GLEW_OK) {
    fprintf(stderr, "Can't initalize GLEW: %s\n", glewGetErrorString(err));
    return -1;
  }

  while (!glfwWindowShouldClose(window)) {
    initOpenglProgram(window);
    drawScene(window);
    glfwPollEvents();
  }

  freeOpenglProgram(window);
  glfwDestroyWindow(window); // Usuń kontekst OpenGL i okn
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

  return;
}

void freeOpenglProgram(GLFWwindow *window) { return; }

void drawScene(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);
  return;
}
