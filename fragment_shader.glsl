#version 330 core

out vec4 pixelColor;
uniform sampler2D textureSampler;

in vec4 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Light {
    vec3 position;  
    vec3 diffuse;  
  
    vec3 ambient;
	
    float constant;
    float linear;
    float quadratic;
};

void main()
{
    Light light;
    // light.position = vec3(0.f, 0.f, 0.f);
    light.position = vec3(-6.25f, 0.311f, -5.625f);
    light.diffuse = vec3(0.5f, 0.5f, 0.5f);
    light.ambient = vec3(0.4f, 0.4f, 0.4f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    // ambient
    vec3 ambient = light.ambient * texture(textureSampler, TexCoord).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal.rgb);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(textureSampler, TexCoord).rgb;  
	
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
        
    vec3 result = ambient + diffuse;
    pixelColor = vec4(result, 1.0f);
}
