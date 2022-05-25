#version 330 core

in vec3 ourColor;
in vec2 TexCoordMap;
in vec2 TexCoordFence;

out vec4 mapColor;
out vec4 fenceColor;
uniform sampler2D textureMap;
uniform sampler2D textureFence;

void main()
{
	mapColor = texture(textureMap, TexCoordMap);
    fenceColor = texture(textureFence,TexCoordFence);
}
