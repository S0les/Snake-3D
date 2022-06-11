#version 330 core
in vec4 position;

uniform mat4 lamp_model;
uniform mat4 lamp_view;
uniform mat4 lamp_projection;


in vec2 texCoord;

out vec2 TexCoord;

void main()
{   
    gl_Position = lamp_projection * lamp_view * lamp_model * position;
    TexCoord = texCoord;

}
