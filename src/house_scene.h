#pragma once
#include <memory>
#include <vector>

#include "../base/application.h"
#include "../base/camera.h"
#include "../base/model.h"
#include "../base/skeleton_model.h"
#include "../base/skybox.h"
#include "my_obj_loader.h"
#include "basic_object.h"
#include "geometry.h"

class SceneRoaming : public Application {
public:
	SceneRoaming();

	SceneRoaming(Application::Config config);


	~SceneRoaming();

	void handleInput() override;

	void handleUI() override;

	void renderFrame() override;
	void draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos);
	void draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos);

private:
	// int prevGeotype = 0;

	const float _cameraMoveSpeed = 5.0f;
	const float _cameraRotateSpeed = 0.02f;
	std::vector<std::shared_ptr<Camera>> _cameras;
	int activeCameraIndex = 0;
	void initShader();


	std::unique_ptr<Model> _house;
	std::unique_ptr<Shader> _shaderS;  // 聚光灯
	std::unique_ptr<Shader> _shaderP;  // 点光源
	//std::unique_ptr<Shader> _shader;

	std::unique_ptr<SkyBox> _skybox;
	
	bool _isLoadingBunny = false;
	std::unique_ptr<MyObjLoader> _bunny;
	std::unique_ptr<Cube> _cube;
	std::unique_ptr<Sphere> _sphere;
	std::unique_ptr<Geometry> _cylinder;
	std::unique_ptr<Geometry> _cone;
	std::unique_ptr<Geometry> _prism;
	std::unique_ptr<Geometry> _pyramid;
	 
	std::unique_ptr<SpotLight> spotLight;
	std::unique_ptr<PointLight> pointLight;

	std::unique_ptr<SkeletonModel> _runningMan;

	unsigned int _runningManShader;

	void initRunningManShader();

};