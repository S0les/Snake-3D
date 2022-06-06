#version 330 core

uniform sampler2D textureSampler;

in vec2 TexCoord;
in float i_nl;
out vec4 pixelColor;

void main()
{
    vec4 color = texture(textureSampler, TexCoord);
    pixelColor=vec4(color.rgb*i_nl,color.a);
}
