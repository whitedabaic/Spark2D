#pragma once
#include "../ECS/Registry.h"

namespace SPARK_CORE::Systems {
	class AnimationSystem
	{
	private:
		SPARK_CORE::ECS::Registry& m_Registry;

	public:
		AnimationSystem(SPARK_CORE::ECS::Registry& registry);
		~AnimationSystem() = default;

		void Update();
	};
}