#include "Common.h"
#include "Clock.h"

namespace openGL
{
	Clock::Clock()
		: _started(false), _laziness(1), _startTime(0), _currentTime(0), _previousTime(0)
	{
	}

	void Clock::Start()
	{
		if (!_started)
		{
			_startTime = glfwGetTime();
			_currentTime = _startTime;
			_previousTime = _startTime;
			_started = true;
		}
	}

	void Clock::Stop()
	{
		_started = false;
		_previousTime = _currentTime;
	}

	void Clock::Reset()
	{
		_laziness = 1;
	}

	void Clock::Update()
	{
		if (_started)
		{
			_previousTime = _currentTime;
			_currentTime = glfwGetTime();
		}
	}

	double Clock::ElapsedTime() const
	{
		return _currentTime - _startTime;
	}

	double Clock::FrameTime() const
	{
		return (_currentTime - _previousTime) / _laziness;
	}

	double Clock::TimeFactor() const
	{
		return FrameTime() / 1000;
	}

	Clock::~Clock()
	{
	}
}