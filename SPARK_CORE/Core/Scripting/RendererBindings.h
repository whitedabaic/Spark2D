#pragma once
#include <sol/sol.hpp>

namespace SPARK_CORE { namespace ECS { class Registry; } }

namespace SPARK_CORE::Scripting {
	struct RendererBinder
	{
		static void CreateRenderingBind(sol::state& lua, SPARK_CORE::ECS::Registry& registry);
	};
}