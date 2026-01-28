#pragma once
#include <sol/sol.hpp>
#include <Rendering/Essentials/Vertex.h>

namespace SPARK_CORE::ECS {
	struct TextComponent
	{
		std::string sFontName{ "pixel" }, sTextStr{ "" };
		int padding{ 0 };
		float wrap{ -1.f };
		SPARK_RENDERING::Color color{ 255, 255, 255, 255 };
		bool bHidden{ false };

		static void CreateLuaTextBindings(sol::state& lua);
	};
}