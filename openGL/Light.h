#ifndef OPENGL_LIGHT_INCLUDED
#define OPENGL_LIGHT_INCLUDED

#include "Common.h"

namespace openGL
{
	struct Light
	{
		const glm::vec3& Color() const { return _color; }
		float Attenuation() const { return _attenuation; }

		explicit Light(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float attenuation = 0.0f)
			: _color(color), _attenuation(attenuation)
		{
		}

		void SetColor(glm::vec3 color)
		{
			_color.x = color.x;
			_color.y = color.y;
			_color.z = color.z;
		}

		void SetColor(float r, float g, float b)
		{
			_color.x = r;
			_color.y = g;
			_color.z = b;
		}

		virtual ~Light()
		{
		}

	protected:
		glm::vec3 _color;
		float _attenuation;
	};
}

#endif //!OPENGL_LIGHT_INCLUDED