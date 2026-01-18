#define SDL_MAIN_HANDLED 1
#define NOMINMAX
#include <Windowing/Window/Window.h>
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
#include <entt.hpp>

struct UVs
{
	float u{ 0.f }, v{ 0.f }, uv_width{ 0.f }, uv_height{ 0.f };
};

struct TransformComponent
{
	glm::vec2 position{ glm::vec2{0.f} }, scale{ glm::vec2{1.} };
	float rotation{ 0.f };
};

struct SpriteComponent
{
	float width{ 0.f }, height{ 0.f };
	UVs uvs{ .u = 0.f, .v = 0.f, .uv_width = 0.f, .uv_height = 0.f };

	SPARK_RENDERING::Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };
	int start_x{ 0 }, start_y{ 0 };

	void generate_uvs(int textureWidth, int textureHeight)
	{
		uvs.uv_width = width / textureWidth;
		uvs.uv_height = height / textureHeight;

		uvs.u = start_x * uvs.uv_width;
		uvs.v = start_y * uvs.uv_height;
	}
};

int main() 
{
	SPARK_INIT_LOGS(true, true)
	bool running{ true };

	// Inint SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to initialize SDL: " << error << std::endl;
		running = false;
		return -1;

	}

	//Set up OpenGL
	if (SDL_GL_LoadLibrary(NULL) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to load OpenGL library: " << error << std::endl;
		running = false;
		return -1;
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
	SPARK_WINDOWING::Window window("Spark Editor", 640, 480, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

	if (!window.GetSDLWindow())
	{
		std::cout << "Failed to create window." << std::endl;
		return -1;
	}

	// Create OpenGL context
	window.SetGLContext(SDL_GL_CreateContext(window.GetSDLWindow().get()));

	if (!window.GetGLContext())
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to create OpenGL context: " << error << std::endl;
		running = false;
		return -1;
	}

	SDL_GL_MakeCurrent(window.GetSDLWindow().get(), window.GetGLContext());
	SDL_GL_SetSwapInterval(1);

	// Initialize GLAD
	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

		return -1;
	}

	// ENable Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create a registry
	auto pRegistry = std::make_unique<entt::registry>();
	if (!pRegistry)
	{
		SPARK_ERROR("Failed to create the entt registry");
		return -1;
	}

	// Add temp texture
	auto texture = SPARK_RENDERING::TextureLoader::Create(
		SPARK_RENDERING::Texture::TextureType::PIXEL,
		"./assets/textures/castle.png"
	);

	if (!texture)
	{
		SPARK_ERROR("Failed to create the texture!");
		return -1;
	}

	// Let's make some temporary UVs
	UVs uVs{};
	int width = texture->GetWidth();
	int height = texture->GetHeight();
	SPARK_LOG("Loaded Texture: [width = {0}, height = {1}]", width, height);
	SPARK_WARN("Loaded Texture: [width = {0}, height = {1}]", width, height);

	// Create a new entity -- for test
	auto ent1 = pRegistry->create();

	auto& transform = pRegistry->emplace<TransformComponent>(ent1, TransformComponent{
		.position = glm::vec2{10.f, 10.f},
		.scale = glm::vec2{1.f, 1.f},
		.rotation = 0.f
		}
	);

	auto& sprite = pRegistry->emplace<SpriteComponent>(ent1, SpriteComponent{
		.width = 16.f,
		.height = 16.f,
		.color = SPARK_RENDERING::Color{.r = 0, .g = 255, .b = 0, .a = 255 },
		.start_x = 0,
		.start_y = 28,
		}
	);

	sprite.generate_uvs(texture->GetWidth(), texture->GetHeight());

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

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Create a temp camera
	SPARK_RENDERING::Camera2D camera{};
	camera.SetScale(15.f);

	// Create out first shader
	auto shader = SPARK_RENDERING::ShaderLoader::Create(
		"assets/shaders/basicShader.vert",
		"assets/shaders/basicShader.frag"
	);

	if (!shader)
	{
		std::cout << "Failed to create the shader!" << std::endl;
		return -1;
	}

	// Now we will have to create the vertex array object and vertex buffer object
	GLuint VAO, VBO, IBO;

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

	SDL_Event event{};

	// Window loop
	while (running)
	{
		// Process events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
					}
					break;
				default:
					break;
			}
		}

		glViewport(
			0,
			0,
			window.GetWidth(),
			window.GetHeight()
		);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader->Enable();
		glBindVertexArray(VAO);

		auto projection = camera.GetCameraMatrix();

		shader->SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		SDL_GL_SwapWindow(window.GetSDLWindow().get());

		camera.Update();
		shader->Disable();
	}

	std::cout << "Exiting Application..." << std::endl;
	return 0;
}