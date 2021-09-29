#include "geometry.h"

#include <iostream>
#include <unordered_map>
#include <glm/ext.hpp>

using namespace std;

Geometry::Geometry(int type){
	geoType = type;
	vector<glm::vec3> m_vertices;
	vector<glm::vec2> m_uvs;
	vector<glm::vec3> m_normals;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	std::unordered_map<Vertex, uint32_t> uniqueVertices;
	
	// 圆柱
	if (type == 2) {  
		for (int i = 0; i <= 360; i+=5) {
			float p = i * 3.1415926 / 180;
			float q = (i+5) * 3.1415926 / 180;

			Vertex vertex[12]{};
			vertex[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex[1].position = glm::vec3(sin(p), 1.0f, cos(p));
			vertex[2].position = glm::vec3(sin(q), 1.0f, cos(q));

			vertex[3].position = vertex[1].position;
			vertex[4].position = vertex[2].position;
			vertex[5].position = glm::vec3(sin(q), -1.0f, cos(q));

			vertex[6].position = vertex[5].position;
			vertex[7].position = glm::vec3(sin(p), -1.0f, cos(p));
			vertex[8].position = vertex[1].position;

			vertex[9].position = vertex[6].position;
			vertex[10].position = vertex[7].position;
			vertex[11].position = glm::vec3(0.0f, -1.0f, 0.0f);

			// 计算法线
			for (int j = 0; j < 4; j++) {
				float x1 = vertex[0 + j * 3].position.x;
				float x2 = vertex[1 + j * 3].position.x;
				float x3 = vertex[2 + j * 3].position.x;
				float y1 = vertex[0 + j * 3].position.y;
				float y2 = vertex[1 + j * 3].position.y;
				float y3 = vertex[2 + j * 3].position.y;
				float z1 = vertex[0 + j * 3].position.z;
				float z2 = vertex[1 + j * 3].position.z;
				float z3 = vertex[2 + j * 3].position.z;

				vertex[0 + j * 3].normal = vertex[1 + j * 3].normal = vertex[2 + j * 3].normal 
					= glm::vec3((y1-y3)*(z1-z2) - (y1-y2)*(z1-z3), 
								(x1-x2)*(z1-z3) - (z1-z2)*(x1-x3), 
								(x1-x3)*(y1-y2) - (x1-x2)*(y1-y3));
			}

			for (int j = 0; j < 12; j++) {
				if (uniqueVertices.count(vertex[j]) == 0) {
					uniqueVertices[vertex[j]] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex[j]);
				}
				indices.push_back(uniqueVertices[vertex[j]]);
			}
		}

		_vertices = vertices;
		_indices = indices;
	}

	// 圆锥
	else if (type == 3) {  
		for (int i = 0; i <= 360; i += 5) {
			float p = i * 3.1415926 / 180;
			float q = (i + 5) * 3.1415926 / 180;

			Vertex vertex[6]{};
			vertex[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex[1].position = glm::vec3(sin(p), -1.0f, cos(p));
			vertex[2].position = glm::vec3(sin(q), -1.0f, cos(q));

			vertex[3].position = vertex[1].position;
			vertex[4].position = vertex[2].position;
			vertex[5].position = glm::vec3(0.0f, -1.0f, 0.0f);


			for (int j = 0; j < 2; j++) {
				float x1 = vertex[0 + j * 3].position.x;
				float x2 = vertex[1 + j * 3].position.x;
				float x3 = vertex[2 + j * 3].position.x;
				float y1 = vertex[0 + j * 3].position.y;
				float y2 = vertex[1 + j * 3].position.y;
				float y3 = vertex[2 + j * 3].position.y;
				float z1 = vertex[0 + j * 3].position.z;
				float z2 = vertex[1 + j * 3].position.z;
				float z3 = vertex[2 + j * 3].position.z;

				vertex[0 + j * 3].normal = vertex[1 + j * 3].normal = vertex[2 + j * 3].normal
					= glm::vec3((y1 - y2)*(z1 - z3) - (y1 - y3)*(z1 - z2),
								(z1 - z2)*(x1 - x3) - (x1 - x2)*(z1 - z3),
								(x1 - x2)*(y1 - y3) - (x1 - x3)*(y1 - y2));
			}


			for (int j = 0; j < 6; j++) {
				if (uniqueVertices.count(vertex[j]) == 0) {
					uniqueVertices[vertex[j]] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex[j]);
				}
				indices.push_back(uniqueVertices[vertex[j]]);
			}
		}

		_vertices = vertices;
		_indices = indices;
	}

	// 多面棱柱
	else if (type == 4) {  
		for (int i = 0; i <= 360; i += 72) {
			float p = i * 3.1415926 / 180;
			float q = (i + 72) * 3.1415926 / 180;

			Vertex vertex[12]{};
			vertex[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex[1].position = glm::vec3(sin(p), 1.0f, cos(p));
			vertex[2].position = glm::vec3(sin(q), 1.0f, cos(q));

			vertex[3].position = vertex[1].position;
			vertex[4].position = vertex[2].position;
			vertex[5].position = glm::vec3(sin(q), -1.0f, cos(q));

			vertex[6].position = vertex[5].position;
			vertex[7].position = glm::vec3(sin(p), -1.0f, cos(p));
			vertex[8].position = vertex[1].position;

			vertex[9].position = vertex[6].position;
			vertex[10].position = vertex[7].position;
			vertex[11].position = glm::vec3(0.0f, -1.0f, 0.0f);

			for (int j = 0; j < 4; j++) {
				float x1 = vertex[0 + j * 3].position.x;
				float x2 = vertex[1 + j * 3].position.x;
				float x3 = vertex[2 + j * 3].position.x;
				float y1 = vertex[0 + j * 3].position.y;
				float y2 = vertex[1 + j * 3].position.y;
				float y3 = vertex[2 + j * 3].position.y;
				float z1 = vertex[0 + j * 3].position.z;
				float z2 = vertex[1 + j * 3].position.z;
				float z3 = vertex[2 + j * 3].position.z;

				vertex[0 + j * 3].normal = vertex[1 + j * 3].normal = vertex[2 + j * 3].normal
					= glm::vec3((y1 - y3)*(z1 - z2) - (y1 - y2)*(z1 - z3),
								(x1 - x2)*(z1 - z3) - (z1 - z2)*(x1 - x3),
								(x1 - x3)*(y1 - y2) - (x1 - x2)*(y1 - y3));
			}


			for (int j = 0; j < 12; j++) {
				if (uniqueVertices.count(vertex[j]) == 0) {
					uniqueVertices[vertex[j]] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex[j]);
				}
				indices.push_back(uniqueVertices[vertex[j]]);
			}
		}

		_vertices = vertices;
		_indices = indices;
	}

	// 多面棱台
	else if (type == 5) {  
		for (int i = 0; i <= 360; i += 72) {
			float p = i * 3.1415926 / 180;
			float q = (i + 72) * 3.1415926 / 180;

			Vertex vertex[12]{};
			vertex[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex[1].position = glm::vec3(sin(p)*0.75, 1.0f, cos(p)*0.75);
			vertex[2].position = glm::vec3(sin(q)*0.75, 1.0f, cos(q)*0.75);

			vertex[3].position = vertex[1].position;
			vertex[4].position = vertex[2].position;
			vertex[5].position = glm::vec3(sin(q)*1.5, -1.0f, cos(q)*1.5);

			vertex[6].position = vertex[5].position;
			vertex[7].position = glm::vec3(sin(p)*1.5, -1.0f, cos(p)*1.5);
			vertex[8].position = vertex[1].position;

			vertex[9].position = vertex[6].position;
			vertex[10].position = vertex[7].position;
			vertex[11].position = glm::vec3(0.0f, -1.0f, 0.0f);

			for (int j = 0; j < 4; j++) {
				float x1 = vertex[0 + j * 3].position.x;
				float x2 = vertex[1 + j * 3].position.x;
				float x3 = vertex[2 + j * 3].position.x;
				float y1 = vertex[0 + j * 3].position.y;
				float y2 = vertex[1 + j * 3].position.y;
				float y3 = vertex[2 + j * 3].position.y;
				float z1 = vertex[0 + j * 3].position.z;
				float z2 = vertex[1 + j * 3].position.z;
				float z3 = vertex[2 + j * 3].position.z;

				vertex[0 + j * 3].normal = vertex[1 + j * 3].normal = vertex[2 + j * 3].normal
					= glm::vec3((y1 - y3)*(z1 - z2) - (y1 - y2)*(z1 - z3),
								(x1 - x2)*(z1 - z3) - (z1 - z2)*(x1 - x3),
								(x1 - x3)*(y1 - y2) - (x1 - x2)*(y1 - y3));
			}


			for (int j = 0; j < 12; j++) {
				if (uniqueVertices.count(vertex[j]) == 0) {
					uniqueVertices[vertex[j]] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex[j]);
				}
				indices.push_back(uniqueVertices[vertex[j]]);
			}
		}

		_vertices = vertices;
		_indices = indices;
	}

	Material mat = { 32.0f,glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.01f,0.01f,0.01f,1.0f),1.0f,1.0f,2 };
	_material = mat;
	
	initGLResources();
}


void Geometry::draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos) {
	// --- determine the type of geometry and give the model ---

	_shaderP->use();
	_shaderP->setMat4("projection", projection);
	_shaderP->setMat4("view", view);
	_shaderP->setMat4("model", this->getModelMatrix());
	_shaderP->setVec3("light.position", light.position);
	_shaderP->setVec3("light.color", light.color);
	_shaderP->setFloat("light.intensity", light.intensity);
	_shaderP->setFloat("light.constant", light.kc);
	_shaderP->setFloat("light.linear", light.kl);
	_shaderP->setFloat("light.quadratic", light.kq);
	_shaderP->setFloat("light.shininess", _material.Ns);
	_shaderP->setVec3("viewPos", viewPos);
	_shaderP->setVec4("Ambient", _material.Ka);
	_shaderP->setVec4("Diffuse", _material.Kd);
	_shaderP->setVec4("Specular", _material.Ks);
	//_texture->bind();

	glBindVertexArray(_vao);    //_vao是private
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos){

	_shaderS->use();
	_shaderS->setMat4("projection", projection);
	_shaderS->setMat4("view", view);
	_shaderS->setMat4("model", this->getModelMatrix());
	_shaderS->setVec3("light.position", light.position);
	_shaderS->setVec3("light.color", light.color);
	_shaderS->setFloat("light.intensity", light.intensity);
	_shaderS->setFloat("light.constant", light.kc);
	_shaderS->setFloat("light.linear", light.kl);
	_shaderS->setFloat("light.quadratic", light.kq);
	_shaderS->setFloat("light.shininess", _material.Ns);
	_shaderS->setVec3("viewPos", viewPos);
	_shaderS->setVec4("Ambient", _material.Ka);
	_shaderS->setVec4("Diffuse", _material.Kd);
	_shaderS->setVec4("Specular", _material.Ks);
	_shaderS->setVec3("light.direction", light.getFront());
	_shaderS->setFloat("light.cutOff", glm::cos(light.cutOff));
	_shaderS->setFloat("light.outerCutOff", glm::cos(light.outerCutOff));
	//_texture->bind();

	glBindVertexArray(_vao);    //_vao是private
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Geometry::~Geometry(){
	if (_ebo != 0) {
		glDeleteBuffers(1, &_ebo);
		_ebo = 0;
	}

	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

GLuint Geometry::getVertexArrayObject() const {
	return _vao;
}

size_t Geometry::getVertexCount() const {
	return _vertices.size();
}

size_t Geometry::getFaceCount() const {
	return _indices.size() / 3;
}

void Geometry::initGLResources() {
	// create a vertex array object
	glGenVertexArrays(1, &_vao);
	// create a vertex buffer object
	glGenBuffers(1, &_vbo);
	// create a element array buffer
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

	// specify layout, size of a vertex, data type, normalize, sizeof vertex array, offset of the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// 根据光照选择着色器
	_shaderP.reset(new Shader(_vsCode, _fsCode));
	_shaderS.reset(new Shader(_vsCode, _fsCode2));
	//_geotexture.reset(new Texture2D("../model/house-with-pool-1.snapshot.1/planet_Quom1200.png"));
}

void Geometry::move(GeoMoveDirection direction, float _deltaTime) {
	switch (direction)
	{
	case GeoMoveDirection::Forward:
		position += _moveSpeed * _deltaTime * getFront();
		break;
	case GeoMoveDirection::Backward:
		position -= _moveSpeed * _deltaTime * getFront();
		break;
	case GeoMoveDirection::Right:
		position += _moveSpeed * _deltaTime * getRight();
		break;
	case GeoMoveDirection::Left:
		position -= _moveSpeed * _deltaTime * getRight();
		break;
	default:
		break;
	}
}

void Geometry::rotate(GeoRotateDirection direction, float _deltaTime)
{
	const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
	float angle = -_rotateSpeed * _deltaTime;
	switch (direction)
	{
	case GeoRotateDirection::CW:
		rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
		break;
	case GeoRotateDirection::CCW:
		angle = -angle;
		rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
		break;
	default:
		break;
	}
}