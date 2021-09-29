#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad\glad.h>
#include "../base/shader.h"
#include "../base/texture.h"
#include "../base/light.h"
#include "../base/material.h"
class MyObjLoader {
public:
    MyObjLoader(std::string filepath, std::string textureFilepath);
    void loadModel();
    void draw(glm::mat4 projection, glm::mat4 view);
    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, PointLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, SpotLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);
    GLuint _vao;
    std::unique_ptr<Shader> _shader;
    std::unique_ptr<Shader> _shaderS;
    std::unique_ptr<Shader> _shaderP;
private:
    std::string _filepath;
    std::string _textureFilepath;
    std::vector<glm::vec3> _points;
    std::vector<glm::vec2> _texcoords;
    std::vector<glm::vec3> _normals;
    GLuint _vbo;
    std::unique_ptr<Texture2D> _texture;
    Material _material;
    const char* vertCode =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "layout(location = 2) in vec2 aTexCoords;\n"
        "out vec3 FragPos;\n"
        "out vec2 TexCoords;\n"
        "out vec3 Normal;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "   TexCoords =aTexCoords; \n"
        "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "}\n";
    // 点光源
    const char* fragCode =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "//光照\n"
        "struct Light {\n"
        "    vec3 position;  \n"
        "  \n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "	\n"
        "    float constant;\n"
        "    float linear;\n"
        "    float quadratic;\n"
        "};\n"

        "in vec3 FragPos;  \n"
        "in vec3 Normal;  \n"
        "in vec2 TexCoords;\n"
        "//Material\n"
        "uniform vec4 Ambient;\n"
        "uniform vec4 Diffuse;\n"
        "uniform vec4 Specular; \n"
        "uniform float shininess;\n"
        "uniform vec3 viewPos;\n"
        "uniform Light light;\n"
        "uniform sampler2D texture1;\n"
        "void main()\n"
        "{    \n"
        "	// ambient\n"
        "    vec3 ambient = light.color * Ambient.rgb;\n"
        "  	\n"
        "    // diffuse \n"
        "    vec3 norm = normalize(Normal);\n"
        "    vec3 lightDir = normalize(light.position - FragPos);\n"
        "    float diff = max(dot(norm, lightDir), 0.0001);\n"
        "    vec3 diffuse =light.color * light.intensity * diff *Diffuse.rgb;  \n"
        "      \n"
        "    // attenuation\n"
        "    float distance    = length(light.position - FragPos);\n"
        "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"
        "	// specular\n"
        "    vec3 viewDir = normalize(viewPos - FragPos);\n"
        "    vec3 reflectDir = reflect(-lightDir, norm);  \n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0001), shininess);\n"
        "    vec3 specular = light.color * light.intensity * spec *  Specular.rgb;  \n"
        "	\n"
        "    diffuse   *= attenuation; \n"
        "    specular *= attenuation; \n"
        "	  \n"
        "    vec3 result = (ambient  + diffuse + specular)*texture(texture1,TexCoords).rgb;\n"
        "    FragColor = vec4(result,1.0f);\n"
        "}\n";

    // spot light
    const char* fragCode1 =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "//光照\n"
        "struct Light {\n"
        "    vec3 position;  \n"
        "  \n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "	\n"
        "    float constant;\n"
        "    float linear;\n"
        "    float quadratic;\n"
        "   float cutOff;\n"
        "   vec3 direction;\n"
        "   float  outerCutOff;\n"
        "};\n"

        "in vec3 FragPos;  \n"
        "in vec3 Normal;  \n"
        "in vec2 TexCoords;\n"
        "//Material\n"
        "uniform vec4 Ambient;\n"
        "uniform vec4 Diffuse;\n"
        "uniform vec4 Specular; \n"
        "uniform float shininess;\n"
        "uniform vec3 viewPos;\n"
        "uniform Light light;\n"
        "uniform sampler2D texture1;\n"
        "void main()\n"
        "{    \n"
        "	// ambient\n"
        "    vec3 ambient = light.color * Ambient.rgb;\n"
        "  	\n"

        "    // diffuse \n"
        "    vec3 norm = normalize(Normal);\n"
        "    vec3 lightDir = normalize(light.position - FragPos);\n"
        "    float theta     = dot(lightDir, normalize(-light.direction));\n"
        "    float epsilon   = light.cutOff - light.outerCutOff;\n"
        "    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0001, light.intensity); \n"
        "    float diff = max(dot(norm, lightDir), 0.0001);\n"
        "    vec3 diffuse =light.color * intensity * diff *Diffuse.rgb;  \n"
        "      \n"
        "    // attenuation\n"
        "    float distance    = length(light.position - FragPos);\n"
        "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"
        "	// specular\n"
        "    vec3 viewDir = normalize(viewPos - FragPos);\n"
        "    vec3 reflectDir = reflect(-lightDir, norm);  \n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0001), shininess);\n"
        "    vec3 specular = light.color * intensity * spec *  Specular.rgb;  \n"

        "	\n"
        "    diffuse   *= attenuation; \n"
        "    specular *= attenuation; \n"
        "	  \n"
        "    vec3 result = (ambient+diffuse+specular)*texture(texture1,TexCoords).rgb;\n"
        "    FragColor = vec4(result,1.0f);\n"
        "}\n";
};
