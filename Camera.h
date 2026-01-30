#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
class Camera
{
private:
unsigned int VAO,VBO;
protected:

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 perspectiveMatrix = glm::mat4(1.0f);

    glm::vec3 cam_up = glm::vec3(0, 1, 0);
    glm::vec3 cam_right = glm::vec3(1, 0, 0);

    float FOV = 90;

    float cameraSpeed = 2;

    float prevX = 0;
    float prevY = 0;
    float mouse_Sensivity = 0.3f;
    float yaw = -90;
    float pitch = 0;

public:

    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;
    int width;
    int height;
    Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection, float width, float height)
    {
        this->cameraPos = cameraPos;
        this->cameraDirection = cameraDirection / glm::length(cameraDirection);
        this->width = width;
        this->height = height;
        getPerpectiveMatrix();
    }
    void Translate(glm::vec3 displacment)
    {
        this->cameraPos += (displacment[2] * cameraDirection + displacment[0] * cam_right + displacment[1] * cam_up) * cameraSpeed;
    }
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraDirection, cam_up);
    }

    glm::mat4 getPerpectiveMatrix()
    {
        this->perspectiveMatrix = glm::perspective(glm::radians(this->FOV), (float)this->width / this->height, 0.1f, 400.0f);
        return this->perspectiveMatrix;
    }
    glm::mat4 getCameraMatrix()
    {
        return getPerpectiveMatrix() * getViewMatrix();
    }
    void updateCamVectors()
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDirection = glm::normalize(direction);

        cam_right = glm::normalize(glm::cross(cameraDirection, glm::vec3(0, 1, 0)));
        cam_up = glm::normalize(glm::cross(cam_right, cameraDirection));
    }
    void mouseMove(int new_x, int new_y)
    {
        float xoff = new_x - prevX;
        float yoff = new_y - prevY;

        yaw += xoff * mouse_Sensivity;
        pitch -= yoff * mouse_Sensivity;

        prevX = new_x;
        prevY = new_y;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
        updateCamVectors();
    }
    void setFov(float fov)
    {
        this->FOV = fov;
    }

    void DrawDebugLine(){

    }

};
