#include "my_obj_loader.h"

MyObjLoader::MyObjLoader(std::string filepath, std::string textureFilepath) : _filepath(filepath), _textureFilepath(textureFilepath)
{
    // 先给一个默认的material材质，这个材质之后应该也不会发生啥变化了
    Material mat = { 32.0f,glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.01f,0.01f,0.01f,1.0f),1.0f,1.0f,2 };
    _material = mat;
    loadModel();
}

void MyObjLoader::loadModel()
{
    // 顶点属性
    std::vector<glm::vec3> vectexPosition;
    std::vector<glm::vec2> vertexTexcoord;
    std::vector<glm::vec3> vectexNormal;

    // 面片索引信息
    std::vector<glm::ivec3> positionIndex;
    std::vector<glm::ivec3> texcoordIndex;
    std::vector<glm::ivec3> normalIndex;

    // 打开文件流
    std::ifstream fin(_filepath);
    std::string line;
    if (!fin.is_open())
    {
        std::cout << "falied to open file: " << _filepath << std::endl;
        exit(-1);
    }

    // 按行读取
    while (std::getline(fin, line))
    {
        //std::cout << "??" << std::endl;
        std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
        std::string type;
        GLfloat x, y, z;
        int v0, vt0, vn0;   // 面片第 1 个顶点的【位置，纹理坐标，法线】索引
        int v1, vt1, vn1;   // 2
        int v2, vt2, vn2;   // 3
        char slash;

        // 读取obj文件
        sin >> type;

        if (type == "v") {
            sin >> x >> y >> z;
            //std::cout <<"v "<< x << y << z << std::endl;
            vectexPosition.push_back(glm::vec3(x, y, z));

        }
        if (type == "vt") {
            sin >> x >> y;
            vertexTexcoord.push_back(glm::vec2(x, y));
            //std::cout << "vt " << x << y << std::endl;
        }
        if (type == "vn") {
            sin >> x >> y >> z;
            vectexNormal.push_back(glm::vec3(x, y, z));
            //std::cout << "vn " << x << y << z << std::endl;
        }
        if (type == "f") {
            sin >> v0 >> slash >> vt0 >> slash >> vn0;
            //std::cout << "f1 " << v0 <<" "  << vt0 <<" " << v0 << std::endl;
            sin >> v1 >> slash >> vt1 >> slash >> vn1;
            //std::cout << "f2 " << v1 << " " << vt1 << " " << vn1 << std::endl;
            sin >> v2 >> slash >> vt2 >> slash >> vn2;
            //std::cout << "f3 " << v2 << " " << vt2 << " " << vn2 << std::endl;
            positionIndex.push_back(glm::ivec3(v0 - 1, v1 - 1, v2 - 1));
            texcoordIndex.push_back(glm::ivec3(vt0 - 1, vt1 - 1, vt2 - 1));
            normalIndex.push_back(glm::ivec3(vn0 - 1, vn1 - 1, vn2 - 1));
        }
    }

    //根据面片信息生成最终传入顶点着色器的顶点数据

    for (int i = 0; i < positionIndex.size(); i++)
    {
        // 顶点位置

        _points.push_back(vectexPosition[positionIndex[i].x]);
        _points.push_back(vectexPosition[positionIndex[i].y]);
        _points.push_back(vectexPosition[positionIndex[i].z]);
        // 顶点纹理坐
        _texcoords.push_back(vertexTexcoord[texcoordIndex[i].x]);
        _texcoords.push_back(vertexTexcoord[texcoordIndex[i].y]);
        _texcoords.push_back(vertexTexcoord[texcoordIndex[i].z]);
        // 顶点法线
        _normals.push_back(vectexNormal[normalIndex[i].x]);
        _normals.push_back(vectexNormal[normalIndex[i].y]);
        _normals.push_back(vectexNormal[normalIndex[i].z]);
    }

    // 生成vao对象并且绑定vao
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // 先确定vbo的总数据大小 -- 传NULL指针表示我们暂时不传数据
    GLuint dataSize = GLuint(sizeof(glm::vec3) * _points.size() + sizeof(glm::vec2) * _texcoords.size() + sizeof(glm::vec3) * _normals.size());
    glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_STATIC_DRAW);

    // 传送数据到vbo 分别传递 顶点位置 和 顶点纹理坐标
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * _points.size(), &_points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _points.size() + sizeof(glm::vec3) * _normals.size(), sizeof(glm::vec2) * _texcoords.size(), &_texcoords[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _points.size(), sizeof(glm::vec3) * _normals.size(), &_normals[0]);
    //// 生成着色器程序对象
    _shaderS.reset(new Shader(vertCode, fragCode));
    _shaderP.reset(new Shader(vertCode, fragCode1));
    // 建立顶点变量vPosition在着色器中的索引 同时指定vPosition变量的数据解析格式
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)(sizeof(glm::vec3) * _points.size()));
    // 纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(sizeof(glm::vec3) * _points.size() + sizeof(glm::vec3) * _normals.size()));
    // 生成纹理

    _texture.reset(new Texture2D(_textureFilepath));

}

void MyObjLoader::draw(glm::mat4 projection, glm::mat4 view)
{
    glBindVertexArray(_vao);
    _shader->setMat4("projection", projection);
    _shader->setMat4("view", view);
    _shader->setInt("Texture", 0);
    glActiveTexture(GL_TEXTURE0);
    _texture->bind();
    //std::cout << _points.size() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(_points.size()));
    glBindVertexArray(0);
}

void MyObjLoader::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, PointLight& light, glm::vec3 viewPos)
{
    glBindVertexArray(_vao);
    _shaderP->use();
    _shaderP->setMat4("projection", projection);
    _shaderP->setMat4("view", view);
    _shaderP->setMat4("model", model);
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
    _shaderP->setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    _texture->bind();
    //std::cout << _points.size() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(_points.size()));
    glBindVertexArray(0);
}


void MyObjLoader::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, SpotLight& light, glm::vec3 viewPos)
{
    glBindVertexArray(_vao);
    _shaderS->use();
    _shaderS->setMat4("projection", projection);
    _shaderS->setMat4("view", view);
    _shaderS->setMat4("model", model);
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
    _shaderS->setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    _texture->bind();
    //std::cout << _points.size() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(_points.size()));
    glBindVertexArray(0);
}
