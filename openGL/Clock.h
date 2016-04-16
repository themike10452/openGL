#ifndef OPENGL_CLOCK_INCLUDED
#define OPENGL_CLOCK_INCLUDED

#include "Interfaces.h"

namespace openGL
{
	struct Clock : public IUpdatable
	{
		Clock();

		void Start();

		void Stop();

		void Reset();

		void Update() override;

		double ElapsedTime() const;

		double FrameTime() const;

		double TimeFactor() const;

		virtual ~Clock();

	protected:
		bool _started;
		double _startTime;
		double _currentTime;
		double _previousTime;
		unsigned int _laziness;
	};
}

#endif //!OPENGL_CLOCK_INCLUDED