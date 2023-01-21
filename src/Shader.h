#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
	
#include<cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>


class Shader
{
public:
    Shader(const std::string& filepath);

    inline void Use() { glUseProgram(m_RenderID); };
    inline unsigned int GetRenderID() { return m_RenderID;}

    inline void SetUniform1I(const std::string& parametername, const int id) { glUniform1i(glGetUniformLocation(m_RenderID, parametername.c_str()), id); }
    inline void SetUniform4f(const std::string& parametername, const glm::vec3& color) { glUniform4f(glGetUniformLocation(m_RenderID, parametername.c_str()), color.r, color.g, color.b, 1.0f); };
    inline void SetUniform3f(const std::string& parametername, const glm::vec3& color) { glUniform3f(glGetUniformLocation(m_RenderID, parametername.c_str()), color.r, color.g, color.b); };
    inline void SetFloat(const std::string& parametername, const float value) { glUniform1f(glGetUniformLocation(m_RenderID, parametername.c_str()), value); };
    inline void SetVec3(const std::string& parametername, float x, float y, float z) { glUniform3f(glGetUniformLocation(m_RenderID, parametername.c_str()), x, y, z); };
    void SetMat4(const std::string& parametername, const glm::mat4& mat);

private:
    unsigned int m_RenderID;

};


#endif /*__SHADER_H__*/