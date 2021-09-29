#pragma once
#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include <vector>

class Mesh {
public:
	//mesh data
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture2D> _textures;
	Material _mat;
	unsigned int _vao;
	//unsigned int _uniformBlockIndex;
	//constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures,Material mat);

	// render the mesh
	void draw(Shader& shader);

private:
	unsigned int _vbo, _ebo;
	void setupMesh();
};






