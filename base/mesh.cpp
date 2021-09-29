#include "mesh.h"
#include <string>
#include <iostream>
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures,Material mat):
	_vertices(vertices),_indices(indices),_textures(textures),_mat(mat)
{
	setupMesh();
}
void Mesh::draw(Shader& shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i]._type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream
        // now set the sampler to the correct texture unit
        shader.setInt((name + number).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, _textures[i]._handle);
    }
    // draw mesh
    glBindVertexArray(_vao);
    //glBindBufferRange(GL_UNIFORM_BUFFER, 0, _uniformBlockIndex, 0, sizeof(Material));
    
    //glBindBuffer(GL_UNIFORM_BUFFER, _uniformBlockIndex);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), NULL, GL_STATIC_DRAW); 
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    //glGenBuffers(1, &_uniformBlockIndex);

    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);


    //glBindBuffer(GL_UNIFORM_BUFFER, _uniformBlockIndex);
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(_mat), (void*)(&_mat), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),&_indices[0], GL_STATIC_DRAW);
    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glBindVertexArray(0);
}

