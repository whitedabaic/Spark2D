#pragma once
#include <sol/sol.hpp>

namespace SPARK_CORE::Scripting {
	struct GLMBindings
	{
		static void CreateGLMBinds(sol::state& lua);
	};
}