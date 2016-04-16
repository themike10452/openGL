#ifndef OPENGL_PLAYER_INCLUDED
#define OPENGL_PLAYER_INCLUDED

#include "Model.h"
#include "Entity.h"

namespace openGL
{
	struct Player : Model, Entity
	{
		explicit Player(std::string model)
			: Model(model), Entity()
		{
		}

		virtual ~Player()
		{
		}
	};
}

#endif //!OPENGL_PLAYER_INCLUDED