#version 330 core

uniform mat4 model;//Матрица модели
uniform mat4 view;//Матрица вида
uniform mat4 projection;//Матрица проекции

uniform vec4 lightDir=vec4(0,0,1,0);

in vec4 normal;
in vec4 position;
in vec2 texCoord;

out vec2 TexCoord;
out float i_nl;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    mat4 G=mat4(inverse(transpose(mat3(model))));
    vec4 n=normalize(view*G*normal)
    
    i_nl=clamp(dot(n,lightDir),0,1);


    TexCoord = texCoord;
}
