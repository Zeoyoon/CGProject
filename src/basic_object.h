#pragma once
#include "../base/object3d.h"
#include <vector>
#include "../base/vertex.h"
#include "../base/shader.h"
#include "../base/material.h"
#include "../base/texture.h"
#include "../base/light.h"
const GLfloat PI = 3.14159265358979323846f;
enum class MoveDirection {
    Forward,
    Backward,
    Right,
    Left
};
enum class RotateDirection {
    CW,
    CCW
};
class BasicObject : public Object3D {


public:
    std::vector<float> _vertices;
    std::vector<unsigned int> _indices;
    Material _material;
    std::unique_ptr<Shader> _shaderSS;   // spot light + simple texture
    std::unique_ptr<Shader> _shaderPS;   // point light + simple texture
	std::unique_ptr<Shader> _shaderSB;   // spot light + blend texture
	std::unique_ptr<Shader> _shaderPB;   // point light + blend texture
	std::unique_ptr<Shader> _shaderSC;   // spot light + check texture
	std::unique_ptr<Shader> _shaderPC;   // point light + check texture

    unsigned int _vbo, _ebo;
    float _moveSpeed = 5.0f;
    float _rotateSpeed = 0.0f;  // 0.2f

public:
    BasicObject() = default;
    ~BasicObject() = default;

    void move(MoveDirection direction, float _deltaTime);
    void rotate(RotateDirection direction, float _deltatime);
    void rotateAngle(RotateDirection direction, float angle);

    unsigned int _vao;

	// 着色器

};

class Cube :public BasicObject {
private:
    std::string _filename;
	std::string _filename2;

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

	// 点光源 + 简单纹理
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

	// spot light + 简单纹理
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

	// 点光源 + 混合纹理
	const char* fragCode2 =
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

		"uniform float blend;\n"   // blend 系数
		"uniform sampler2D mapKd1;\n"
		"uniform sampler2D mapKd2;\n"

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
		"    vec3 result = (ambient  + diffuse + specular)  ;\n"
		"    FragColor = vec4(result,1.0f) * mix(texture(mapKd1, TexCoords), texture(mapKd2, TexCoords), blend) ;\n"
		"}\n";


	// spot light + 混合纹理
	const char* fragCode3 =
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

		"uniform float blend;\n"   // blend 系数
		"uniform sampler2D mapKd1;\n"
		"uniform sampler2D mapKd2;\n"

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

		"	 \n"
		"    diffuse   *= attenuation; \n"
		"    specular *= attenuation; \n"
		"	 \n"
		"    vec3 result = (ambient+diffuse+specular);\n"
		"    FragColor = vec4(result,1.0f) * mix(texture(mapKd1, TexCoords), texture(mapKd2, TexCoords), blend);\n"
		"}\n";

public:
    Cube(Material mat, std::string _filename);
    std::unique_ptr<Texture2D> _texture;
	std::unique_ptr<Texture2D> _texture2;

	bool ifBlend = false;
	float blend_coef = 0.0f;  // blend 系数

	void draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 model, glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
    void draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 model, glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);
    void scaleLength();
    void scaleWidth();
    void scaleHeight();
    void exportObj(std::string filepath);
};

class Sphere : public BasicObject {
private:
    const int Y_SEGMENTS = 50;
    const int X_SEGMENTS = 50;

public:
    Sphere(Material mat);

	std::unique_ptr<Texture2D> _texture;
	std::unique_ptr<Texture2D> _texture2;

    void draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 model, glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
    void draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);
    //void draw(glm::mat4 model, glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);
    void scaleBigger();
    void scaleSmaller();
    void exportObj(std::string filepath);

	int rendermode = 0;
	float blend_coef = 0.0f;  // blend 系数

    int _repeat = 10;
    glm::vec3 _color1 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 _color2 = glm::vec3(0.0f, 0.0f, 0.0f);

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

    const char* check_fragCode =
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
        "uniform int repeat;\n"
        "uniform vec3 color1;\n"
        "uniform vec3 color2;\n"
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
        "    vec3 result = (ambient  + diffuse + specular);\n"
        "    vec3 color;\n"
        "	if((int(TexCoords.x*repeat) + int(TexCoords.y*repeat))%2==0)\n"
        "		color = color1;\n"
        "	else\n"
        "		color = color2;\n"
        "    FragColor = vec4(result*color,1.0f);\n"
        "}\n";

    // spot light + check texture
    const char* check_fragCode1 =
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
        "uniform int repeat;\n"
        "uniform vec3 color1;\n"
        "uniform vec3 color2;\n"
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
        "    vec3 diffuse = light.color * intensity * diff *Diffuse.rgb;  \n"
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
        "    vec3 result = (ambient  + diffuse + specular);\n"
        "   vec3 color;\n"
        "	if((int(TexCoords.x*repeat) + int(TexCoords.y*repeat))%2==0)\n"
        "		color = color1;\n"
        "	else\n"
        "		color = color2;\n"
        "    FragColor = vec4(result*color,1.0f);\n"
        "}\n";
};