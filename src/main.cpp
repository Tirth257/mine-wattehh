#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <GL/freeglut.h>
#include <fstream>
#include "Shaders.h"
#include "stb_image.h"
#include <iostream>
#include "Camera.h"
#include "ChunkManager.h"
#include "Player.h"
#include "TickServer.h"
#include "WaterManager.h"

unsigned int program;
unsigned int water_program;
int Width = 1000;
int Height = 600;

ChunkManager *chunk_manager;
WaterManager *water_manager;

GLint camMatrix_pos;
GLint perspective_matrix_pos;
GLint camPos_uniformLocation;

GLint camMatrix_pos_water;
GLint perspective_matrix_pos_water;
GLint camPos_uniformLocation_water;

glm::vec3 cam_pos(0.0f, 50.0f, 0.0f);
glm::vec3 cam_dir(0, 0, -1);
Camera cam(cam_pos, cam_dir, Width, Height);

Player *player;

GLuint atlas;

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
            player->pickBlock();
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {

            player->PlaceBlock();
        }
    }
}

chrono::duration<double> minDiffWent;

// Ticksss
TickServer waterServer(0.1);

// display
void display()
{
    auto frameStart = chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 displacenemt = glm::vec3(keys['d'] - keys['a'], 0, keys['w'] - keys['s']) * 0.1f;

    cam.Translate(displacenemt);

    cam.GenPerpectiveMatrix();
    cam.GenViewMatrix();

    // Updates Uniforms
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);

    glUniformMatrix4fv(camMatrix_pos, 1, GL_FALSE, glm::value_ptr(cam.viewMatrix));
    glUniformMatrix4fv(perspective_matrix_pos, 1, GL_FALSE, glm::value_ptr(cam.perspectiveMatrix));
    glUniform3fv(camPos_uniformLocation, 1, glm::value_ptr(cam.cameraPos));

    // Updates Uniforms for water shader
    float timeValue = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // 2. Send it to the water shader
    glUseProgram(water_program);
    // GLint timeLoc = glGetUniformLocation(water_program, "time");
    // glUniform1f(timeLoc, timeValue);

    glUniformMatrix4fv(camMatrix_pos_water, 1, GL_FALSE, glm::value_ptr(cam.viewMatrix));
    glUniformMatrix4fv(perspective_matrix_pos_water, 1, GL_FALSE, glm::value_ptr(cam.perspectiveMatrix));
    glUniform3fv(camPos_uniformLocation_water, 1, glm::value_ptr(cam.cameraPos));

    // drawing
    chunk_manager->draw();

    glutSwapBuffers();

    // Frame Time territory

    auto frameEnd = chrono::steady_clock::now();
    minDiffWent = frameEnd - frameStart;

    // Update Servers
    // waterServer.tickMe(minDiffWent);
    if (waterServer.tickMe(minDiffWent))
    {
        water_manager->updateAllWaters();
    }

    chunk_manager->updateMapping();
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

    // var cre
    heightNoise = new FastNoiseLite();
    heightNoise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    chunk_manager = new ChunkManager(&program, &water_program);
    water_manager = new WaterManager(chunk_manager);
    player = new Player("Steve", &cam, chunk_manager, water_manager);

    // Shader Creation
    Shader shader_1("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    shader_1.use();
    program = shader_1.ID;

    camMatrix_pos = glGetUniformLocation(program, "view");
    perspective_matrix_pos = glGetUniformLocation(program, "perspective");
    camPos_uniformLocation = glGetUniformLocation(program, "cam_pos");

    // setup lighting uniform

    GLint lightColor_uniformLocation = glGetUniformLocation(program, "lightColor");
    glUniform3fv(lightColor_uniformLocation, 1, lightColor);
    GLint lightPos_uniformLocation = glGetUniformLocation(program, "light_pos");
    glUniform3fv(lightPos_uniformLocation, 1, lightPos);

    // setup water Shader
    Shader waterShader("../shaders/WaterVertex.glsl", "../shaders/WaterFrag.glsl");
    waterShader.use();
    water_program = waterShader.ID;

    camMatrix_pos_water = glGetUniformLocation(water_program, "view");
    perspective_matrix_pos_water = glGetUniformLocation(water_program, "perspective");
    camPos_uniformLocation_water = glGetUniformLocation(water_program, "cam_pos");

    // setup lighting uniform

    GLint lightColor_uniformLocation_water = glGetUniformLocation(water_program, "lightColor");
    glUniform3fv(lightColor_uniformLocation_water, 1, lightColor);
    GLint lightPos_uniformLocation_water = glGetUniformLocation(water_program, "light_pos");
    glUniform3fv(lightPos_uniformLocation_water, 1, lightPos);

    // Chunk
    chunk_manager->genMapping();

    // WATER

    // Upload Atlas

    shader_1.use();
    glGenTextures(1, &atlas);

    glBindTexture(GL_TEXTURE_2D, atlas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int w, h, c;
    stbi_set_flip_vertically_on_load(true);

    // w = 256 , h = 256 . each block is  16*16f
    unsigned char *data = stbi_load("../Texture/texture.png", &w, &h, &c, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glUniform1i(glGetUniformLocation(program, "atlas"), 0); // setting it at slot 0

    // TESTING

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mm);
    glutMouseFunc(mouse_callback);
    glutMainLoop();
    return 0;
}
