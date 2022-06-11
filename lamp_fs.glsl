#version 330 core

uniform sampler2D textureSampler;

in vec2 TexCoord;
out vec4 color;

void main()
{
    color = texture(textureSampler, TexCoord);
}
