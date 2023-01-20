#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target,glm::vec3 up)
{
    m_Position = position;
    m_WorldUp = up;
    
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    m_Position = position;
    m_WorldUp = up;
    m_Yaw = yaw;
    m_Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    if(m_Pitch > 89.0f) m_Pitch = 89.0f;
    if(m_Pitch < -89.0f) m_Pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}