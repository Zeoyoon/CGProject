#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

#include "../base/vertex.h"
#include "../base/object3d.h"
#include "../base/shader.h"
#include "../base/light.h"
#include "../base/texture.h"
#include "../base/material.h"

const std::string textFile1 = "../model/house-with-pool-1.snapshot.1/planet_Quom1200.png";

enum class GeoMoveDirection {
	Forward,
	Backward,
	Right,
	Left
};
enum class GeoRotateDirection {
	CW,
	CCW
};

class Geometry : public Object3D 
{
public:
	Geometry(int type);   // default constructor
	~Geometry();

	GLuint getVertexArrayObject() const;

	size_t getVertexCount() const;

	size_t getFaceCount() const;
	
	void draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);  
	void draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);

	void move(GeoMoveDirection direction, float _deltaTime);
	void rotate(GeoRotateDirection direction, float _deltaTime);
	
	// move/rotate speed
	float _moveSpeed = 5.0f;
	float _rotateSpeed = 0.0f;

private:
	// vertices of the table represented in model's own coordinate
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	std::unique_ptr<Texture2D> _geotexture; // texture of geometry

	// geometry type
	int geoType = 0;
	
	// material
	Material _material;

	// texture file
	std::string _textfile;


	// opengl objects
	GLuint _vao = 0;
	GLuint _vbo = 0;
	GLuint _ebo = 0;

	void initGLResources();
	
	const char* _vsCode =
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

	// spot light
	const char* _fsCode =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"// π‚’’\n"
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
		//"uniform sampler2D texture1;\n"
		"void main()\n"
		"{    \n"
		"	// ambient\n"
		"    vec3 ambient = light.color * Ambient.rgb;\n"

		"    // diffuse \n"
		"    vec3 norm = normalize(Normal);\n"
		"    vec3 lightDir = normalize(light.position - FragPos);\n"
		"    float diff = max(dot(norm, lightDir), 0.0001);\n"
		"    vec3 diffuse = light.color * light.intensity * diff *Diffuse.rgb;  \n"

		"    // attenuation\n"
		"    float distance    = length(light.position - FragPos);\n"
		"    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"

		"	// specular\n"
		"    vec3 viewDir = normalize(viewPos - FragPos);\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);  \n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0001), shininess);\n"
		"    vec3 specular = light.color * light.intensity * spec *  Specular.rgb;  \n"

		"    diffuse   *= attenuation; \n"
		"    specular *= attenuation; \n"

		"    vec3 result = (ambient + diffuse + specular);\n"//"    vec3 result = (ambient + diffuse + specular)*texture(texture1,TexCoords).rgb;\n"
		"    FragColor = vec4(result,1.0f);\n"
		"}\n";

	// spot light
	const char* _fsCode2 =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"//π‚’’\n"
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
		//"uniform sampler2D texture1;\n"
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
		"    vec3 result = (ambient+diffuse+specular);\n"//"    vec3 result = (ambient+diffuse+specular)*texture(texture1,TexCoords).rgb;\n"
		"    FragColor = vec4(result,1.0f);\n"
		"}\n";

	std::unique_ptr<Shader> _shader;
	std::unique_ptr<Shader> _shaderS;
	std::unique_ptr<Shader> _shaderP;
};