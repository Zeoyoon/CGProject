#pragma once

#include<memory>
#include<imgui.h>
#include"window.h"
#include"../config/user_settings.h"

struct Statistics {
	uint32_t totalVertices;
	uint32_t totalFaces;
	uint32_t totalCustomObjects;
	uint32_t windowWidth, windowHeight;
	uint32_t totalSamples;
	float framePerSecond;

};

class UserInterface {
public:
	struct Config {
		std::string font;
	};
public:
	UserInterface(const Window& window, const Config& config, std::shared_ptr<UserSettings> settings);
	UserInterface(const UserInterface&) = delete;
	UserInterface(UserInterface&&) = delete;
	UserInterface operator=(const UserInterface&) = delete;
	UserInterface operator=(UserInterface&&) = delete;

	~UserInterface();

	void render(const Statistics& statistics);
	std::shared_ptr<UserSettings> _userSettings;
private:
	

	void setCustomStyle(ImGuiStyle& style);
	void drawMainMenu();

	void drawHelpPanel();

	void drawBallMaterialTexturePanel();
	void drawCubeMaterialTexturePanel();

	void drawSpotLightPanel();
	void drawPointLightPanel();

	void drawSettingsPanel();
	void drawStatisticsPanel(Statistics statistics);
};
