#version 330 core

uniform sampler2D textureSampler;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
out vec4 pixelColor;

void main()
{
    vec4 color = texture(textureSampler, TexCoord);
    pixelColor=vec4(color.rgb,color.a);
}
