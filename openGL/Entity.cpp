#include "Entity.h"

namespace openGL
{
	/// Getters
	unsigned int Entity::Mass() const { return _mass; }
	const glm::vec3& Entity::Position() const { return _position; }
	/// -------

	Entity::Entity()
		: _position(), _mass(0)
	{
	}

	void Entity::Move(glm::vec3& translate, bool correctMovement)
	{
		this->Move(translate.x, translate.y, translate.z, correctMovement);
	}

	void Entity::Move(float x, float y, float z, bool correctMovement)
	{
		if (correctMovement)
		{
			_position += x * _right;
			_position += y * _up;
			_position += z * _direction;
		}
		else
		{
			this->SetPosition(_position.x + x, _position.y + y, _position.z + z);
		}
	}

	void Entity::SetPosition(glm::vec3& newPosition)
	{
		this->SetPosition(newPosition.x, newPosition.y, newPosition.z);
	}

	void Entity::SetPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	void Entity::SetPositionX(float x)
	{
		_position.x = x;
	}

	void Entity::SetPositionY(float y)
	{
		_position.y = y;
	}

	void Entity::SetPositionZ(float z)
	{
		_position.z = z;
	}

	Entity::~Entity()
	{
	}
}