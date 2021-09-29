#pragma once
#include<iostream>

class UserSettings {

public:
	// 当前状态信息
	struct Status {
		bool basicVoxel; // 导入基本体素
	}_status;

	// 导入基本体素
	struct ImportBasicVoxel {
		bool Cube;    // 立方体
		bool Ball;    // 球
		bool Cylinder;// 圆柱
		bool Cone;    // 圆锥
		bool Prism;   // 多面棱柱
		bool Pyramid; // 多面棱台
	}_importBasicVoxel;

	struct ExportMedia {
		bool saveImage;
	}_exportMedia;


	bool showSettings;
	bool showStatistics;
	int displayMode;
	uint32_t samples;
	float exampleFloat;

	UserSettings() {
		_importBasicVoxel = { false };
		_exportMedia = { false };
		showSettings = false;
		showStatistics = false;
		samples = 120;
		exampleFloat = 1.0f;
	}

	UserSettings(std::shared_ptr<UserSettings> settings) {
		_importBasicVoxel = settings->_importBasicVoxel;
		_exportMedia = settings->_exportMedia;
		_status = settings->_status;
		showSettings = settings->showSettings;
		showStatistics = settings->showStatistics;
		displayMode = settings->displayMode;
		samples = settings->samples;
		exampleFloat = settings->exampleFloat;
	}
};