#ifndef OPENGL_POINTLIGHT_INCLUDED
#define OPENGL_POINTLIGHT_INCLUDED

#include "Light.h"
#include "Entity.h"

namespace openGL
{
	struct PointLight : Light, Entity
	{
		explicit PointLight(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 position = glm::vec3(), float attenuation = 0.0f)
			: Light(color, attenuation)
		{
			this->Entity::SetPosition(position);
		}
	};
}

#endif //!OPENGL_POINTLIGHT_INCLUDED