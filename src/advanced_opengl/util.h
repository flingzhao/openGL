#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <iostream>
#include <vector>
#include <string>

#include "Camera.h"

inline constexpr unsigned int SCR_WIDTH = 800;
inline constexpr unsigned int SCR_HEIGHT = 600;

inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

inline float lastX = (float)SCR_WIDTH / 2.0;
inline float lastY = (float)SCR_HEIGHT / 2.0;
inline bool firstMouse = true;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(std::string_view path);
unsigned int loadCubeMap(const std::vector<std::string>& faces);
#endif /* __UTILITY_H__ */