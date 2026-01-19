#include "ScriptingSystem.h"
#include "../ECS/Components/ScriptComponent.h"
#include "../ECS/Entity.h"
#include <Logger/Logger.h>

namespace SPARK_CORE::Systems {
	ScriptingSystem::ScriptingSystem(SPARK_CORE::ECS::Registry& registry)
		: m_Registry(registry), m_bMainLoaded{ false }
	{
	}

	bool ScriptingSystem::LoadMainScript(sol::state& lua)
	{
		try
		{
			auto result = lua.safe_script_file("./assets/scripts/main.lua");
		}
		catch (const sol::error& err)
		{
			auto e = err.what();
			SPARK_ERROR("Error loading the main lua script: {0}", e);
			return false;
		}

		sol::table main_lua = lua["main"];
		sol::optional<sol::table> bUpdateExists = main_lua[1];
		if (bUpdateExists == sol::nullopt)
		{
			SPARK_ERROR("There is no update function in main.lua");
			return false;
		}

		sol::table update_script = main_lua[1];
		sol::function update = update_script["update"];

		sol::optional<sol::table> bRenderExists = main_lua[2];
		if (bRenderExists == sol::nullopt)
		{
			SPARK_ERROR("There is no render function in main.lua");
			return false;
		}

		sol::table render_script = main_lua[2];
		sol::function render = render_script["render"];

		SPARK_CORE::ECS::Entity mainLuaScript{ m_Registry, "main_script", "" };
		mainLuaScript.AddComponent<SPARK_CORE::ECS::ScriptComponent>(
			SPARK_CORE::ECS::ScriptComponent{
				.update = update,
				.render = render
			}
		);

		m_bMainLoaded = true;
		return true;
	}

	void ScriptingSystem::Update()
	{
		if (!m_bMainLoaded)
		{
			SPARK_ERROR("Main lua script has not been loaded!");
			return;
		}

		auto view = m_Registry.GetRegistry().view<SPARK_CORE::ECS::ScriptComponent>();

		for (const auto& entity : view)
		{
			SPARK_CORE::ECS::Entity ent{ m_Registry, entity };
			if (ent.GetName() != "main_script")
				continue;

			auto& script = ent.GetComponent<SPARK_CORE::ECS::ScriptComponent>();
			auto error = script.update(script);
			if (!error.valid())
			{
				sol::error err = error;
				auto e = err.what();
				SPARK_ERROR("Error running the Update script: {0}", e);
			}
		}
	}

	void ScriptingSystem::Render()
	{
		if (!m_bMainLoaded)
		{
			SPARK_ERROR("Main lua script has not been loaded!");
			return;
		}

		auto view = m_Registry.GetRegistry().view<SPARK_CORE::ECS::ScriptComponent>();

		for (const auto& entity : view)
		{
			SPARK_CORE::ECS::Entity ent{ m_Registry, entity };
			if (ent.GetName() != "main_script")
				continue;

			auto& script = ent.GetComponent<SPARK_CORE::ECS::ScriptComponent>();
			auto error = script.render(script);
			if (!error.valid())
			{
				sol::error err = error;
				auto e = err.what();
				SPARK_ERROR("Error running the Render script: {0}", e);
			}
		}
	}
}