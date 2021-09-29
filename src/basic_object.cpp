#include "basic_object.h"
#include "config/option.h"
#include <iostream>

extern struct Option globalOption;

void BasicObject::move(MoveDirection direction, float _deltaTime)
{
    switch (direction)
    {
    case MoveDirection::Forward:
        position += _moveSpeed * _deltaTime * getFront();
        break;
    case MoveDirection::Backward:
        position -= _moveSpeed * _deltaTime * getFront();
        break;
    case MoveDirection::Right:
        position += _moveSpeed * _deltaTime * getRight();
        break;
    case MoveDirection::Left:
        position -= _moveSpeed * _deltaTime * getRight();
        break;
    default:
        break;
    }
}

void BasicObject::rotate(RotateDirection direction, float _deltaTime)
{
    const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
    float angle = -_rotateSpeed * _deltaTime;
    switch (direction)
    {
    case RotateDirection::CW:
        rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
        break;
    case RotateDirection::CCW:
        angle = -angle;
        rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
        break;
    default:
        break;
    }
}

void BasicObject::rotateAngle(RotateDirection direction, float angle)
{
    const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
    switch (direction)
    {
    case RotateDirection::CW:
        rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
        break;
    case RotateDirection::CCW:
        angle = -angle;
        rotation = glm::quat(cos(angle * 0.5), axis.x * sin(angle * 0.5), axis.y * sin(angle * 0.5), axis.z * sin(angle * 0.5)) * rotation;
        break;
    default:
        break;
    }
}


Cube::Cube(Material mat, std::string filename)
{
    _material = mat;
	_filename = filename;
	_filename2 = "../model/house/texture2.jpg";
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    for (int i = 0; i < 288; i++)
    {
        _vertices.push_back(vertices[i]);
    }
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(_vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // init pointLight shader

    // 根据光照选择着色器
    _shaderPS.reset(new Shader(vertCode, fragCode));
    _shaderSS.reset(new Shader(vertCode, fragCode1));
	_shaderPB.reset(new Shader(vertCode, fragCode2));
	_shaderSB.reset(new Shader(vertCode, fragCode3));
    _texture.reset(new Texture2D(_filename));
	_texture2.reset(new Texture2D(_filename2));
}

void Cube::draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos)
{
	if (ifBlend == false) {  
		_shaderPS->use();
		_shaderPS->setMat4("projection", projection);
		_shaderPS->setMat4("view", view);
		_shaderPS->setMat4("model", this->getModelMatrix());
		_shaderPS->setVec3("light.position", light.position);
		_shaderPS->setVec3("light.color", light.color);
		_shaderPS->setFloat("light.intensity", light.intensity);
		_shaderPS->setFloat("light.constant", light.kc);
		_shaderPS->setFloat("light.linear", light.kl);
		_shaderPS->setFloat("light.quadratic", light.kq);
		_shaderPS->setFloat("shininess", _material.Ns);
	
		_shaderPS->setVec3("viewPos", viewPos);
		_shaderPS->setVec4("Ambient", _material.Ka);
		_shaderPS->setVec4("Diffuse", _material.Kd);
		_shaderPS->setVec4("Specular", _material.Ks);
		_texture->bind();
	}
	else if (ifBlend == true) {
		_shaderPB->use();
		_shaderPB->setMat4("projection", projection);
		_shaderPB->setMat4("view", view);
		_shaderPB->setMat4("model", this->getModelMatrix());
		_shaderPB->setVec3("light.position", light.position);
		_shaderPB->setVec3("light.color", light.color);
		_shaderPB->setFloat("light.intensity", light.intensity);
		_shaderPB->setFloat("light.constant", light.kc);
		_shaderPB->setFloat("light.linear", light.kl);
		_shaderPB->setFloat("light.quadratic", light.kq);
		_shaderPB->setFloat("shininess", _material.Ns);

		_shaderPB->setVec3("viewPos", viewPos);
		_shaderPB->setVec4("Ambient", _material.Ka);
		_shaderPB->setVec4("Diffuse", _material.Kd);
		_shaderPB->setVec4("Specular", _material.Ks); 
		_shaderPB->setFloat("blend", blend_coef);   //// blend

		_shaderPB->setInt("mapKd1", 0);
		_shaderPB->setInt("mapKd2", 1);

		glActiveTexture(GL_TEXTURE0);
		_texture->bind();

		glActiveTexture(GL_TEXTURE1);
		_texture2->bind();
	}

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos)
{
	if (ifBlend == false) {
		_shaderSS->use();
		_shaderSS->setMat4("projection", projection);
		_shaderSS->setMat4("view", view);
		_shaderSS->setMat4("model", getModelMatrix());
		_shaderSS->setVec3("light.position", light.position);
		_shaderSS->setVec3("light.color", light.color);
		_shaderSS->setFloat("light.intensity", light.intensity);
		_shaderSS->setFloat("light.constant", light.kc);
		_shaderSS->setFloat("light.linear", light.kl);
		_shaderSS->setFloat("light.quadratic", light.kq);
		_shaderSS->setFloat("shininess", _material.Ns);
		_shaderSS->setVec3("viewPos", viewPos);
		_shaderSS->setVec4("Ambient", _material.Ka);
		_shaderSS->setVec4("Diffuse", _material.Kd);
		_shaderSS->setVec4("Specular", _material.Ks);
		_shaderSS->setVec3("light.direction", light.getFront());
		_shaderSS->setFloat("light.cutOff", glm::cos(light.cutOff));
		_shaderSS->setFloat("light.outerCutOff", glm::cos(light.outerCutOff));
		_texture->bind();
	}
	else if (ifBlend == true) {
		_shaderSB->use();
		_shaderSB->setMat4("projection", projection);
		_shaderSB->setMat4("view", view);
		_shaderSB->setMat4("model", getModelMatrix());
		_shaderSB->setVec3("light.position", light.position);
		_shaderSB->setVec3("light.color", light.color);
		_shaderSB->setFloat("light.intensity", light.intensity);
		_shaderSB->setFloat("light.constant", light.kc);
		_shaderSB->setFloat("light.linear", light.kl);
		_shaderSB->setFloat("light.quadratic", light.kq);
		_shaderSB->setFloat("shininess", _material.Ns);
		_shaderSB->setVec3("viewPos", viewPos);
		_shaderSB->setVec4("Ambient", _material.Ka);
		_shaderSB->setVec4("Diffuse", _material.Kd);
		_shaderSB->setVec4("Specular", _material.Ks);
		_shaderSB->setVec3("light.direction", light.getFront());
		_shaderSB->setFloat("light.cutOff", glm::cos(light.cutOff));
		_shaderSB->setFloat("light.outerCutOff", glm::cos(light.outerCutOff));

		_shaderSB->setFloat("blend", blend_coef);   //// blend
		_shaderSB->setInt("mapKd1", 0);
		_shaderSB->setInt("mapKd2", 1);

		glActiveTexture(GL_TEXTURE0);
		_texture->bind();

		glActiveTexture(GL_TEXTURE1);
		_texture2->bind();
	}
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


void Cube::scaleLength()
{
    scale *= glm::vec3(0.8f, 1.0f, 1.0f);
}
void Cube::scaleWidth()
{
    scale *= glm::vec3(1.0f, 0.8f, 1.0f);
}
void Cube::scaleHeight()
{
    scale *= glm::vec3(1.0f, 1.0f, 0.8f);
}

void Cube::exportObj(std::string filepath)
{
    std::ofstream file;
    file.open(filepath.c_str(), std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "cannot open the file" << std::endl;
    }
    file << "# export cube obj file" << std::endl;
    for (int i = 0; i < 36; i++)
    {
        glm::vec4 pos{ _vertices.at(i * 8),_vertices.at(i * 8 + 1),_vertices.at(i * 8 + 2),1.0f };
        pos = getModelMatrix() * pos;
        file << "v " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    }
    for (int i = 0; i < 36; i++)
    {
        glm::vec3 nor{ _vertices.at(i * 8 + 3),_vertices.at(i * 8 + 4),_vertices.at(i * 8 + 5) };
        nor = glm::vec3(glm::mat3(glm::transpose(glm::inverse(getModelMatrix()))) * nor);
        file << "vn " << nor.x << " " << nor.y << " " << nor.z << std::endl;
    }
    for (int i = 0; i < 36; i++)
    {
        file << "vt " << _vertices.at(i * 6) << " " << _vertices.at(i * 7 + 1) << " " << 0 << std::endl;
    }
    for (int i = 0; i < 12; i++)
    {
        file << "f " << i * 3 + 1 << "/" << i * 3 + 1 << "/" << i * 3 + 1 << " " << i * 3 + 2 << "/" << i * 3 + 2 << "/" << i * 3 + 2 << " " << i * 3 + 3 << "/" << i * 3 + 3 << "/" << i * 3 + 3 << std::endl;
    }
    file.close();
}

Sphere::Sphere(Material mat)
{
    _material = mat;
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

	//生成球的顶点
    for (int y = 0; y <= Y_SEGMENTS; y++)
    {
        for (int x = 0; x <= X_SEGMENTS; x++)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            //aPos
            sphereVertices.push_back(xPos);
            sphereVertices.push_back(yPos);
            sphereVertices.push_back(zPos);
            //aMormal
            sphereVertices.push_back(xPos);
            sphereVertices.push_back(yPos);
            sphereVertices.push_back(zPos);
            //aTexCoords
            sphereVertices.push_back(xSegment);
            sphereVertices.push_back(ySegment);
        }
    }
    for (int i = 0; i < sphereVertices.size(); i++)
    {
        _vertices.push_back(sphereVertices[i]);
    }
    //生成球的Indices
    for (int i = 0; i < Y_SEGMENTS; i++)
    {
        for (int j = 0; j < X_SEGMENTS; j++)
        {
            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
        }
    }
    for (int i = 0; i < sphereIndices.size(); i++)
    {
        _indices.push_back(sphereIndices[i]);
    }

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    //解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // 根据光照选择着色器

	_shaderPC.reset(new Shader(vertCode, check_fragCode));
	_shaderSC.reset(new Shader(vertCode, check_fragCode1));
}

// point light
void Sphere::draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos)
{
	glm::vec3 temp_color;
	if (rendermode == 0) {
		temp_color = _color1;
	}
	else if (rendermode == 2) {
		temp_color = _color2;
	}
	_shaderPC->use();
	_shaderPC->setMat4("projection", projection);
	_shaderPC->setMat4("view", view);
	_shaderPC->setMat4("model", this->getModelMatrix());
	_shaderPC->setVec3("light.position", light.position);
	_shaderPC->setVec3("light.color", light.color);
	_shaderPC->setFloat("light.intensity", light.intensity);
	_shaderPC->setFloat("light.constant", light.kc);
	_shaderPC->setFloat("light.linear", light.kl);
	_shaderPC->setFloat("light.quadratic", light.kq);
	_shaderPC->setFloat("light.shininess", _material.Ns);
	_shaderPC->setVec3("viewPos", viewPos);
	_shaderPC->setVec4("Ambient", _material.Ka);
	_shaderPC->setVec4("Diffuse", _material.Kd);
	_shaderPC->setVec4("Specular", _material.Ks);
	_shaderPC->setInt("repeat", _repeat);
	_shaderPC->setVec3("color1", _color1);
	_shaderPC->setVec3("color2", temp_color);

	glBindVertexArray(_vao);
	//使用线框模式绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos)
{
	glm::vec3 temp_color;
	if (rendermode == 0) {
		temp_color = _color1;
	}
	else if (rendermode == 2) {
		temp_color = _color2;
	}

	_shaderSC->use();
    _shaderSC->setMat4("projection", projection);
    _shaderSC->setMat4("view", view);
    _shaderSC->setMat4("model", this->getModelMatrix());
    _shaderSC->setVec3("light.position", light.position);
    _shaderSC->setVec3("light.color", light.color);
    _shaderSC->setFloat("light.intensity", light.intensity);
    _shaderSC->setFloat("light.constant", light.kc);
    _shaderSC->setFloat("light.linear", light.kl);
    _shaderSC->setFloat("light.quadratic", light.kq);
    _shaderSC->setFloat("light.shininess", _material.Ns);
    _shaderSC->setVec3("viewPos", viewPos);
    _shaderSC->setVec4("Ambient", _material.Ka);
    _shaderSC->setVec4("Diffuse", _material.Kd);
    _shaderSC->setVec4("Specular", _material.Ks);
    _shaderSC->setVec3("light.direction", light.getFront());
    _shaderSC->setFloat("light.cutOff", glm::cos(light.cutOff));
    _shaderSC->setFloat("light.outerCutOff", glm::cos(light.outerCutOff));
    _shaderSC->setInt("repeat", _repeat);
    _shaderSC->setVec3("color1", _color1);
    _shaderSC->setVec3("color2", temp_color);
    glBindVertexArray(_vao);
    //使用线框模式绘制
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::scaleBigger()
{
    scale *= glm::vec3(1.2f, 1.2f, 1.2f);
}

void Sphere::scaleSmaller()
{
    scale *= glm::vec3(0.8f, 0.8f, 0.8f);
}


void Sphere::exportObj(std::string filepath)
{
    std::ofstream file;
    file.open(filepath.c_str(), std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "cannot open the file" << std::endl;
    }
    file << "# export cube obj file" << std::endl;
    for (int i = 0; i < _vertices.size() / 8; i++)
    {
        glm::vec4 pos{ _vertices.at(i * 8),_vertices.at(i * 8 + 1),_vertices.at(i * 8 + 2),1.0f };
        pos = getModelMatrix() * pos;
        file << "v " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    }
    for (int i = 0; i < _vertices.size() / 8; i++)
    {
        glm::vec3 nor{ _vertices.at(i * 8 + 3),_vertices.at(i * 8 + 4),_vertices.at(i * 8 + 5) };
        nor = glm::vec3(glm::mat3(glm::transpose(glm::inverse(getModelMatrix()))) * nor);
        file << "vn " << nor.x << " " << nor.y << " " << nor.z << std::endl;
    }
    for (int i = 0; i < _vertices.size() / 8; i++)
    {
        file << "vt " << _vertices.at(i * 6) << " " << _vertices.at(i * 7 + 1) << " " << 0 << std::endl;
    }
    std::cout << _indices.size() / 3 << std::endl;
    for (int i = 0; i < _indices.size() / 3; i++)
    {
        file << "f " << _indices.at(i * 3 + 0) + 1 << "/" << _indices.at(i * 3 + 0) + 1 << "/" << _indices.at(i * 3 + 0) + 1 << " " << _indices.at(i * 3 + 1) + 1 << "/" << _indices.at(i * 3 + 1) + 1 << "/" << _indices.at(i * 3 + 1) + 1 << " " << _indices.at(i * 3 + 2) + 1 << "/" << _indices.at(i * 3 + 2) + 1 << "/" << _indices.at(i * 3 + 2) + 1 << std::endl;
    }
    file.close();
}