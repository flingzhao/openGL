#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 position){}
    Camera(glm::vec3 position, glm::vec3 target){UpdateCameraVectors();}
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

public:
    glm::mat4 GetViewMatrix();

    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessKeyBoard();

    glm::vec3& GetCameraPosition() {return m_Position;}
    glm::vec3& GetCameraFront() {return m_Front;}
    glm::vec3& GetCameraUp() {return m_Up;}


private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;      //相机的上方
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp; // 世界坐标系的上方

    float m_Yaw, m_Pitch;

    float m_MouseSensitivity = 0.05f;

private:
    void UpdateCameraVectors();
    
};

#endif /*__CAMERA_H__*/