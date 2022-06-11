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

vec3 generate_light(vec3 position, vec3 FragPos, vec4 Normal, vec2 TexCoord, sampler2D textureSampler){
    Light light;
    light.position = position;
    light.diffuse = vec3(0.2f, 0.2f, 0.2f);
    light.ambient = vec3(0.6f, 0.6f, 0.6f);
    light.constant = 1.0f;
    light.linear = 0.35f;
    light.quadratic = 0.44f;

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
    return result;
}

vec3 generate_light_boxes(vec3 position, vec3 FragPos, vec4 Normal, vec2 TexCoord, sampler2D textureSampler){
    Light light;
    light.position = position;
    light.diffuse = vec3(0.05f, 0.05f, 0.05f);
    light.ambient = vec3(0.8f, 0.8f, 0.8f);
    light.constant = 1.0f;
    light.linear = 0.7f;
    light.quadratic = 1.8f;

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
    return result;
}

const vec3 positions[5] = vec3[5](
    vec3(0.0f, 1.311f, 0.625f),
    vec3(-10.56f, 1.5f, 10.551f),
    vec3(9.939f,  1.5f, 9.939f),
    vec3(9.939f,  1.5f, -10.585f),
    vec3(10.551f, 1.5f, 9.939f)
);

void main()
{
    vec3 result = vec3(0.f, 0.f, 0.f);
    // Center light
    result += generate_light_boxes(vec3(0.0f, 0.0f, 0.625f), FragPos, Normal, TexCoord, textureSampler);

    // Columns
    result += generate_light(vec3(-10.56f, 3.f, 10.551f), FragPos, Normal, TexCoord, textureSampler);
     result += generate_light(vec3(9.939f, 3.f, -10.585f), FragPos, Normal, TexCoord, textureSampler);
     result += generate_light(vec3(9.939f, 3.f, 9.939f), FragPos, Normal, TexCoord, textureSampler);
     result += generate_light(vec3(-10.551f, 3.f, -9.939f), FragPos, Normal, TexCoord, textureSampler);

    // Boxes
    result += generate_light_boxes(vec3(5.625f, 0.0f, 5.625f), FragPos, Normal, TexCoord, textureSampler);
    result += generate_light_boxes(vec3(-6.25f,0.0f, -5.625f), FragPos, Normal, TexCoord, textureSampler);
    result += generate_light_boxes(vec3(5.625f, 0.0f, -5.625f), FragPos, Normal, TexCoord, textureSampler);
    result += generate_light_boxes(vec3(-6.25f,0.0f, 5.625f), FragPos, Normal, TexCoord, textureSampler);

    pixelColor = vec4(result, 1.0f);
}
