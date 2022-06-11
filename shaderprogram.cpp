#include "shaderprogram.h"

ShaderProgram *basicShader;
ShaderProgram *lampShader;

void initShaders() {
  basicShader = new ShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");
  lampShader = new ShaderProgram("lamp_vs.glsl", "lamp_fs.glsl");
}

void freeShaders() { delete basicShader;
                      delete lampShader;}

char *ShaderProgram::readFile(const char *fileName) {
  char *result;
  FILE *file;
  int filesize;

  file = fopen(fileName, "rb");
  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    result = new char[filesize + 1];
    fseek(file, 0, SEEK_SET);
    fread(result, 1, filesize, file);
    result[filesize] = 0;
    fclose(file);
    return result;
  }
  return NULL;
}

GLuint ShaderProgram::loadShader(GLenum shaderType, const char *fileName) {
  // Create shader with given type
  GLuint shader = glCreateShader(shaderType);

  // Read shader from source file
  const GLchar *shaderSource = readFile(fileName);

  // Binding shader code and shader object
  glShaderSource(shader, 1, &shaderSource, NULL);

  glCompileShader(shader);

  delete[] shaderSource;

  // Errors debugging
  GLint infologLength = 0;
  GLchar *infolog;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
  if (infologLength > 1) {
    infolog = new char[infologLength];
    glGetShaderInfoLog(shader, infologLength, NULL, infolog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infolog << std::endl;
    delete[] infolog;
  }

  return shader;
}

ShaderProgram::ShaderProgram(const char *vertexShaderFile,
                             const char *fragmentShaderFile) {
  vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFile);
  fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint infologLength = 0;
  GLchar *infolog;
  // Errors debugging
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);

  if (infologLength > 1) {
    infolog = new char[infologLength];
    glGetProgramInfoLog(shaderProgram, infologLength, NULL, infolog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infolog << std::endl;
    delete[] infolog;
  }
}

GLuint ShaderProgram::attrib(const char *variableName) {
  return glGetAttribLocation(shaderProgram, variableName);
}

GLuint ShaderProgram::uniform(const char *variableName) {
  return glGetUniformLocation(shaderProgram, variableName);
}

void ShaderProgram::use() { glUseProgram(shaderProgram); }

ShaderProgram::~ShaderProgram() {
  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
}
