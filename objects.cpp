#include "objects.h"

GLuint map_texture;
GLuint column_texture;
GLuint fence_texture;
GLuint snake_texture;
SnakeInfo snakeData[1024];

void initObjects(void){
  map_texture = loadTexture("images/map-texture.png");
  fence_texture = loadTexture("images/bricks.png");
  column_texture = loadTexture("images/bricks.png");
  snake_texture = loadTexture("images/snake.jpg");
  for (int i = 0; i < 1024; i++) {
    for (int j = 0; j < 2; j++)
      snakeData[i].snake_coords[j] = 0.f;
    snakeData[i].texture = snake_texture;
  }
  snakeData[1].snake_coords[1] = -0.622;
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

void generateMap(ShaderProgram *basicShader) {
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

void generateFence(ShaderProgram *basicShader, int fenceNumber) {
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
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0, fence_tex_coords);

  glDrawArrays(GL_TRIANGLES, 0, fence_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void generateColumn(ShaderProgram *basicShader, int columnNumber) {
  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        column_vertices);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, column_texture);
  glUniform1i(basicShader->uniform("textureSampler"), 0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, columnPositions[columnNumber]);
  if (columnNumber > 1)
    model = glm::rotate(model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(0.25f, 1.0f, 0.25f));
  glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                     glm::value_ptr(model));

  glEnableVertexAttribArray(basicShader->attrib("texCoord"));
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        column_tex_coords);

  glDrawArrays(GL_TRIANGLES, 0, column_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void generateSnake(ShaderProgram *basicShader, int total_snake) {
  for (int i = 0; i < total_snake; i++) {
    glEnableVertexAttribArray(basicShader->attrib("position"));
    glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false,
                          0, cube_vertices);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, snakeData[i].texture);
    glUniform1i(basicShader->uniform("textureSampler"), 0);

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::translate(model, glm::vec3(snakeData[i].snake_coords[0], 0.311f,
                                        snakeData[i].snake_coords[1]));
    model = glm::scale(model, glm::vec3(0.311f, 0.311f, 0.311f));
    model = glm::rotate(model, -snakeData[i].rotate_angle,
                        glm::vec3(0.f, 1.f, 0.f));
    glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                       glm::value_ptr(model));

    glEnableVertexAttribArray(basicShader->attrib("texCoord"));
    glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false,
                          0, cube_tex_coords);

    glDrawArrays(GL_TRIANGLES, 0, cube_vertexcount);
    glDisableVertexAttribArray(basicShader->attrib("position"));
    glDisableVertexAttribArray(basicShader->attrib("texCoord"));
  }
}
