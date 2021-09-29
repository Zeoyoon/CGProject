#pragma once

#include <glm/glm.hpp>

struct Material {
	// 高光度
	float Ns;
	//材质颜色光照
	glm::vec4 Ka;
	//漫反射
	glm::vec4 Kd;
	//镜反射
	glm::vec4 Ks;
	// 折射率
	float Ni;
	// 渐隐指数
	float d;
	// 光照类型
	unsigned int illum;
};