#pragma once

#include "utils.h"
#include <unordered_map>

// vertex of an animated model
struct SkeletonVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;		// texCoord
	glm::vec4 boneIds = glm::vec4(0);
	glm::vec4 boneWeights = glm::vec4(0.0f);
};

// structure to hold bone tree (skeleton)
struct Bone {
	int id = 0; // position of the bone in final upload array
	std::string name = "";
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};
};

// sturction representing an animation track
struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

// structure containing animation information
struct Animation {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

