#include<iostream>
#include<stb_image.h>

#include "window.h"




Window::Window(const Window::Config& config) : _config(config) {
	//glfwSetErrorCallback(glfwErrorCallback);

	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("init glfw failure");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, _config.resizable? GLFW_TRUE : GLFW_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	_window = glfwCreateWindow(
		config.width, config.height, config.title.c_str(), nullptr, nullptr);

	if (_window == nullptr) {
		throw std::runtime_error("create glfw window failure");
	}
	if (config.disableCursor) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);

	glfwSetKeyCallback(_window, glfwKeyCallback);
	glfwSetMouseButtonCallback(_window, glfwMousebuttonCallback);
	glfwSetCursorPosCallback(_window, glfwCursorPosCallback);
	glfwSetFramebufferSizeCallback(_window, glfwFramebufferResizeCallback);
	glfwSetScrollCallback(_window, glfwScrollCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cleanup();
		throw std::runtime_error("initialize glad failure");
	}

	_mouseInput.move.xOld = _mouseInput.move.xCurrent = 0.5 * _config.width;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent = 0.5 * _config.height;
	glfwSetCursorPos(getHandle(), _mouseInput.move.xCurrent, _mouseInput.move.yCurrent);

	glViewport(0, 0, config.width, config.height);
	
}

Window::~Window()
{
	cleanup();
	glfwSetErrorCallback(nullptr);
}



GLFWwindow* Window::getHandle() const
{
	return _window;
}

float Window::getContentScale() const
{
	float xScale = 1.0f, yScale = 1.0f;
	glfwGetWindowContentScale(_window, &xScale, &yScale);
	return xScale;
}

void Window::close() const
{
	glfwSetWindowShouldClose(_window, true);
}

bool Window::isClosed() const
{
	return glfwWindowShouldClose(_window);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

void Window::waitEvents() const
{
	glfwWaitEvents();
}

bool Window::isMinimized() const
{
	return getFramebufferSize() == glm::ivec2{ 0,0 };
}

glm::ivec2 Window::getWindowSize() const
{
	glm::ivec2 windowSize{};
	glfwGetWindowSize(_window, &windowSize.x, &windowSize.y);
	return windowSize;
}

glm::ivec2 Window::getFramebufferSize() const
{
	glm::ivec2 framebufferSize{};
	glfwGetFramebufferSize(_window, &framebufferSize.x, &framebufferSize.y);
	return framebufferSize;
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(_window);
}

void Window::cleanup()
{
	if (_window) {
		glfwDestroyWindow(_window);
		_window = nullptr;
	}
	glfwTerminate();
}

void Window::glfwErrorCallback(const int error, const char* const description)
{
	std::cerr << "[GLFW ERROR(" << error << ")]:" << description << std::endl;
}


void Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key != GLFW_KEY_UNKNOWN) {
		Window* _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		_window->_keyboardInput.keyStates[key] = action;
	}
}

void Window::glfwCursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Window* _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	_window->_mouseInput.move.xCurrent = xPos;
	_window->_mouseInput.move.yCurrent = yPos;
}

void Window::glfwMousebuttonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			_window->_mouseInput.click.left = true;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			_window->_mouseInput.click.middle = true;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			_window->_mouseInput.click.right = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			_window->_mouseInput.click.left = false;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			_window->_mouseInput.click.middle = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			_window->_mouseInput.click.right = false;
			break;
		}
	}
}

void Window::glfwScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window* _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	_window->_mouseInput.scroll.x += xOffset;
	_window->_mouseInput.scroll.y += yOffset;
}

void Window::glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	Window* _window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	_window->_config.width = width;
	_window->_config.height = height;
	_window->_config.resizable = true;
	glViewport(0, 0, width, height);
}





