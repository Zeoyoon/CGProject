#pragma once
#include "object3d.h"
#include <glm/glm.hpp>

class Light : public Object3D {
public:
	Light() = default;

	~Light() = default;

public:
	float intensity = 1.0f;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

class DirectionalLight : public Light {
public:
	DirectionalLight() = default;

	~DirectionalLight() = default;
};

class PointLight : public Light {
public:
	PointLight(float _kc = 1.0f, float _kl = 0.01f, float _kq = 0.01f);
	~PointLight() = default;
public:
	// Ë¥¼õ
	float kc = 1.0f;
	float kl = 0.01f;
	float kq = 0.01f;
};

class SpotLight : public Light {
public:
	SpotLight(float _cutOff = glm::radians(60.0f), float _outerCutOff = glm::radians(100.0f), float _kc = 1.0f, float _kl = 0.01f, float _kq = 0.01f);

	~SpotLight() = default;

public:
	float cutOff = glm::radians(60.0f);
	float outerCutOff = glm::radians(100.0f);
	// Ë¥¼õ
	float kc = 1.0f;
	float kl = 0.01f;
	float kq = 0.01f;
};