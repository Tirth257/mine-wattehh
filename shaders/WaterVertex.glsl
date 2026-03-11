#version 330 core

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in int inType;
layout(location = 2) in int normal_index;

uniform mat4 view;
uniform mat4 perspective;

flat out int blockType;
out vec3 OutNormal;

const vec3 FACE_N[6] = vec3[](
    vec3( 1, 0, 0),
    vec3(-1, 0, 0),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 0, 0, 1),
    vec3( 0, 0,-1)
);

out vec3 fragPos;
void main(){
    blockType=inType;
    fragPos =vert_pos;
    OutNormal =FACE_N[normal_index];

    gl_Position = perspective*view*vec4(vert_pos,1);
}