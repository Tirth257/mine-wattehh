#version 330 core

layout(location = 0) in vec3 vert_pos;

uniform mat4 cam_m;

void main(){
    gl_Position = cam_m*vec4(vert_pos,1);
}