#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;
    glm::vec3 target_lookAt;
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 perspectiveMatrix = glm::mat4(1.0f);

    float FOV = 90;
    int width;
    int height;

public:
    float x, y, z;
    Camera(glm::vec3 cameraPos, glm::vec3 cameraDirection, float width, float height)
    {
        this->cameraPos = cameraPos;
        this->cameraDirection = cameraDirection / glm::length(cameraDirection);
        this->width = width;
        this->height = height;
        getPerpectiveMatrix();
    }
    void setCameraPos(glm::vec3 cameraPos)
    {
        this->cameraPos = cameraPos;
    }

    void setCameraDirection(glm::vec3 cameraDirection)
    {
        this->cameraDirection = cameraDirection;
    }
    glm::mat4 getViewMatrix()
    {
        // Just use lookAt
        return glm::lookAt(cameraPos, target_lookAt, glm::vec3(0, 1, 0));
    }

    void setRotation(glm::vec3 target)
    {
        this->target_lookAt = target;
    }

    glm::mat4 getPerpectiveMatrix()
    {
        this->perspectiveMatrix = glm::perspective(this->FOV, (float)this->width / this->height, 0.1f, 200.0f);
        return this->perspectiveMatrix;
    }
    glm::mat4 getCameraMatrix()
    {
        return getPerpectiveMatrix() * getViewMatrix();
    }
    void setFov(float fov)
    {
        this->FOV = fov;
    }
};
