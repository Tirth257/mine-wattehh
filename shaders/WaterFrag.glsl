#version 410 core

uniform vec3 lightColor;
uniform vec3 light_pos;
uniform vec3 cam_pos;
flat in int blockType;
in vec3 OutNormal;
in vec3 fragPos;
//in vec3 camPos;

out vec4 FragColor;
void main(){
    
    vec3 objectColor =  vec3(0.1,0.1,0.1)*blockType*2;

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
    vec3 result =  objectColor*(ambient) +specular;
    FragColor = vec4(result, 0.9);
}