#include "Application.h"
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Vertex.h>
#include <Rendering/Core/Camera2D.h>
#include <Logger/Logger.h>
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/Identification.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/Resources/AssetManager.h>

#include <Core/Systems/ScriptingSystem.h>

namespace SPARK_EDITOR {
	bool Application::Initialize()
	{
		SPARK_INIT_LOGS(true, true)

		// Inint SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::string error = SDL_GetError();
			SPARK_ERROR( "Failed to initialize SDL: {0}", error);
			return false;

		}

		//Set up OpenGL
		if (SDL_GL_LoadLibrary(NULL) != 0)
		{
			std::string error = SDL_GetError();
			SPARK_ERROR( "Failed to load OpenGL library: {0}", error);
			return false;
		}

		// Set the OpenGL attributes
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Set the number of bits per channel
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// Create a window
		m_pWindow = std::make_unique<SPARK_WINDOWING::Window>(
			"Spark Editor", 
			640, 480, 
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			true, SDL_WINDOW_OPENGL);

		if (!m_pWindow->GetSDLWindow())
		{
			SPARK_ERROR("Failed to create window.");
			return false;
		}

		// Create OpenGL context
		m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetSDLWindow().get()));

		if (!m_pWindow->GetGLContext())
		{
			std::string error = SDL_GetError();
			SPARK_ERROR("Failed to create OpenGL context: {0}", error);
			return false;
		}

		SDL_GL_MakeCurrent(m_pWindow->GetSDLWindow().get(), m_pWindow->GetGLContext());
		SDL_GL_SetSwapInterval(1);

		// Initialize GLAD
		if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
		{
			SPARK_ERROR("Failed to initialize GLAD");
			return false;
		}

		// ENable Alpha Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto assetManager = std::make_shared<SPARK_RESOURCES::AssetManager>();
		if (!assetManager)
		{
			SPARK_ERROR("Failed to create the Asset Manager!");
			return false;
		}

		if (!assetManager->AddTexture("castle", "./assets/textures/castle.png", true))
		{
			SPARK_ERROR("Failed to load texture into Asset Manager!");
			return false;
		}

		// Add temp texture
		auto texture = assetManager->GetTexture("castle");

		// Let's make some temporary UVs
		int width = texture.GetWidth();
		int height = texture.GetHeight();
		SPARK_LOG("Loaded Texture: [width = {0}, height = {1}]", width, height);
		SPARK_WARN("Loaded Texture: [width = {0}, height = {1}]", width, height);

		m_pRegistry = std::make_unique<SPARK_CORE::ECS::Registry>();

		SPARK_CORE::ECS::Entity entity1{ *m_pRegistry, "Ent1", "Test" };

		auto& transform = entity1.AddComponent<SPARK_CORE::ECS::TransformComponent>(SPARK_CORE::ECS::TransformComponent{
			.position = glm::vec2{10.f, 10.f},
			.scale = glm::vec2{1.f, 1.f},
			.rotation = 0.f
			}
		);

		auto& sprite = entity1.AddComponent<SPARK_CORE::ECS::SpriteComponent>(SPARK_CORE::ECS::SpriteComponent{
			.width = 16.f,
			.height = 16.f,
			.color = SPARK_RENDERING::Color{.r = 0, .g = 255, .b = 0, .a = 255 },
			.start_x = 0,
			.start_y = 28,
			}
			);

		sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());

		std::vector<SPARK_RENDERING::Vertex> vertices{};
		SPARK_RENDERING::Vertex vTL{}, vTR{}, vBL{}, vBR{};

		vTL.position = glm::vec2{ transform.position.x, transform.position.y + sprite.height };
		vTL.uvs = glm::vec2{ sprite.uvs.u, sprite.uvs.v + sprite.uvs.uv_height };

		vTR.position = glm::vec2{ transform.position.x + sprite.width, transform.position.y + sprite.height };
		vTR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width, sprite.uvs.v + sprite.uvs.uv_height };

		vBL.position = glm::vec2{ transform.position.x, transform.position.y };
		vBL.uvs = glm::vec2{ sprite.uvs.u, sprite.uvs.v };

		vBR.position = glm::vec2{ transform.position.x + sprite.width, transform.position.y };
		vBR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width, sprite.uvs.v };

		vertices.push_back(vTL);
		vertices.push_back(vBL);
		vertices.push_back(vBR);
		vertices.push_back(vTR);

		auto& id = entity1.GetComponent<SPARK_CORE::ECS::Identification>();

		SPARK_LOG("Name: {0}, Group: {1}, ID: {2}", id.name, id.group, id.entity_id);

		GLuint indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		// Create the lua state
		auto lua = std::make_shared<sol::state>();

		if (!lua)
		{
			SPARK_ERROR("Failed to create the lua state!");
			return false;
		}

		lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);

		if (!m_pRegistry->AddToContext<std::shared_ptr<sol::state>>(lua))
		{
			SPARK_ERROR("Failed to add the sol::state to the registry context");
			return false;
		}

		auto scriptSystem = std::make_shared<SPARK_CORE::Systems::ScriptingSystem>(*m_pRegistry);
		if (!scriptSystem)
		{
			SPARK_ERROR("Failed to create the script system!");
			return false;
		}

		if (!scriptSystem->LoadMainScript(*lua))
		{
			SPARK_ERROR("Failed to load the main lua script!");
			return false;
		}

		if (!m_pRegistry->AddToContext<std::shared_ptr<SPARK_CORE::Systems::ScriptingSystem>>(scriptSystem))
		{
			SPARK_ERROR("Failed to add the script system to the registry context!");
			return false;
		}

		// Create a temp camera
		auto camera = std::make_shared<SPARK_RENDERING::Camera2D>();
		camera->SetScale(15.f);

		if (!m_pRegistry->AddToContext<std::shared_ptr<SPARK_RESOURCES::AssetManager>>(assetManager))
		{
			SPARK_ERROR("Failed to add the asset manager to the registry context!");
			return false;
		}

		if (!m_pRegistry->AddToContext<std::shared_ptr<SPARK_RENDERING::Camera2D>>(camera))
		{
			SPARK_ERROR("Failed to add the camera to the registry context!");
			return false;
		}

		if (!LoadShaders())
		{
			SPARK_ERROR("Failed to load the shaders!");
			return false;
		}

		// Let's generate VAO
		glGenVertexArrays(1, &VAO);

		// Generate VBO
		glGenBuffers(1, &VBO);

		// Bind the VAO and VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(
			GL_ARRAY_BUFFER,									// 目标缓冲区类型
			vertices.size() * sizeof(SPARK_RENDERING::Vertex),	// 缓冲区数据大小
			vertices.data(),									// 缓冲区数据来源
			GL_STATIC_DRAW										// 缓冲区使用模式
		);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,				// 目标缓冲区类型
			6 * sizeof(GLuint),						// 缓冲区数据大小
			indices,								// 缓冲区数据来源
			GL_STATIC_DRAW							// 缓冲区使用模式
		);

		glVertexAttribPointer(
			0,															// 属性位置
			2,															// 属性大小
			GL_FLOAT,													// 属性类型
			GL_FALSE,													// 是否标准化
			sizeof(SPARK_RENDERING::Vertex),							// 步长
			(void*)offsetof(SPARK_RENDERING::Vertex, position)			// 偏移量
		);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,														// 属性位置
			2,														// 属性大小
			GL_FLOAT,												// 属性类型
			GL_FALSE,												// 是否标准化
			sizeof(SPARK_RENDERING::Vertex),						// 步长
			(void*)offsetof(SPARK_RENDERING::Vertex, uvs)			// 偏移量
		);

		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			2,														// 属性位置
			4,														// 属性大小
			GL_UNSIGNED_BYTE,										// 属性类型
			GL_TRUE,												// 是否标准化
			sizeof(SPARK_RENDERING::Vertex),						// 步长
			(void*)offsetof(SPARK_RENDERING::Vertex, color)			// 偏移量
		);

		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		return true;
	}

	bool Application::LoadShaders()
	{
		auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<SPARK_RESOURCES::AssetManager>>();

		if (!assetManager)
		{
			SPARK_ERROR("Failed to get the asset manager from the registry context");
			return false;
		}

		if (!assetManager->AddShader("basic", "assets/shaders/basicShader.vert", "assets/shaders/basicShader.frag"))
		{
			SPARK_ERROR("Failed to add the shader to the asset manager");
			return false;
		}

		return true;
	}

	void Application::ProcessEvents()
	{
		// Process events
		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
			case SDL_QUIT:
				m_bIsRunning = false;
				break;
			case SDL_KEYDOWN:
				if (m_Event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_bIsRunning = false;
				}
				break;
			default:
				break;
			}
		}
	}

	void Application::Update()
	{
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<SPARK_RENDERING::Camera2D>>();
		if (!camera)
		{
			SPARK_ERROR("Failed to get the camera from the registry context!");
			return;
		}

		camera->Update();

		auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<SPARK_CORE::Systems::ScriptingSystem>>();
		scriptSystem->Update();
	}

	void Application::Render()
	{
		auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<SPARK_RESOURCES::AssetManager>>();
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<SPARK_RENDERING::Camera2D>>();

		auto& shader = assetManager->GetShader("basic");
		auto projection = camera->GetCameraMatrix();

		if (shader.ShaderProgramID() == 0)
		{
			SPARK_ERROR("SHader program has not be created correctly!");
			return;
		}

		glViewport(
			0, 0,
			m_pWindow->GetWidth(),
			m_pWindow->GetHeight()
		);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.Enable();
		glBindVertexArray(VAO);

		shader.SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE0);
		const auto& texture = assetManager->GetTexture("castle");
		glBindTexture(GL_TEXTURE_2D, texture.GetID());

		auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<SPARK_CORE::Systems::ScriptingSystem>>();
		scriptSystem->Render();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		SDL_GL_SwapWindow(m_pWindow->GetSDLWindow().get());

		shader.Disable();
	}

	void Application::ClearUp()
	{
		SDL_Quit();
	}

	Application::Application()
		: m_pWindow{ nullptr }, m_pRegistry{ nullptr }, m_Event{}, m_bIsRunning{ true }
		, VAO{ 0 }, VBO{ 0 }, IBO{ 0 }
	{
	}

	Application& Application::GetInstance()
	{
		static Application app{};
		return app;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		if (!Initialize())
		{
			SPARK_ERROR("Initialization Failed!");
			return;
		}

		while (m_bIsRunning)
		{
			ProcessEvents();
			Update();
			Render();
		}

		ClearUp();
	}
}