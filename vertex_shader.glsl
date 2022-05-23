#version 330 core

layout (location = 0) in vec3 position;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    ourColor = vec3(0.5, 0.01, 0.5);
}
