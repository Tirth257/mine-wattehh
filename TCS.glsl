#version 410 core
layout (vertices = 3) out;

in int vs_blockType[];
in int vs_normalIndex[];

out int tcs_blockType[];
out int tcs_normalIndex[];

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    
    // Pass attributes through the array
    tcs_blockType[gl_InvocationID] = vs_blockType[gl_InvocationID];
    tcs_normalIndex[gl_InvocationID] = vs_normalIndex[gl_InvocationID];

    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 16.0;
        gl_TessLevelOuter[0] = 16.0;
        gl_TessLevelOuter[1] = 16.0;
        gl_TessLevelOuter[2] = 16.0;
    }
}