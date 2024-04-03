#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model 
{
    public:
        /*  函数   */
        Model(const std::string& path)
        {
            loadModel(path);
        }
        std::vector<Mesh>& getMesh() { return m_Meshes; }
        const std::vector<Texture>& getTextureLoaded() { return m_TextureLoaded; }
        void Draw(Shader shader);   
    private:
        /*  模型数据  */
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        std::vector<Texture> m_TextureLoaded;
        /*  函数   */
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
};
#endif /*__MODEL_H__*/
