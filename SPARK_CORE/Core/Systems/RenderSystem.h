#pragma once
#include "../ECS/Registry.h"
#include <Rendering/Core/BatchRenderer.h>

namespace SPARK_CORE::Systems {
	class RenderSystem
	{
	private:
		SPARK_CORE::ECS::Registry& m_Registry;
		std::unique_ptr<SPARK_RENDERING::BatchRenderer> m_pBatchRenderer;

	public:
		RenderSystem(SPARK_CORE::ECS::Registry& registry);
		~RenderSystem() = default;

		void Update();
	};
}