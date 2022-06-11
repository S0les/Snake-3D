#version 330 core

uniform mat4 model;//Матрица модели
uniform mat4 view;//Матрица вида
uniform mat4 projection;//Матрица проекции

uniform vec4 lightDir=vec4(1,1,1,1);

in vec4 normal;

in vec4 position;
in vec2 texCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * position;
    
    mat4 G=mat4(inverse(transpose(mat3(model))));
    vec4 n=normalize(view*G*normal);
    

    TexCoord = texCoord;
}
