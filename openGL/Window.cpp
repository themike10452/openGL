#include "Window.h"

namespace openGL
{
	/// Getters
	int Window::Width() const { return _width; }
	int Window::Height() const { return _height; }
	float Window::Ratio() const { return (float)_width / _height; }
	/// -------

	Window::Window()
	{
		this->Create();

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) throw std::exception("glewInit() failed");

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Window::Create()
	{
		if (!glfwInit()) throw std::exception("glfwinit() failed");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		//glfwWindowHint(GLFW_SAMPLES, 8);

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

		_window = glfwCreateWindow(_width = videoMode->width / 2, _height = videoMode->height / 2, "GLFW", nullptr/*primaryMonitor*/, nullptr);
		if (!_window) throw std::exception("Failed to create window");
		glfwMakeContextCurrent(_window);

		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		std::cout << "GL_VERSION: " << version << std::endl;
		std::cout << "GLSL_VERSION: " << glslVersion << std::endl;
		std::cout << "GL_VENDOR: " << vendor << std::endl;

		glfwSwapInterval(1);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::Update()
	{
		SetViewPort();
	}

	int Window::ShouldClose() const
	{
		return glfwWindowShouldClose(_window);
	}

	int Window::GetKey(int key) const
	{
		return glfwGetKey(_window, key);
	}

	void Window::SetTitle(const char* title) const
	{
		glfwSetWindowTitle(_window, title);
	}

	void Window::SetViewPort()
	{
		glfwGetFramebufferSize(_window, &_width, &_height);
		glViewport(0, 0, _width, _height);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(_window);
	}

	void Window::Close() const
	{
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}

	void Window::Destroy() const
	{
		if (_window) glfwDestroyWindow(_window);
	}

	Window::~Window()
	{
		Destroy();
	}

	std::vector<void*> InputInterface::InputListeners;
}