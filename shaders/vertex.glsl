#version 330 core

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in uint type_face;

uniform mat4 view;
uniform mat4 perspective;

flat out int blockType;
out vec3 OutNormal;
out vec2 UV;
out vec3 fragPos;

const vec3 FACE_N[6] = vec3[](
    vec3( 1, 0, 0),
    vec3(-1, 0, 0),
    vec3( 0, 1, 0),
    vec3( 0,-1, 0),
    vec3( 0, 0, 1),
    vec3( 0, 0,-1)
);

const vec2 uvTable[4] = vec2[](
    
    vec2(0.0, 0.0),  // 0 = BL
    vec2(1.0, 0.0),  // 1 = BR
    vec2(0.0, 1.0),  // 2 = TL
    vec2(1.0, 1.0)   // 3 = TR
);
// +x , -x , +Y,-Y,+Z,-Z
const vec2 typeOffset[24] = vec2[](

    // AIR
    vec2(0,0), vec2(0,0), vec2(0,0),
    vec2(0,0), vec2(0,0), vec2(0,0),

    // DIRT
    vec2(0,1), vec2(0,1), vec2(0,2),
    vec2(0,0), vec2(0,1), vec2(0,1),

    // GRASS
    vec2(2,0), vec2(2,0), vec2(1,0),
    vec2(0,0), vec2(2,0), vec2(2,0),

    // STONE
    vec2(5,0), vec2(5,0), vec2(5,0),
    vec2(5,0), vec2(5,0), vec2(5,0)
);

void main()
{
    uint face =  type_face        & 7u;   // bits 0-2
    uint uv   = (type_face >> 3)  & 3u;   // bits 3-4
    uint type = (type_face >> 5);         // bits 5+

    vec2 tile = typeOffset[type * 6u + face];
    UV = (tile + uvTable[uv]) / 16.0;


    blockType = int(type);
    OutNormal = FACE_N[face];

    fragPos = vert_pos;

    gl_Position = perspective * view * vec4(vert_pos, 1.0);
}
