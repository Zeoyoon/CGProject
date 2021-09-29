#include <unordered_map>
#include "house_scene.h"
#include "../base/vertex.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include<memory>

#include"animation/export_media.h"

#include "config/option.h"
struct Option globalOption;

const std::string modelPath = "../model/house/House w. pool.obj";
const std::vector<std::string> skyboxTexturePaths = {
	"../model/starfield/Right_Tex.jpg",
	"../model/starfield/Left_Tex.jpg",
	"../model/starfield/Up_Tex.jpg",
	"../model/starfield/Down_Tex.jpg",
	"../model/starfield/Front_Tex.jpg",
	"../model/starfield/Back_Tex.jpg"
};

const std::string runningManModelPath[2] = {
	"../model/model.dae",
	"../model/diffuse.png"
};

static float runManAngle = 0;

SceneRoaming::SceneRoaming(Application::Config config):Application(config){

	// set input mode
	_userSettings->displayMode = true;
	_window->_mouseInput.move.xOld = _window->_mouseInput.move.xCurrent = 0.5 * _window->_config.width;
	_window->_mouseInput.move.yOld = _window->_mouseInput.move.yCurrent = 0.5 * _window->_config.height;
	glfwSetCursorPos(_window->getHandle(), _window->_mouseInput.move.xCurrent, _window->_mouseInput.move.yCurrent);

	// init skybox
	_skybox.reset(new SkyBox(skyboxTexturePaths));

	// init cameras
	_cameras.resize(2);

	const float aspect = 1.0f * _window->_config.width / _window->_config.height;
	constexpr float znear = 0.1f;
	constexpr float zfar = 10000.0f;

	// perspective camera
	_cameras[0] = std::make_shared<PerspectiveCamera>(
		glm::radians(60.0f), aspect, 0.1f, 10000.0f);
	_cameras[0]->position = glm::vec3(0.0f, 0.0f, 15.0f);

	// orthographic camera
	_cameras[1] = std::make_shared<OrthographicCamera>(
		-4.0f * aspect, 4.0f * aspect, -4.0f, 4.0f, znear, zfar);
	_cameras[1]->position = glm::vec3(0.0f, 0.0f, 15.0f);

	// init model
	_house.reset(new Model(modelPath));

	// init bunny
	_bunny.reset(new MyObjLoader("../model/house/nanosuit.obj", "../model/house/planet_Quom1200.png"));


	// init cube & sphere
	Material geoMat = { 32.0f,glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.01f,0.01f,0.01f,1.0f),1.0f,1.0f,2 };
	_cube.reset(new Cube(geoMat, std::string("../model/house/planet_Quom1200.png")));	
	_sphere.reset(new Sphere(geoMat));

	// init geometry
	_cylinder.reset(new Geometry(2));
	_cone.reset(new Geometry(3));
	_prism.reset(new Geometry(4));
	_pyramid.reset(new Geometry(5));

	// init light
	pointLight.reset(new PointLight(1.0f, 0.01f, 0.01f));
	pointLight->color = glm::vec3(1.0f, 0.0f, 0.0f);  // 1.0, 0.0, 0.0  红
	pointLight->intensity = 1.0f;
	pointLight->position = glm::vec3(0.0f, 10.0f, 0.0f);
	spotLight.reset(new SpotLight(glm::radians(20.0f), glm::radians(25.0f), 1.0f, 0.001f, 0.001f));
	spotLight->color = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight->intensity = 1.0f;
	spotLight->position = _cameras[activeCameraIndex]->position;
	spotLight->rotation = _cameras[activeCameraIndex]->getFront();


	// init shader
	initShader();

	_runningMan.reset(new SkeletonModel(runningManModelPath[0], runningManModelPath[1]));

	initRunningManShader();
}


SceneRoaming::~SceneRoaming() {

}

void SceneRoaming::handleInput() {
	const float cameraMoveSpeed = 5.0f;
	const float cameraRotateSpeed = 0.02f;
	static float yAngle = 0.0f;


	if (_window->_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
		_window->close();
		return;
	}

	if (_window->_keyboardInput.keyStates[GLFW_KEY_ENTER] == GLFW_KEY_LEFT_ALT) {
		_ui->_userSettings->_status.gameMode = (_ui->_userSettings->_status.gameMode + 1) % 2;
	}

	if (_window->_keyboardInput.keyStates[GLFW_KEY_ENTER] == GLFW_PRESS) {
		std::cout << "switch camera" << std::endl;
		// switch camera
		activeCameraIndex = (activeCameraIndex + 1) % _cameras.size();
		_window->_keyboardInput.keyStates[GLFW_KEY_ENTER] = GLFW_RELEASE;
		return;
	}

	// 切换光照
	if (_window->_keyboardInput.keyStates[GLFW_KEY_C] == GLFW_PRESS) {
		//std::cout << "switch light" << std::endl;
		// switch light
		if (globalOption.light.compare("PointLight") == 0)
			globalOption.light.assign(std::string("SpotLight"));
		else if (globalOption.light.compare("SpotLight") == 0)
			globalOption.light.assign(std::string("PointLight"));
		_window->_keyboardInput.keyStates[GLFW_KEY_C] = GLFW_RELEASE;
		return;
	}

	std::shared_ptr<Camera> camera = _cameras[activeCameraIndex];
	std::shared_ptr<Camera> camera2 = _cameras[(activeCameraIndex+1)%2];  


	// 位置移动
	if (_window->_keyboardInput.keyStates[GLFW_KEY_W] != GLFW_RELEASE) {
		camera->position += _cameraMoveSpeed * _deltaTime * camera->getFront();
		camera2->position += _cameraMoveSpeed * _deltaTime * camera->getFront();  // keep two cameras same position
		spotLight->position = camera->position;
	}

	if (_window->_keyboardInput.keyStates[GLFW_KEY_A] != GLFW_RELEASE) {
		camera->position -= _cameraMoveSpeed * _deltaTime * glm::normalize(glm::cross(camera->getFront(), camera->getUp()));
		camera2->position -= _cameraMoveSpeed * _deltaTime * glm::normalize(glm::cross(camera->getFront(), camera->getUp()));
		spotLight->position = camera->position;
	}

	if (_window->_keyboardInput.keyStates[GLFW_KEY_S] != GLFW_RELEASE) {
		camera->position -= _cameraMoveSpeed * _deltaTime * camera->getFront();
		camera2->position -= _cameraMoveSpeed * _deltaTime * camera->getFront();
		spotLight->position = camera->position;
	}

	if (_window->_keyboardInput.keyStates[GLFW_KEY_D] != GLFW_RELEASE) {
		camera->position += _cameraMoveSpeed * _deltaTime * glm::normalize(glm::cross(camera->getFront(), camera->getUp()));
		camera2->position += _cameraMoveSpeed * _deltaTime * glm::normalize(glm::cross(camera->getFront(), camera->getUp()));
		spotLight->position = camera->position;
	}


	// 视角方向移动
	if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneALLRoaming)
	{
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE) {
			const float deltaY = 10.0f;
			const float angle = cameraRotateSpeed * _deltaTime * deltaY;
			if (yAngle < 90.0f / 180.0f * PI)
			{
				yAngle += angle;
				const glm::vec3 axis = camera->getRight();
				camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
				spotLight->rotation = camera->rotation;
			}
		}

		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE) {
			const float deltaY = -10.0f;
			const float angle = cameraRotateSpeed * _deltaTime * deltaY;
			if (yAngle > -90.0f / 180.0f * PI)
			{
				yAngle += angle;
				const glm::vec3 axis = camera->getRight();
				camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
				spotLight->rotation = camera->rotation;
			}
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE) {
			const float deltaX = -10.0f;
			const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
			runManAngle += angle;
			const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
			camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
			spotLight->rotation = camera->rotation;
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE) {
			const float deltaX = 10.0f;
			const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
			runManAngle += angle;
			const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
			camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
			spotLight->rotation = camera->rotation;
		}
	}
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneFPSRoaming)
	{
		
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE) {
			const float deltaX = -10.0f;
			const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
			runManAngle += angle;
			const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
			camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
			spotLight->rotation = camera->rotation;
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE) {
			const float deltaX = 10.0f;
			const float angle = -cameraRotateSpeed * _deltaTime * deltaX;
			runManAngle += angle;
			const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
			camera->rotation = glm::quat(cos(angle * 0.5f), axis.x * sin(angle * 0.5f), axis.y * sin(angle * 0.5f), axis.z * sin(angle * 0.5f)) * camera->rotation;
			spotLight->rotation = camera->rotation;
		}
		
	}
	// 操作立方体
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId == UserSettings::BasicVoxel::Cube)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_cube->move(MoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE)
			_cube->move(MoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE)
			_cube->move(MoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE)
			_cube->move(MoveDirection::Right, _deltaTime);
		
		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_cube->_rotateSpeed = 0.2f - _cube->_rotateSpeed;
		}
		_cube->rotate(RotateDirection::CCW, _deltaTime);  
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_cube->scale *= glm::vec3(0.99f, 0.99f, 0.99f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_cube->scale *= glm::vec3(1.01f, 1.01f, 1.01f);
		}
		
		return;
	}

	// 操作球
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId==UserSettings::BasicVoxel::Ball)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_sphere->move(MoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE)
			_sphere->move(MoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE)
			_sphere->move(MoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE)
			_sphere->move(MoveDirection::Right, _deltaTime);
		
		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_sphere->_rotateSpeed = 0.2f - _sphere->_rotateSpeed;
		}
		_sphere->rotate(RotateDirection::CCW, _deltaTime);  
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_sphere->scale *= glm::vec3(0.99f, 0.99f, 0.99f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_sphere->scale *= glm::vec3(1.01f, 1.01f, 1.01f);
		}
		return;
	}
	
	// 操作圆柱体
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId == UserSettings::BasicVoxel::Cylinder)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_cylinder->move(GeoMoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE) 
			_cylinder->move(GeoMoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE) 
			_cylinder->move(GeoMoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE) 
			_cylinder->move(GeoMoveDirection::Right, _deltaTime); 

		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_cylinder->_rotateSpeed = 0.2f - _cylinder->_rotateSpeed;
		}
		_cylinder->rotate(GeoRotateDirection::CCW, _deltaTime);  // 旋转
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_cylinder->scale *= glm::vec3(0.99f, 0.99f, 0.99f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_cylinder->scale *= glm::vec3(1.01f, 1.01f, 1.01f);
		}
		return;
	}

	// 操作圆锥
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId == UserSettings::BasicVoxel::Cone)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_cone->move(GeoMoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE)
			_cone->move(GeoMoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE)
			_cone->move(GeoMoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE)
			_cone->move(GeoMoveDirection::Right, _deltaTime);

		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_cone->_rotateSpeed = 0.2f - _cone->_rotateSpeed;
		}
		_cone->rotate(GeoRotateDirection::CCW, _deltaTime);  // 旋转
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_cone->scale *= glm::vec3(0.99f, 0.99f, 0.99f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_cone->scale *= glm::vec3(1.01f, 1.01f, 1.01f);
		}

		return;
	}

	// 操作多面棱柱
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId == UserSettings::BasicVoxel::Prism)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_prism->move(GeoMoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE)
			_prism->move(GeoMoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE)
			_prism->move(GeoMoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE)
			_prism->move(GeoMoveDirection::Right, _deltaTime);

		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_prism->_rotateSpeed = 0.2f - _prism->_rotateSpeed;
		}
		_prism->rotate(GeoRotateDirection::CCW, _deltaTime);  // 旋转
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_prism->scale *= glm::vec3(0.99f, 0.99f, 0.99f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_prism->scale *= glm::vec3(1.01f, 1.01f, 1.01f);
		}

		return;
	}

	// 操作多面棱台
	else if (_userSettings->_status.gameMode == UserSettings::GameMode::SceneCreation && _userSettings->_importBasicVoxel.actionId == UserSettings::BasicVoxel::Pyramid)
	{
		// 平移
		if (_window->_keyboardInput.keyStates[GLFW_KEY_UP] != GLFW_RELEASE)
			_pyramid->move(GeoMoveDirection::Forward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_DOWN] != GLFW_RELEASE)
			_pyramid->move(GeoMoveDirection::Backward, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_LEFT] != GLFW_RELEASE)
			_pyramid->move(GeoMoveDirection::Left, _deltaTime);
		if (_window->_keyboardInput.keyStates[GLFW_KEY_RIGHT] != GLFW_RELEASE)
			_pyramid->move(GeoMoveDirection::Right, _deltaTime);

		// 旋转
		if (_window->_keyboardInput.keyStates[GLFW_KEY_SPACE] == GLFW_RELEASE && _window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] == GLFW_PRESS) {
			_pyramid->_rotateSpeed = 0.2f - _pyramid->_rotateSpeed;
		}
		_pyramid->rotate(GeoRotateDirection::CCW, _deltaTime);  // 旋转
		_window->_keyboardInput.prev_keyStates[GLFW_KEY_SPACE] = _window->_keyboardInput.keyStates[GLFW_KEY_SPACE];

		// 缩放
		if (_window->_keyboardInput.keyStates[GLFW_KEY_MINUS] != GLFW_RELEASE) {
			_pyramid->scale *= glm::vec3(0.95f, 0.95f, 0.95f);
		}
		if (_window->_keyboardInput.keyStates[GLFW_KEY_EQUAL] != GLFW_RELEASE) {
			_pyramid->scale *= glm::vec3(1.05f, 1.05f, 1.05f);
		}
		return;
	}

	// 透视照相机的视角张角变化（在非创造模式下）
	// zoom in
	if (_window->_keyboardInput.keyStates[GLFW_KEY_I] != GLFW_RELEASE) {
		std::shared_ptr<PerspectiveCamera> camera = std::dynamic_pointer_cast<PerspectiveCamera>(_cameras[0]);
		if (camera->fovy <= glm::radians(75.0f))
			camera->fovy += glm::radians(5.0f);

		_window->_keyboardInput.keyStates[GLFW_KEY_I] = GLFW_RELEASE;
		return;
	}
	// zoom out
	if (_window->_keyboardInput.keyStates[GLFW_KEY_O] != GLFW_RELEASE) {
		std::shared_ptr<PerspectiveCamera> camera = std::dynamic_pointer_cast<PerspectiveCamera>(_cameras[0]);
		if (camera->fovy >= glm::radians(1.0f))
			camera->fovy -= glm::radians(5.0f);

		_window->_keyboardInput.keyStates[GLFW_KEY_O] = GLFW_RELEASE;
		return;
	}
	// zoom to fit
	if (_window->_keyboardInput.keyStates[GLFW_KEY_P] != GLFW_RELEASE) {
		std::shared_ptr<PerspectiveCamera> camera = std::dynamic_pointer_cast<PerspectiveCamera>(_cameras[0]);
		camera->fovy = glm::radians(60.0f);

		_window->_keyboardInput.keyStates[GLFW_KEY_P] = GLFW_RELEASE;
		return;
	}

}

void SceneRoaming::handleUI()
{
	_userSettings.reset(new UserSettings(_ui->_userSettings));

	
	//导出截图或录像
	if (_userSettings->_exportMedia.saveImage) {
		ExportMedia::ExportImage(_window->_config.width, _window->_config.height);
	}

	if (_userSettings->_exportMedia.Cube) {
		_cube->exportObj("..\\model\\cube.obj");	
		_ui->_userSettings->_exportMedia.Cube = false;
	}
	if (_userSettings->_exportMedia.Ball) {
		_sphere->exportObj("..\\model\\sphere.obj");
		_ui->_userSettings->_exportMedia.Ball = false;		
	}

	switch (_userSettings->_materialTexture.actionId)
	{
	case UserSettings::BasicVoxel::Cube:
		if (_ui->_userSettings->_materialTexture.editInit) {
			_ui->_userSettings->_materialTexture.Ns = _cube->_material.Ns;
			_ui->_userSettings->_materialTexture.Ka = _cube->_material.Ka;
			_ui->_userSettings->_materialTexture.Kd = _cube->_material.Kd;
			_ui->_userSettings->_materialTexture.Ks = _cube->_material.Ks;
			_ui->_userSettings->_materialTexture.editInit = false;
		}
		_cube->_material.Ns = _ui->_userSettings->_materialTexture.Ns;
		_cube->_material.Ka = glm::vec4(_ui->_userSettings->_materialTexture.Ka, 1.0f);
		_cube->_material.Kd = glm::vec4(_ui->_userSettings->_materialTexture.Kd, 1.0f);
		_cube->_material.Ks = glm::vec4(_ui->_userSettings->_materialTexture.Ks, 1.0f);

		if (_userSettings->_materialTexture.renderMode == UserSettings::RenderMode::Simple) {
			_cube->ifBlend = false;
			_cube->blend_coef = 0.0f;
		}
		else if (_userSettings->_materialTexture.renderMode == UserSettings::RenderMode::Blend) {
			_cube->ifBlend = true;
			_cube->blend_coef = _ui->_userSettings->_materialTexture.blend;
		}
		break;

	case UserSettings::BasicVoxel::Ball:
		if (_ui->_userSettings->_materialTexture.editInit) {
			_ui->_userSettings->_materialTexture.Ns = _sphere->_material.Ns;
			_ui->_userSettings->_materialTexture.Ka = _sphere->_material.Ka;
			_ui->_userSettings->_materialTexture.Kd = _sphere->_material.Kd;
			_ui->_userSettings->_materialTexture.Ks = _sphere->_material.Ks;
			_ui->_userSettings->_materialTexture.repeat = _sphere->_repeat;
			_ui->_userSettings->_materialTexture.checkerColors0 = _sphere->_color1;
			_ui->_userSettings->_materialTexture.checkerColors1 = _sphere->_color2;
			_ui->_userSettings->_materialTexture.editInit = false;
		}

		_sphere->_material.Ns = _ui->_userSettings->_materialTexture.Ns;
		_sphere->_material.Ka = glm::vec4(_ui->_userSettings->_materialTexture.Ka, 1.0f);
		_sphere->_material.Kd = glm::vec4(_ui->_userSettings->_materialTexture.Kd, 1.0f);
		_sphere->_material.Ks = glm::vec4(_ui->_userSettings->_materialTexture.Ks, 1.0f);
		_sphere->_repeat = _ui->_userSettings->_materialTexture.repeat;
		_sphere->_color1 = _ui->_userSettings->_materialTexture.checkerColors0, 1.0f;
		_sphere->_color2 = _ui->_userSettings->_materialTexture.checkerColors1, 1.0f;

		if (_userSettings->_materialTexture.renderMode == UserSettings::RenderMode::Simple) {
			_sphere->rendermode = 0;
		}
		else if (_userSettings->_materialTexture.renderMode == UserSettings::RenderMode::Checker) {
			_sphere->rendermode = 2;
		}
		break;

	default:
		_ui->_userSettings->_materialTexture.editInit = true;
		break;
	}

	float currentX = pointLight->position.x;
	float currentY = pointLight->position.y;
	float currentZ = pointLight->position.z;

	// 处理光源
	switch (_userSettings->_lightSettings.actionId)
	{
	case UserSettings::editLight::PointLight:
		if (_ui->_userSettings->_lightSettings.editInit) {
			_ui->_userSettings->_lightSettings.color = pointLight->color;
			_ui->_userSettings->_lightSettings.intensity = pointLight->intensity;
			_ui->_userSettings->_lightSettings.moveX = 0;
			_ui->_userSettings->_lightSettings.moveY = 0;
			_ui->_userSettings->_lightSettings.moveZ = 0;
			_ui->_userSettings->_lightSettings.editInit = false;
		}
		pointLight->color = _ui->_userSettings->_lightSettings.color;
		pointLight->intensity = _ui->_userSettings->_lightSettings.intensity;
		pointLight->position = glm::vec3(0.0f, 10.0f, 0.0f);
		pointLight->position += glm::vec3(_ui->_userSettings->_lightSettings.moveX, _ui->_userSettings->_lightSettings.moveY, _ui->_userSettings->_lightSettings.moveZ);
		break;
	case UserSettings::editLight::SpotLight:
		if (_ui->_userSettings->_lightSettings.editInit) {
			_ui->_userSettings->_lightSettings.color = spotLight->color;
			_ui->_userSettings->_lightSettings.intensity = spotLight->intensity;
			_ui->_userSettings->_lightSettings.editInit = false;
		}
		spotLight->color = _ui->_userSettings->_lightSettings.color;
		spotLight->intensity = _ui->_userSettings->_lightSettings.intensity;
		break;
	default:
		_ui->_userSettings->_lightSettings.editInit = true;
		break;
	}

}

void SceneRoaming::renderFrame() {
	
	//showFpsInWindowTitle();
	glClearColor(_window->_config.clearColor.r, _window->_config.clearColor.g, _window->_config.clearColor.b, _window->_config.clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = _cameras[activeCameraIndex]->getProjectionMatrix();
	glm::mat4 view = _cameras[activeCameraIndex]->getViewMatrix();
	
	glm::mat4 model = _cameras[activeCameraIndex]->getModelMatrix();
	model = glm::translate(model, glm::vec3(_cameras[activeCameraIndex]->getFront().x*3.0, 0.0f, _cameras[activeCameraIndex]->getFront().z*3.0));
	// 绘制场景
	if (globalOption.light.compare("PointLight") == 0)
		draw(projection, view, *pointLight, pointLight->position);
	else if (globalOption.light.compare("SpotLight") == 0)
		draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);

	// 绘制天空盒 天空盒不受光照限制
	_skybox->draw(projection, view);

	// 绘制obj模型，先判断是否要绘制模型
	if (_userSettings->_importComplexVoxel.importRobot)
	{	// 调整位置、旋转和缩放
		static glm::mat4 m;
		if (_ui->_userSettings->_importComplexVoxel.isBeginimportRobot) {
			m = glm::translate(_cameras[activeCameraIndex]->getModelMatrix(), glm::vec3(0.0f, -0.7f, -1.5f));
			m = glm::scale(m, glm::vec3(0.07f));
			_ui->_userSettings->_importComplexVoxel.isBeginimportRobot = false;
		}
		
		if (globalOption.light.compare("PointLight") == 0)
			_bunny->draw(projection, view, m, *pointLight, pointLight->position);
		else if (globalOption.light.compare("SpotLight") == 0)
			_bunny->draw(projection, view, m, *spotLight, _cameras[activeCameraIndex]->position);
	} else _ui->_userSettings->_importComplexVoxel.isBeginimportRobot = true;
	
	// 绘制基本体素 方块
	if (_userSettings->_importBasicVoxel.importCube)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportCube) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_cube->scale = glm::vec3{ 0.7f, 0.7f, 0.7f };
			_cube->position = glm::vec3(x - 2.5f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportCube = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			_cube->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_cube->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);
		}			
	} else _ui->_userSettings->_importBasicVoxel.firstimportCube = true;

	// 绘制基本体素 球体
	if (_userSettings->_importBasicVoxel.importBall)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportBall) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_sphere->scale = glm::vec3{ 0.4f, 0.4f, 0.4f };
			_sphere->position = glm::vec3(x - 1.5f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportBall = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			
			_sphere->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_sphere->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);			
		}
		//_sphere->rotate(RotateDirection::CCW, _deltaTime);  // 旋转 
	}
	else _ui->_userSettings->_importBasicVoxel.firstimportBall = true;

	
	// 绘制基本体素 圆柱
	if (_userSettings->_importBasicVoxel.importCylinder)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportCylinder) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_cylinder->scale = glm::vec3{ 0.4f, 0.4f, 0.4f };
			_cylinder->position = glm::vec3(x - 0.6f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportCylinder = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			_cylinder->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_cylinder->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);
		}
	}
	else _ui->_userSettings->_importBasicVoxel.firstimportCylinder = true;

	// 绘制基本体素 圆锥
	if (_userSettings->_importBasicVoxel.importCone)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportCone) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_cone->scale = glm::vec3{ 0.4f, 0.4f, 0.4f };
			_cone->position = glm::vec3(x + 0.3f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportCone = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			_cone->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_cone->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);
		}
	}
	else _ui->_userSettings->_importBasicVoxel.firstimportCone = true;

	// 绘制基本体素 多面棱柱
	if (_userSettings->_importBasicVoxel.importPrism)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportPrism) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_prism->scale = glm::vec3{ 0.4f, 0.4f, 0.4f };
			_prism->position = glm::vec3(x + 1.2f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportPrism = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			_prism->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_prism->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);
		}
	}
	else _ui->_userSettings->_importBasicVoxel.firstimportPrism = true;

	// 绘制基本体素 多面棱台
	if (_userSettings->_importBasicVoxel.importPyramid)
	{
		if (_ui->_userSettings->_importBasicVoxel.firstimportPyramid) {
			float x = _cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x * 3;
			float z = _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z * 3;
			_pyramid->scale = glm::vec3{ 0.35f, 0.3f, 0.35f };
			_pyramid->position = glm::vec3(x + 2.2f, -0.5f, z);
			_ui->_userSettings->_importBasicVoxel.firstimportPyramid = false;
		}
		if (globalOption.light.compare("PointLight") == 0) {
			_pyramid->draw(projection, view, *pointLight, pointLight->position);
		}
		else if (globalOption.light.compare("SpotLight") == 0) {
			_pyramid->draw(projection, view, *spotLight, _cameras[activeCameraIndex]->position);
		}
	}
	else _ui->_userSettings->_importBasicVoxel.firstimportPyramid = true;

	// 导入骨骼动画
	if (_ui->_userSettings->_animationSettings.runMan) {
		// initialize projection view and model matrix
		glm::mat4 projectionMatrix = _cameras[activeCameraIndex]->getProjectionMatrix();
		glm::mat4 viewMatrix = _cameras[activeCameraIndex]->getViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;


		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(_cameras[activeCameraIndex]->position.x + _cameras[activeCameraIndex]->getFront().x, _cameras[activeCameraIndex]->position.y - 1.0, _cameras[activeCameraIndex]->position.z + _cameras[activeCameraIndex]->getFront().z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(.15f, .15f, .15f));
		modelMatrix = glm::rotate(modelMatrix, PI, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, PI - runManAngle, glm::vec3(0, 1, 0));

		float elapsedTime = (float)glfwGetTime();
		_runningMan->getPose(_runningMan->_animation, _runningMan->_skeleton, elapsedTime, _runningMan->_currentPose, _runningMan->_identity, _runningMan->_globalInverseTransform);


		glUseProgram(_runningManShader);
		//get all shader uniform locations
		uint viewProjectionMatrixLocation = glGetUniformLocation(_runningManShader, "view_projection_matrix");
		uint modelMatrixLocation = glGetUniformLocation(_runningManShader, "model_matrix");
		uint boneMatricesLocation = glGetUniformLocation(_runningManShader, "bone_transforms");
		uint textureLocation = glGetUniformLocation(_runningManShader, "diff_texture");
		glUniformMatrix4fv(viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(boneMatricesLocation, _runningMan->_boneCount, GL_FALSE, glm::value_ptr(_runningMan->_currentPose[0]));
		_runningMan->draw(_runningManShader);
	}

	Statistics sta;
	_ui->render(sta);
}

void SceneRoaming::draw(glm::mat4 projection, glm::mat4 view, SpotLight& light, glm::vec3 viewPos)
{
	_shaderS->use();
	_shaderS->setMat4("projection", projection);
	_shaderS->setMat4("view", view);
	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::mat4_cast(glm::quat(1.0f, 0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	_shaderS->setMat4("model", m);
	_shaderS->setVec3("light.position", light.position);
	_shaderS->setVec3("light.color", light.color);
	_shaderS->setFloat("light.intensity", light.intensity);
	_shaderS->setFloat("light.constant", light.kc);
	_shaderS->setFloat("light.linear", light.kl);
	_shaderS->setFloat("light.quadratic", light.kq);
	_shaderS->setVec3("viewPos", viewPos);
	_shaderS->setVec3("light.direction", _cameras[activeCameraIndex]->getFront());
	_shaderS->setFloat("light.cutOff", glm::cos(light.cutOff));
	_shaderS->setFloat("light.outerCutOff", glm::cos(light.outerCutOff));
	_house->draw(*_shaderS);
}

void SceneRoaming::draw(glm::mat4 projection, glm::mat4 view, PointLight& light, glm::vec3 viewPos)
{
	_shaderP->use();
	_shaderP->setMat4("projection", projection);
	_shaderP->setMat4("view", view);
	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::mat4_cast(glm::quat(1.0f, 0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	_shaderP->setMat4("model", m);
	_shaderP->setVec3("light.position", light.position);
	_shaderP->setVec3("light.color", light.color);
	_shaderP->setFloat("light.intensity", light.intensity);
	_shaderP->setFloat("light.constant", light.kc);
	_shaderP->setFloat("light.linear", light.kl);
	_shaderP->setFloat("light.quadratic", light.kq);
	_shaderP->setVec3("viewPos", viewPos);
	_house->draw(*_shaderP);
}


void SceneRoaming::initShader() {
	// 着色器
	const char* vertCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
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
		"void main()\n"
		"{    \n"
		"	// ambient\n"
		"    vec3 ambient = light.color * Ambient.rgb ;\n"
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
		"    vec3 result = (ambient + diffuse + specular);\n"
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
		"void main()\n"
		"{    \n"
		"	// ambient\n"
		"    vec3 ambient = light.color * Ambient.rgb ;\n"
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
		"    vec3 result = (ambient + diffuse + specular);\n"
		"    FragColor = vec4(result,1.0f);\n"
		"}\n";

	_shaderP.reset(new Shader(vertCode, fragCode));
	_shaderS.reset(new Shader(vertCode, fragCode1));
}



void SceneRoaming::initRunningManShader() {

	const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 position; 
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;
	layout (location = 3) in vec4 boneIds;
	layout (location = 4) in vec4 boneWeights;

	out vec2 tex_cord;
	out vec3 v_normal;
	out vec3 v_pos;
	out vec4 bw;

	uniform mat4 bone_transforms[50];
	uniform mat4 view_projection_matrix;
	uniform mat4 model_matrix;

	void main()
	{
		bw = vec4(0);
		if(int(boneIds.x) == 1)
		bw.z = boneIds.x;
		//boneWeights = normalize(boneWeights);
		mat4 boneTransform  =  mat4(0.0);
		boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
		boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
		boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
		boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;
		vec4 pos =boneTransform * vec4(position, 1.0);
		gl_Position = view_projection_matrix * model_matrix * pos;
		v_pos = vec3(model_matrix * boneTransform * pos);
		tex_cord = uv;
		v_normal = mat3(transpose(inverse(model_matrix * boneTransform))) * normal;
		v_normal = normalize(v_normal);
	}

	)";

	const char* fragmentShaderSource = R"(
	#version 330 core

	in vec2 tex_cord;
	in vec3 v_normal;
	in vec3 v_pos;
	in vec4 bw;
	out vec4 color;

	uniform sampler2D diff_texture;

	vec3 lightPos = vec3(0.2, 1.0, -3.0);
	
	void main()
	{
		vec3 lightDir = normalize(lightPos - v_pos);
		float diff = max(dot(v_normal, lightDir), 0.2);
		vec3 dCol = diff * texture(diff_texture, tex_cord).rgb; 
		color = vec4(dCol, 1);
	}
	)";

	_runningManShader = createShader(vertexShaderSource, fragmentShaderSource);
}