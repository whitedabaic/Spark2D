#pragma once
#include "../ECS/Registry.h"

namespace SPARK_CORE::Systems {
	class PhysicsSystem
	{
	private:
		SPARK_CORE::ECS::Registry& m_Registry;

	public:
		PhysicsSystem(SPARK_CORE::ECS::Registry& registry);
		~PhysicsSystem() = default;

		void Update(entt::registry& registry);
	};
}