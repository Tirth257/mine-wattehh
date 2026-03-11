#version 410 core
layout (triangles, equal_spacing, ccw) in;

uniform mat4 view;
uniform mat4 perspective;
uniform float time;

in int tcs_blockType[];
in int tcs_normalIndex[];

// Final outputs to the Fragment Shader
flat out int blockType;
out vec3 OutNormal;
out vec3 fragPos;

void main() {
    // 1. Interpolate Position
    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
    vec3 pos = p0 + p1 + p2;

    // 2. Wave Displacement
    float freq = 2.0;
    float amp = 0.15;
    pos.y += sin(pos.x * freq + time) * amp;
    pos.y += cos(pos.z * freq * 0.5 + time) * (amp * 0.5);

    // 3. Send data to Fragment Shader
    fragPos = pos;
    blockType = tcs_blockType[0]; // Take type from the first vertex of the patch
    
    // 4. Calculate Wave Normal
    float dx = cos(pos.x * freq + time) * freq * amp;
    float dz = -sin(pos.z * freq * 0.5 + time) * (freq * 0.5 * amp * 0.5);
    OutNormal = normalize(vec3(-dx, 1.0, -dz));

    gl_Position = perspective * view * vec4(pos, 1.0);
}