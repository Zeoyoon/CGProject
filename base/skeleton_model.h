#pragma once

#include "skeleton.h"
#include "utils.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <stb_image.h>


class SkeletonModel
{
public:
    SkeletonModel(std::string const& modelPath, std::string const& diffusePath);
    void draw(unsigned int& shader);
    
    void getPose(Animation& animation, Bone& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);

    unsigned int _vao;

    unsigned int _vbo, _ebo;
    aiMesh* _meshes;
    std::vector<SkeletonVertex> _vertices = {};
    std::vector<uint> _indices = {};
    Bone _skeleton;
    uint _boneCount = 0;
    uint _diffuseTexture;
    Animation _animation;
    std::vector<glm::mat4> _currentPose = {};
    glm::mat4 _globalInverseTransform;
    glm::mat4 _identity;

private:
   


    void loadModel(std::string modelPath, std::string diffusePath);
    void processModel(const aiScene* scene, aiMesh* mesh, std::vector<SkeletonVertex>& verticesOutput, std::vector<uint>& indicesOutput, Bone& skeletonOutput, uint& nBoneCount);

    // a recursive function to read all bones and form skeleton
    bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);
    void loadAnimation(const aiScene* scene, Animation& animation);
    void createVertexArray(std::vector<SkeletonVertex>& vertices, std::vector<uint> indices);
    void createTexture(std::string filepath);
};
