#pragma once
#include<memory>

#include"../src/ui/window.h"
#include"../src/ui/user_interface.h"

#include "input.h"
#include "shader.h"

class Application {
public:
	struct Config {
		Window::Config windowConfig;
		UserInterface::Config uiConfig;
	};

public:
	Application();

	Application(Config config);

	virtual ~Application();

	void run();

protected:
	struct Config _config;
	std::shared_ptr<UserSettings> _userSettings;

	std::unique_ptr<Statistics> _statistics;

	std::unique_ptr<Window> _window;
	std::unique_ptr<UserInterface> _ui;

		/* timer for fps */
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimeStamp;
	float _deltaTime = 0.0f;

	void updateTime();

	/* derived class can override this function to handle input */
	virtual void handleInput() = 0;

	/* derived class can override this function to handle input */
	virtual void handleUI() = 0;

	/* derived class can override this function to render a frame */
	virtual void renderFrame() = 0;
};