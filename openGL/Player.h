#ifndef OPENGL_PLAYER_INCLUDED
#define OPENGL_PLAYER_INCLUDED

#include "Model.h"
#include "Entity.h"
#include "Game.h"

namespace openGL
{
	struct Player : Model, Entity
	{
		explicit Player(openGL::Game* game, openGL::Program* shaderProgram, std::string model);

		void Update();
		void Draw() const;

		virtual ~Player();

	protected:
		openGL::Game* _game;
		openGL::Program* _program;
	};
}

#endif //!OPENGL_PLAYER_INCLUDED