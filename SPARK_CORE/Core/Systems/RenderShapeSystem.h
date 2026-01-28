#pragma once
#include "../ECS/Registry.h"
#include <Rendering/Core/RectBatchRenderer.h>
#include <Rendering/Core/CircleBatchRenderer.h>

namespace SPARK_CORE::Systems {
	class RenderShapeSystem
	{
	private:
		SPARK_CORE::ECS::Registry& m_Registry;
		std::unique_ptr<SPARK_RENDERING::RectBatchRenderer> m_pRectRenderer;
		std::unique_ptr<SPARK_RENDERING::CircleBatchRenderer> m_pCircleRenderer;

	public:
		RenderShapeSystem(SPARK_CORE::ECS::Registry& registry);
		~RenderShapeSystem() = default;

		void Update();
	};
}