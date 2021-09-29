#include "shader.h"
#include "mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
public:
    /*  函数   */
    Model(std::string const& path, bool gamma = false);
    void draw(Shader& shader);
private:
    /*  模型数据  */
    std::vector<Texture2D> textures_loaded;
    std::vector<Mesh> _meshes;
    std::string _directory;
    bool gammaCorrection; //gamma校正
    /*  函数   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
