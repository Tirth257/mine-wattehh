#version 330 core

flat in int blockType;
vec3 blockColors[4] = vec3[4](
    vec3(0.0), 
    vec3(0.2, 0.8, 0.2), 
    vec3(0.55, 0.27, 0.07), 
    vec3(0.5, 0.5, 0.5)
);
out vec4 FragColor;
void main(){
    FragColor = vec4(blockColors[blockType],1);
}