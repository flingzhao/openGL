#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	// //bone indexes which will influence this vertex
	// int m_BoneIDs[MAX_BONE_INFLUENCE];
	// //weights from each bone
	// float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture 
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Texture> m_Textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texture);
    void Draw(Shader& shader);
    unsigned int getVAO() { return VAO; }

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};
#endif /*  __MESH_H__ */

