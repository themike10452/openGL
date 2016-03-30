#ifndef OPENGL_INTERFACES_INCLUDED
#define OPENGL_INTERFACES_INCLUDED

namespace openGL
{
	struct IComponent
	{
		virtual void Initialize() = 0;
	};

	struct IUpdatable : IComponent
	{
		virtual void Update() = 0;
		void Initialize() override {}
	};
}

#endif //!OPENGL_INTERFACES_INCLUDED