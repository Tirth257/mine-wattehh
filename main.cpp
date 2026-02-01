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
#include "ChunkManager.h"
#include"Player.h"

unsigned int program;
int Width = 1000;
int Height = 600;

ChunkManager chkMang(10);

GLint camMatrix_pos;
glm::vec3 cam_pos(0.0f, 50.0f, 0.0f);
glm::vec3 cam_dir(0, 0, -1);
Camera cam(cam_pos, cam_dir, Width, Height);

Player player("steve",&cam,&chkMang);

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
void mouse_callback(int button, int state, int x, int y) {
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            std::cout << "Left Click at: " << x << ", " << y << std::endl;
        player.pickBlock();

        }
    }

}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 displacenemt = glm::vec3(keys['d'] - keys['a'], 0, keys['w'] - keys['s']) * 0.1f;

    cam.Translate(displacenemt);

    glUseProgram(program);
    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.getCameraMatrix()));

    chkMang.draw();

    glutSwapBuffers();
}
void mm(int x, int y)
{
    cam.mouseMove(x, y);
}
int main(int argc, char *argv[])
{

    glutInit(&argc, &argv[0]);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Voxels");
    glewInit();


    // Shader Creation
    Shader shader_1("vertex.glsl", "fragment.glsl");
    shader_1.use();
    program = shader_1.ID;

    camMatrix_pos = glGetUniformLocation(program, "cam_m");
    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.getCameraMatrix()));

    // Chunk
    chkMang.genMapping();
    // TESTING

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mm);
    glutMouseFunc(mouse_callback);
    glutMainLoop();
    return 0;
}
