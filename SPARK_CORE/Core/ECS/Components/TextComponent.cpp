#include "TextComponent.h"
#include <entt.hpp>

void SPARK_CORE::ECS::TextComponent::CreateLuaTextBindings(sol::state& lua)
{
	lua.new_usertype<TextComponent>(
		"TextComponent",
		"type_id", &entt::type_hash<TextComponent>::value,
		sol::call_constructor,
		sol::factories(
			[](const std::string& sFontName, const std::string& sTextStr, SPARK_RENDERING::Color color, int padding, float wrap) {
				return TextComponent{
					.sFontName = sFontName,
					.sTextStr = sTextStr,
					.padding = padding,
					.wrap = wrap,
					.color = color,
				};
			}
		),
		"textStr", &TextComponent::sTextStr,
		"fontName", &TextComponent::sFontName,
		"padding", &TextComponent::padding,
		"wrap", &TextComponent::wrap,
		"hidden", &TextComponent::bHidden,
		"color", &TextComponent::color
	);
}
