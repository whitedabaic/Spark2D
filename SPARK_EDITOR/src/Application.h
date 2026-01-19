#pragma once
#include <glad/glad.h>
#include <Windowing/Window/Window.h>
#include <Core/ECS/Registry.h>

namespace SPARK_EDITOR {
	class Application
	{
	private:
		std::unique_ptr<SPARK_WINDOWING::Window> m_pWindow;
		std::unique_ptr<SPARK_CORE::ECS::Registry> m_pRegistry;

		SDL_Event m_Event;
		bool m_bIsRunning;

	private:
		bool Initialize();
		bool LoadShaders();

		void ProcessEvents();
		void Update();
		void Render();

		void ClearUp();

		Application();
	public:
		static Application& GetInstance();
		~Application();

		void Run();
	};
}