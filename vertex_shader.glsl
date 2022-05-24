#version 330 core

in vec3 position;
in vec2 textureCoords;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    ourColor = vec3(0.5, 0.01, 0.5);
    TexCoord = textureCoords;
}
