#version 330 core

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in int inType;

uniform mat4 cam_m;
flat out int blockType;
void main(){
    gl_Position = cam_m*vec4(vert_pos,1);
    blockType=inType;
}