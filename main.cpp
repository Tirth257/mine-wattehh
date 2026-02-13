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
#include "Player.h"
unsigned int program;
int Width = 1000;
int Height = 600;

ChunkManager chkMang(10);

GLint camMatrix_pos;
GLint perspective_matrix_pos;
GLint camPos_uniformLocation;
glm::vec3 cam_pos(0.0f, 50.0f, 0.0f);
glm::vec3 cam_dir(0, 0, -1);
Camera cam(cam_pos, cam_dir, Width, Height);

Player player("steve", &cam, &chkMang);

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
void mouse_callback(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            std::cout << "Left Click at: " << x << ", " << y << std::endl;
            player.pickBlock();
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {

            std::cout << "Right Click at: " << x << ", " << y << std::endl;
            player.PlaceBlock();
        }
    }
}

chrono::duration<double> minDiffWent;
void display()
{
    auto frameStart = chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 displacenemt = glm::vec3(keys['d'] - keys['a'], 0, keys['w'] - keys['s']) * 0.1f;

    cam.Translate(displacenemt);

    glUseProgram(program);
    // Updates Uniforms
    cam.GenPerpectiveMatrix();
    cam.GenViewMatrix();
    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.viewMatrix));
    glUniformMatrix4fv(perspective_matrix_pos, 1, GL_FALSE, glm::value_ptr(cam.perspectiveMatrix));

    glUniform3fv(camPos_uniformLocation,1,glm::value_ptr(cam.cameraPos));
    // drawing
    chkMang.draw();

    glutSwapBuffers();

    auto frameEnd = chrono::steady_clock::now();

    minDiffWent = frameEnd - frameStart;

    if (minDiffWent < targetFrameDuration)
    {
        this_thread::sleep_for(targetFrameDuration - minDiffWent);
    }
    double workSec = minDiffWent.count();
    double maxFPS = 1.0 / workSec;

    char title[128];
    snprintf(title, sizeof(title), "Target FPS: %d (%.2f ms) | Max FPS (no sleep): %.1f (%.2f ms)", targetFPS, targetFrameDuration.count() * 1000.0, maxFPS, workSec * 1000.0);

    glutSetWindowTitle(title);
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

    camMatrix_pos = glGetUniformLocation(program, "view");
    perspective_matrix_pos =glGetUniformLocation(program, "perspective");
   

    // setup lighting uniform

    GLint lightColor_uniformLocation = glGetUniformLocation(program, "lightColor");
    glUniform3fv(lightColor_uniformLocation, 1, lightColor);
    GLint lightPos_uniformLocation = glGetUniformLocation(program, "light_pos");
    glUniform3fv(lightPos_uniformLocation, 1, lightPos);
    
    camPos_uniformLocation = glGetUniformLocation(program, "cam_pos");
    // Chunk
    chkMang.genMapping();
    // TESTING

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mm);
    glutMouseFunc(mouse_callback);
    glutMainLoop();
    return 0;
}
