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

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 perspectiveMatrix = glm::mat4(1.0f);

    glm::vec3 cam_up = glm::vec3(0,1,0);
    glm::vec3 cam_right = glm::vec3(0,0,-1);

    float FOV = 90;

    float cameraSpeed = 2;

    float prevX=0;
    float prevY =0;
public:
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
    void Translate(glm::vec3 displacment){
        this->cameraPos += (displacment[2]*cameraDirection + displacment[0]*cam_right + displacment[1]*cam_up)*cameraSpeed;
    }
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraDirection, cam_up);
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
    
    void mouseMove(){

    }
    void setFov(float fov)
    {
        this->FOV = fov;
    }

    
};
