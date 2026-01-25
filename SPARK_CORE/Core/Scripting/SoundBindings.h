#pragma once
#include <sol/sol.hpp>

namespace SPARK_CORE { namespace ECS { class Registry; } }

namespace SPARK_CORE::Scripting {
	struct SoundBinder
	{
		static void CreateSoundBind(sol::state& lua, SPARK_CORE::ECS::Registry& registry);
	};
}