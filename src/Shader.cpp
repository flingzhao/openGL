#include "Shader.h"
#include <array>

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    else if (type == "fragment")
        return GL_FRAGMENT_SHADER;
    else if(type == "geometry")
        return GL_GEOMETRY_SHADER;
    std::cout << "unknown shader type" << std::endl;
    return 0;
}

static std::string ReadFile(const std::string& filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        std::cout << "Could not open file!\n";
    }
	return result;
}

static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);

        size_t begin = pos + typeTokenLength + 1; //begin为vertex 或 fragment
        std::string type = source.substr(begin, eol - begin);

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);

        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - 
                                        (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

    }
    return shaderSources;
}

Shader::Shader(const std::string& filepath)
{
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);

    unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

    int success;
    char infoLog[512];
    
    for(auto& shadersrc : shaderSources)
    {
        unsigned int shader = glCreateShader(shadersrc.first);
        const std::string src = shadersrc.second;
        // std::cout << src << std::endl;
        const GLchar* sourceCStr = src.c_str();
        glShaderSource(shader, 1, &sourceCStr, nullptr);
        glCompileShader(shader);

	    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);   

        if(!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::" << shadersrc.first << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        glAttachShader(shaderProgram, shader);
        // std::cout << shaderProgram << '\n';
        glDeleteShader(shader);
    }

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    m_RenderID = shaderProgram;

}

void Shader::SetMat4(const std::string& parametername, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_RenderID, parametername.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetMat4(const std::string& parametername, const std::array<std::array<float, 4>, 4>& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_RenderID, parametername.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetInt(std::string_view parametername, const int value)
{
    glUniform1i(glGetUniformLocation(m_RenderID, parametername.data()), value);
}