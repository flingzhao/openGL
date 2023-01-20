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

    inline void SetUniform4f(const std::string& parametername, const glm::vec3& color) { glUniform4f(glGetUniformLocation(m_RenderID, parametername.c_str()), color.r, color.g, color.b, 1.0f); };
    inline void SetUniform1I(const std::string& parametername, const int id) { glUniform1i(glGetUniformLocation(m_RenderID, parametername.c_str()), id); }
    void SetMat4(const std::string& parametername, const glm::mat4& mat);

private:
    unsigned int m_RenderID;

};


#endif /*__SHADER_H__*/