#pragma once
#include <chrono>
#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../base/input.h"

class Window {
public:
	struct Config {
		bool resizable = true;
		int width;
		int height;
		std::string title;
		bool disableCursor;
		glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}_config;

	/* input handler */
	KeyboardInput _keyboardInput;
	MouseInput _mouseInput;

public:
	Window(const Window::Config& config);
	~Window();
	
public:
	GLFWwindow* getHandle() const;
	float getContentScale() const;
	void close() const;
	bool isClosed() const;
	void pollEvents() const;
	void waitEvents() const;
	bool isMinimized() const;
	glm::ivec2 getWindowSize() const;
	glm::ivec2 getFramebufferSize() const;
	void swapBuffers() const;
	void cleanup();

public:
	GLFWwindow* _window = nullptr;

	static void glfwErrorCallback(const int error, const char* const description);
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwCursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void glfwMousebuttonCallback(GLFWwindow* window, int button, int action, int mods);

	static void glfwScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height);
};