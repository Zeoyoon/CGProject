#include "application.h"


Application::Application()
{
}

Application::Application(Config config) : _config(config){

	_userSettings.reset(new UserSettings());

	_window.reset(new Window(config.windowConfig));

	_ui.reset(new UserInterface(*_window, config.uiConfig, _userSettings));

	_lastTimeStamp = std::chrono::high_resolution_clock::now();
}

Application::~Application() {
	if (_window != nullptr) {
		_window->cleanup();
	}
}

void Application::run() {
	while (!_window->isClosed()) {
		updateTime();
		handleInput();
		handleUI();
		renderFrame();
		_window->swapBuffers();
		_window->pollEvents();
	}
}

void Application::updateTime() {
	auto now = std::chrono::high_resolution_clock::now();
	_deltaTime = 0.001f * std::chrono::duration<float, std::milli>(now - _lastTimeStamp).count();
	_lastTimeStamp = now;
}