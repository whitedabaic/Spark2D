#pragma once
#include <glm/glm.hpp>

namespace SPARK_CORE::ECS
{
	struct TransformComponent
	{
		glm::vec2 position{ glm::vec2{0.f} }, scale{ glm::vec2{1.} };
		float rotation{ 0.f };
	};
}