#ifndef OPENGL_WINDOW_INCLUDED
#define OPENGL_WINDOW_INCLUDED

#include <exception>
#define GLEW_STATIC
#include <GLFW\glfw3.h>
#include "Interfaces.h"

namespace openGL
{
	struct InputInterface
	{
		static void* gameInstance;

		template<class Game>
		static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			((Game*)gameInstance)->OnKeyDown(key, action);
		}

		template<class Game>
		static void OnMouseDown(GLFWwindow* window, int button, int action, int mods)
		{
			((Game*)gameInstance)->OnMouseDown(button, action);
		}

		template<class Game>
		static void OnMouseMove(GLFWwindow* window, double x, double y)
		{
			((Game*)gameInstance)->OnMouseMove(x, y);
		}
	};

	void* InputInterface::gameInstance;

	struct Window : IUpdatable
	{
		int Width() const { return _width; }
		int Height() const { return _height; }
		float Ratio() const { return (float)_width / _height; }

		static void error_callback(int error, const char* description)
		{
			///MessageBox::Show(nullptr, )
			throw std::exception(description);
		}

		Window()
			: _window(nullptr)
		{
			_width = 800;
			_height = 800;

			glfwSetErrorCallback(error_callback);
		}

		template<class Game>
		void Create(Game* game)
		{
			_interface.gameInstance = game;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			_window = glfwCreateWindow(_width, _height, "GLFW", nullptr, nullptr);
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

			//register input callbacks
			glfwSetKeyCallback(_window, InputInterface::OnKeyDown<Game>);
			glfwSetMouseButtonCallback(_window, InputInterface::OnMouseDown<Game>);
			glfwSetCursorPosCallback(_window, InputInterface::OnMouseMove<Game>);
		}

		void Update() override
		{
			SetViewPort();
		}

		int ShouldClose() const
		{
			return glfwWindowShouldClose(_window);
		}

		int GetKey(int key) const
		{
			return glfwGetKey(_window, key);
		}

		void SetTitle(const char* title) const
		{
			glfwSetWindowTitle(_window, title);
		}

		void SetViewPort()
		{
			glfwGetFramebufferSize(_window, &_width, &_height);
			glViewport(0, 0, _width, _height);
		}

		void SwapBuffers() const
		{
			glfwSwapBuffers(_window);
		}

		void Close() const
		{
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}

		void Destroy() const
		{
			if (_window) glfwDestroyWindow(_window);
		}

		virtual ~Window()
		{
			Destroy();
		}

	protected:
		GLFWwindow* _window;
		InputInterface _interface;

		int _width;
		int _height;
	};
}

#endif // !OPENGL_WINDOW_INCLUDED