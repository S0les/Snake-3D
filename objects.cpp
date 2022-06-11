#include "objects.h"
#include <vector>
#include <GLFW/glfw3.h>

//----------------------------
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//---------------------------

GLuint textureSampler;
//-----------Assimp----------------
std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> texCoords;
std::vector<unsigned int> indices;
//----------------------------
#include "shaderprogram.h"
#include <cstdio>
#include <math.h>

GLuint map_texture;
GLuint column_texture;
GLuint fence_texture;
GLuint snake_texture;

float snake_speed = 0.05f;
int snake_total;
Snake SnakeData[1024];

void initObjects(void) {
  map_texture = loadTexture("images/map-texture.png");
  fence_texture = loadTexture("images/bricks.png");
  column_texture = loadTexture("images/bricks.png");
  snake_texture = loadTexture("images/snake.jpg");
  textureSampler = loadTexture("images/apple_tex.jpg");
  reset_snake();
}

void reset_snake(void) {
  for (int i = 0; i < 1024; i++) {
    SnakeData[i].snake_coords[0] = 0.f;
    SnakeData[i].snake_coords[1] = 0.f;
    SnakeData[i].snake_rotate_angle = 0.f;
    for (int j = 0; j < 13; j++) {
      SnakeData[i].snake_rotate_angle_old[j] = 0.f;
      SnakeData[i].snake_pos_old[j][0] = 0.f;
      SnakeData[i].snake_pos_old[j][1] = 0.f;
    }
    SnakeData[i].snake_texture = snake_texture;
  }
  snake_total = 1;
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

void generateObjects() {
  generateRing(basicShader);
  for (int i = 0; i < snake_total; i++)
    generateSnake(basicShader, SnakeData[i]);
  generateMap(basicShader);
  for (int i = 0; i < 4; i++) {
    generateFence(basicShader, i);
    generateColumn(basicShader, i);
  }
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
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        fence_tex_coords);

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

void loadModel(std::string plik){

        using namespace std;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(plik,aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_GenSmoothNormals);
        cout << importer.GetErrorString() << endl;
        aiMesh* mesh = scene->mMeshes[0];

        for(int i = 0; i < mesh ->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];
            verts.push_back(glm::vec4(vertex.x,vertex.y,vertex.z,  1));

            aiVector3D normal = mesh->mNormals[i];
            norms.push_back(glm::vec4(normal.x,normal.y,normal.z,  0));

            aiVector3D texCoord = mesh->mTextureCoords[0][i];
            texCoords.push_back(glm::vec2(texCoord.x,texCoord.y));
        }
        for (int i = 0 ; i < mesh->mNumFaces; i++){
            aiFace& face = mesh->mFaces[i];

            for(int j = 0; j < face.mNumIndices; j++ )
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        for(int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE);i++)
        {
            aiString str;
            material -> GetTexture(aiTextureType_DIFFUSE,i,&str);
        }
}

void generateRing(ShaderProgram *basicShader){

     glEnableVertexAttribArray(basicShader->attrib("position"));
     glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0, verts.data()); //Współrzędne wierzchołków bierz z tablicy birdVertices

     glEnableVertexAttribArray(basicShader->attrib("texCoord"));
     glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data()); //Współrzędne wierzchołków bierz z tablicy birdColors

      glEnableVertexAttribArray(basicShader->attrib("normal"));
      glVertexAttribPointer(basicShader->attrib("normal"), 4, GL_FLOAT, false, 0, norms.data()); //Współrzędne wierzchołków bierz  z tablicy birdColors

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, textureSampler);
     glUniform1i(basicShader->uniform("textureSampler"),0);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model,(GLfloat)glfwGetTime() * 1.0f,glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::scale(model,glm::vec3(0.311f, 0.311f, 0.311f));
      glUniformMatrix4fv(basicShader->uniform("model"), 1, false,glm::value_ptr(model));


     glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, indices.data());

     glDisableVertexAttribArray(basicShader->attrib("vertex"));
     glDisableVertexAttribArray(basicShader->attrib("texCoord"));
     glDisableVertexAttribArray(basicShader->attrib("normal"));
 }

void generateSnake(ShaderProgram *basicShader, Snake snake_current) {
  glEnableVertexAttribArray(basicShader->attrib("position"));
  glVertexAttribPointer(basicShader->attrib("position"), 4, GL_FLOAT, false, 0,
                        cube_vertices);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, snake_current.snake_texture);
  glUniform1i(basicShader->uniform("textureSampler"), 0);

  glm::mat4 model = glm::mat4(1.0f);
  model =
      glm::translate(model, glm::vec3(-snake_current.snake_coords[0], 0.311f,
                                      -snake_current.snake_coords[1]));
  model = glm::scale(model, glm::vec3(0.311f, 0.311f, 0.311f));
  model = glm::rotate(model, -snake_current.snake_rotate_angle,
                      glm::vec3(0.f, 1.f, 0.f));
  glUniformMatrix4fv(basicShader->uniform("model"), 1, false,
                     glm::value_ptr(model));

  glEnableVertexAttribArray(basicShader->attrib("texCoord"));
  glVertexAttribPointer(basicShader->attrib("texCoord"), 2, GL_FLOAT, false, 0,
                        cube_tex_coords);

  glDrawArrays(GL_TRIANGLES, 0, cube_vertexcount);
  glDisableVertexAttribArray(basicShader->attrib("position"));
  glDisableVertexAttribArray(basicShader->attrib("texCoord"));
}

void update_snake_coords() {
  SnakeData[0].snake_coords[0] =
      SnakeData[0].snake_coords[0] +
      snake_speed * cos(SnakeData[0].snake_rotate_angle + 1.5708f);
  SnakeData[0].snake_coords[1] =
      SnakeData[0].snake_coords[1] +
      snake_speed * sin(SnakeData[0].snake_rotate_angle + 1.5708f);
  SnakeData[0].snake_rotate_angle_old[0] = SnakeData[0].snake_rotate_angle;
  SnakeData[0].snake_pos_old[0][0] = SnakeData[0].snake_coords[0];
  SnakeData[0].snake_pos_old[0][1] = SnakeData[0].snake_coords[1];
  for (int i = 1; i < 1024; i++) {
    SnakeData[i].snake_coords[0] = SnakeData[i].snake_pos_old[0][0];
    SnakeData[i].snake_coords[1] = SnakeData[i].snake_pos_old[0][1];
    SnakeData[i].snake_rotate_angle = SnakeData[i].snake_rotate_angle_old[0];
  }
}

void snake_save_old_angle(void) {
  for (int i = 1023; i > -1; i--) {
    for (int j = 12; j > 0; j--) {
      SnakeData[i].snake_rotate_angle_old[j] =
          SnakeData[i].snake_rotate_angle_old[j - 1];
      SnakeData[i].snake_pos_old[j][0] = SnakeData[i].snake_pos_old[j - 1][0];
      SnakeData[i].snake_pos_old[j][1] = SnakeData[i].snake_pos_old[j - 1][1];
    }
    if (i != 0) {
      SnakeData[i].snake_rotate_angle_old[0] =
          SnakeData[i - 1].snake_rotate_angle_old[12];
      SnakeData[i].snake_pos_old[0][0] = SnakeData[i - 1].snake_pos_old[12][0];
      SnakeData[i].snake_pos_old[0][1] = SnakeData[i - 1].snake_pos_old[12][1];
    }
  }
}

void check_collision() {
  float x = SnakeData[0].snake_coords[0];
  float y = SnakeData[0].snake_coords[1];
  if (y < -9.4f || y > 10.f || x < -9.4f || x > 10.f) {
    reset_snake();
    return;
  }
  for (int i = 1; i < snake_total; i++) {
    double squared_x = pow((SnakeData[i].snake_coords[0] - x), 2);
    double squared_y = pow((SnakeData[i].snake_coords[1] - y), 2);
    double distance = sqrt(squared_y + squared_x);
    if (distance < 0.6f) {
      reset_snake();
      return;
    }
  }
}
