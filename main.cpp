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
glm::vec3 cam_pos(30.0f, 0.0f, 0.0f);
glm::vec3 cam_dir(0, 0, -1);
Camera cam(cam_pos, cam_dir, Width, Height);



bool keys[256] = {false};
void keyDown(unsigned char key, int x, int y)
{
    keys[key] = true;
}
void keyUp(unsigned char key, int x, int y)
{
    keys[key] = false;
}

float prevX = 0;
int prevY = 0;


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 displacenemt = glm::vec3(keys['d'] - keys['a'], 0, keys['w'] - keys['s']) * 0.1f;

    cam.Translate(displacenemt);

    glUseProgram(program);
    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.getCameraMatrix()));

    chunk1.draw();

    glutSwapBuffers();
}
void mm(int x, int y){
    cam.mouseMove(x,y);
}
int main(int argc, char *argv[])
{
    glutInit(&argc, &argv[0]);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Voxels");
    glewInit();

    // Camera Creation

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
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mm);
    glutMainLoop();
    return 0;
}
