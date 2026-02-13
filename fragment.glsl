#version 330 core

uniform vec3 lightColor;
uniform vec3 light_pos;
uniform vec3 cam_pos;
flat in int blockType;
in vec3 OutNormal;
in vec3 fragPos;
//in vec3 camPos;
vec3 blockColors[4] = vec3[4](
    vec3(0.0), 
    vec3(0.2, 0.8, 0.2), 
    vec3(0.55, 0.27, 0.07), 
    vec3(0.5, 0.5, 0.5)
);

out vec4 FragColor;
void main(){
    
    vec3 objectColor = blockColors[blockType];

    vec3 lightDir = normalize(light_pos - fragPos);

    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(OutNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular

    float specStrength = 0.3;
    vec3 viewDir = normalize(cam_pos-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec3 specular = specStrength * spec *lightColor;


    // calculate result
    vec3 result =  objectColor*(ambient +diffuse) +specular;
    FragColor = vec4(result, 1.0);
}