#ifndef OPENGL_LIGHT_INCLUDED
#define OPENGL_LIGHT_INCLUDED

#include "glm/glm.hpp"

namespace openGL
{
	struct Light
	{
		const glm::vec3& Position() const { return _position; }
		const glm::vec3& Color() const { return _color; }
		const float& Attenuation() const { return _attenuation; }

		explicit Light(glm::vec3 color = glm::vec3(), glm::vec3 position = glm::vec3(), float attenuation = 0)
			: _position(position), _color(color), _attenuation(attenuation)
		{
		}

		void SetPosition(glm::vec3 position)
		{
			_position.x = position.x;
			_position.y = position.y;
			_position.z = position.z;
		}

		void SetColor(glm::vec3 color)
		{
			_color.x = color.x;
			_color.y = color.y;
			_color.z = color.z;
		}

		void SetAttenuation(float attenuation)
		{
			_attenuation -= _attenuation;
			_attenuation += attenuation;
		}

		virtual ~Light()
		{
		}

	protected:
		glm::vec3 _position;
		glm::vec3 _color;
		float _attenuation;
	};
}

#endif //!OPENGL_LIGHT_INCLUDED