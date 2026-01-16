#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <GL/freeglut.h>
#include <fstream>
#include "Shaders.h"
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Chunk.h"
unsigned int program;
int Width = 1000;
int Height = 600;
Chunk chunk1;

GLint camMatrix_pos;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);

    chunk1.draw();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, &argv[0]);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Voxels");
    glewInit();

    // Camera Creation
    glm::vec3 cam_pos(30.0f, 2.0f, 0.0f);
    glm::vec3 cam_dir(0, 0, -5);
    Camera cam(cam_pos, cam_dir, Width, Height);
    cam.setRotation(glm::vec3(0,0,0));
    // Shader Creation
    Shader shader_1("vertex.glsl", "fragment.glsl");
    shader_1.use();
    program = shader_1.ID;

    camMatrix_pos = glGetUniformLocation(program, "cam_m");
    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.getCameraMatrix()));

    // Chunk
    chunk1.GenerateTerrain();
    chunk1.buildMesh();
    chunk1.uploadToGPU();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    
    glutMainLoop();
    return 0;
}
