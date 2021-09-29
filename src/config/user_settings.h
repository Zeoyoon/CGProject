#pragma once
#include<iostream>

class UserSettings {

public:

	enum GameMode
	{
		SceneALLRoaming,
		SceneFPSRoaming,
		SceneCreation,
	};
	// 当前状态信息
	struct Status {
		bool basicVoxel; // 导入基本体素
		int gameMode;
	}_status;

	enum BasicVoxel
	{
		Cube,
		Ball,
		Cylinder,
		Cone,
		Prism,
		Pyramid,
		House,
	};

	enum ComplexVoxel
	{
		Robot,
	};

	// 导入基本体素
	struct ImportBasicVoxel {	
		//int importType;

		bool importCube;    // 立方体
		bool firstimportCube;    // 立方体
		bool importBall;    // 球
		bool firstimportBall;    // 球
		bool importCylinder;// 圆柱
		bool firstimportCylinder;// 圆柱
		bool importCone;    // 圆锥
		bool firstimportCone;    // 圆锥
		bool importPrism;   // 多面棱柱
		bool firstimportPrism;   // 多面棱柱
		bool importPyramid; // 多面棱台
		bool firstimportPyramid; // 多面棱台
		int actionId;
	}_importBasicVoxel;

	enum ImportGeoType {
		import_cube,
		import_sphere,
		import_cylinder,
		import_cone,
		import_prism,
		import_Pyramid
	};

	// 导入基本体素
	struct ImportComplexVoxel {
		bool importRobot;    // 机械人
		bool isBeginimportRobot;    // 机械人
		int actionId;
	}_importComplexVoxel;

	struct ExportMedia {
		bool saveImage;
		bool Ball;
		bool Cube;
	}_exportMedia;

	struct CommonSettings {
		bool help;
	}_commonSettings;

	enum RenderMode
	{
		Simple,
		Blend,
		Checker,
	}_renderMode;

	struct MaterialTexture {
		bool editInit;
		int renderMode;
		float Ns; // 高光度		
		glm::vec3 Ka; //材质颜色光照		
		glm::vec3 Kd; //漫反射		
		glm::vec3 Ks; //镜反射
		float blend;
		int repeat;
		glm::vec3 checkerColors0; 
		glm::vec3 checkerColors1;

		int actionId;
	}_materialTexture;
	
	enum editLight
	{
		None = -1,
		SpotLight,
		PointLight,
	};
	struct LightSetting {
		bool editInit; // 是否初始点击编辑光源按钮
		int actionId; // 编辑的光源id
		float intensity;
		glm::vec3 color;
		float moveX;
		float moveY;
		float moveZ;
	}_lightSettings;

	struct AnimationSettings {
		bool runMan;
	}_animationSettings;

	bool showSettings;
	bool showStatistics;
	int displayMode;
	uint32_t samples;
	float exampleFloat;

	UserSettings() {
		_animationSettings.runMan = false;
		_lightSettings.editInit = true;
		_lightSettings.actionId = editLight::None;
		_lightSettings.color = glm::vec3(1.0f, 1.0f, 1.0f);

		_materialTexture.renderMode = 0;
		_materialTexture.actionId = -1;
		_materialTexture.blend = 0.0f;
		_materialTexture.repeat = 10;
		_materialTexture.checkerColors0 = glm::vec3(1.0f, 1.0f, 1.0f);
		_materialTexture.checkerColors1 = glm::vec3(0.0f, 0.0f, 0.0f);

		_importComplexVoxel.importRobot = false;
		_importComplexVoxel.isBeginimportRobot = true;
		_importComplexVoxel.actionId = -1;
		_status.gameMode = GameMode::SceneCreation;
		_commonSettings = { false };
		_importBasicVoxel = { false,true,false,true,false,true,false,true,false,true,false,true,-1 };
		_exportMedia = { false };
		showSettings = false;
		showStatistics = false;
		samples = 120;
		exampleFloat = 1.0f;
	}

	UserSettings(std::shared_ptr<UserSettings> settings) {
		_animationSettings = settings->_animationSettings;
		_lightSettings = settings->_lightSettings;
		_materialTexture = settings->_materialTexture;
		_importComplexVoxel = settings->_importComplexVoxel;
		_commonSettings = settings->_commonSettings;
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