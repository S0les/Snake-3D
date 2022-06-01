#version 330 core

uniform mat4 model;//Матрица модели
uniform mat4 view;//Матрица вида
uniform mat4 projection;//Матрица проекции

in vec3 position;
in vec2 texCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = texCoord;
}
