#include "Player.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace openGL
{
	glm::mat4 model;

	Player::Player(openGL::Game* game, openGL::Program* shaderProgram, std::string model)
		: Model(model), Entity(), _game(game), _program(shaderProgram)
	{
	}

	void Player::Update()
	{
		this->Entity::Update();

		if (_game->GetKey(GLFW_KEY_I)) { this->Move(0, 0, 0.5f); }
		if (_game->GetKey(GLFW_KEY_K)) { this->Move(0, 0, -0.5f); }
		if (_game->GetKey(GLFW_KEY_J)) { this->Move(0.5f, 0, 0); }
		if (_game->GetKey(GLFW_KEY_L)) { this->Move(-0.5f, 0, 0); }

		model = glm::translate(glm::mat4(), this->Position());
		model = glm::rotate(model, -30.0f, glm::vec3(0, 1, 0));
	}

	void Player::Draw() const
	{
		_program->SetUniformMat4fv("ModelMatrix", glm::value_ptr(model));
		this->Model::Draw(_program);
	}

	Player::~Player()
	{
	}
}
