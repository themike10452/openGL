#ifndef OPENGL_CLOCK_INCLUDED
#define OPENGL_CLOCK_INCLUDED

#include "GLFW/glfw3.h"
#include "Interfaces.h"

namespace openGL
{
	struct Clock : public IUpdatable
	{
		explicit Clock()
			: _started(false), _laziness(1), _startTime(0), _currentTime(0), _previousTime(0)
		{
		}

		void Start()
		{
			if (!_started)
			{
				_startTime = glfwGetTime();
				_currentTime = _startTime;
				_previousTime = _startTime;
				_started = true;
			}
		}

		void Stop()
		{
			_started = false;
			_previousTime = _currentTime;
		}

		void Reset()
		{
			_laziness = 1;
		}

		void Update() override
		{
			if (_started)
			{
				_previousTime = _currentTime;
				_currentTime = glfwGetTime();
			}
		}

		double ElapsedTime() const
		{
			return _currentTime - _startTime;
		}

		double FrameTime() const
		{
			return (_currentTime - _previousTime) / _laziness;
		}

		double TimeFactor() const
		{
			return FrameTime() / 1000;
		}

		virtual ~Clock()
		{
		}

	protected:
		bool _started;
		double _startTime;
		double _currentTime;
		double _previousTime;
		GLuint _laziness;
	};
}

#endif //!OPENGL_CLOCK_INCLUDED