#version 330 core
#define NR_LIGHT_ROW 11
#define NR_LIGHTS NR_LIGHT_ROW * NR_LIGHT_ROW
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[NR_LIGHTS];

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient shading
    vec3 ambient = light.ambient * material.diffuse;

    // diffuse shading
    vec3 lightDir = normalize(light.position - fragPos);
    float diff    = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * (diff * material.diffuse);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = light.specular * (spec * material.specular);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
} 


void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0);

    for(int i = 0; i < NR_LIGHTS; i++)
        result += CalcPointLight(lights[i], normal, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
