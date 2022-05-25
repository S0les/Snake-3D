#version 330 core

uniform mat4 model;//Матрица модели
uniform mat4 view;//Матрица вида
uniform mat4 projection;//Матрица проекции

in vec3 position;
in vec2 textureCoords;
in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoordMap;
out vec2 TexCoordFence;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    ourColor = vec3(0.5, 0.01, 0.5);
    TexCoordMap = textureCoords;
    TexCoordFence = texCoord;
}
