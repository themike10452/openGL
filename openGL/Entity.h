#ifndef OPENGL_ENTITY_INCLUDED
#define OPENGL_ENTITY_INCLUDED

#include "Common.h"

namespace openGL
{
	struct Entity
	{
		/// Getters
		unsigned int Mass() const;
		const glm::vec3& Position() const;
		/// -------

		Entity();

		virtual void Update();

		virtual void UpdateDirection();

		virtual void Move(glm::vec3& translate, bool correctMovement = true);

		virtual void Move(float x, float y, float z, bool correctMovement = true);

		void SetPosition(glm::vec3& newPosition);

		void SetPosition(float x, float y, float z);

		void SetPositionX(float x);

		void SetPositionY(float y);

		void SetPositionZ(float z);

		virtual ~Entity();

	protected:
		glm::vec3 _position, _direction, _right, _up;
		float _pitch, _yaw;
		unsigned int _mass;
	};
}

#endif //!OPENGL_ENTITY_INCLUDED