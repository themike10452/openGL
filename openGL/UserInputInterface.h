#ifndef OPENGL_USERINPUTINTERFACE_INCLUDED
#define OPENGL_USERINPUTINTERFACE_INCLUDED

#include "Common.h"

namespace openGL
{
	struct InputInterface
	{
		static std::vector<void*> InputListeners;

		template<class Game>
		static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			for (void* listener : InputListeners)
			{
				((Game*)listener)->OnKeyDown(key, action);
			}
		}

		template<class Game>
		static void OnMouseDown(GLFWwindow* window, int button, int action, int mods)
		{
			for (void* listener : InputListeners)
			{
				((Game*)listener)->OnMouseDown(button, action);
			}
		}

		template<class Game>
		static void OnMouseMove(GLFWwindow* window, double x, double y)
		{
			for (void* listener : InputListeners)
			{
				((Game*)listener)->OnMouseMove(x, y);
			}
		}
	};
}

#endif //!OPENGL_USERINPUTINTERFACE_INCLUDED