#ifndef OPENGL_WINDOW_INCLUDED
#define OPENGL_WINDOW_INCLUDED

#include "Common.h"
#include "Interfaces.h"
#include "UserInputInterface.h"

namespace openGL
{
	struct Window : IUpdatable
	{
		/// Getters
		int Width() const;
		int Height() const;
		float Ratio() const;
		/// -------

		Window();

		template<class InputListener>
		void Bind(InputListener* inputListener) const
		{
			_inputInterface.InputListeners.push_back(inputListener);

			//register input callbacks
			glfwSetKeyCallback(_window, InputInterface::OnKeyDown<InputListener>);
			glfwSetMouseButtonCallback(_window, InputInterface::OnMouseDown<InputListener>);
			glfwSetCursorPosCallback(_window, InputInterface::OnMouseMove<InputListener>);
		};

		void Update() override;

		void SetTitle(const char* title) const;

		void SetViewPort();

		int ShouldClose() const;

		int GetKey(int key) const;

		void SwapBuffers() const;

		void Close() const;

		void Destroy() const;

		virtual ~Window();

	protected:
		GLFWwindow* _window;
		InputInterface _inputInterface;

		int _width;
		int _height;

	private:
		void Create();
	};
}

#endif // !OPENGL_WINDOW_INCLUDED