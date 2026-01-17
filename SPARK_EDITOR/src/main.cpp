#define SDL_MAIN_HANDLED 1;
#include <Windowing/Window/Window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>

#include <Logger/Logger.h>
class Camera2D
{
private:
	int m_Width, m_Height;
	float m_Scate;

	glm::vec2 m_Position;
	glm::mat4 m_CameraMatrix, m_OrthoProjection;

	bool m_bNeedsUpdate;

public:
	Camera2D()
		:Camera2D{ 640, 480 }
	{

	}
	Camera2D(int width, int height)
		: m_Width{ width }, m_Height{ height }, m_Scate{ 1.f }, m_Position{ glm::vec2{0} },
		m_CameraMatrix{ 1.f }, m_OrthoProjection{ 1.f }, m_bNeedsUpdate{ true }
	{
		// Init orthographic projection
		m_OrthoProjection = glm::ortho(
			0.f,							// Left
			static_cast<float>(m_Width),	// Right
			static_cast<float>(m_Height),	// Top
			0.f,							// Bottom
			-1.f,							// Near
			1.f								// Far
		);
	}

	inline void SetScale(float scale)
	{
		m_Scate = scale;
		m_bNeedsUpdate = true;
	}

	inline glm::mat4 GetCameraMatrix() { return m_CameraMatrix; }

	void Update()
	{
		if(!m_bNeedsUpdate)
			return;

		// Translate
		glm::vec3 translate{ -m_Position.x, -m_Position.y, 0.f };
		m_CameraMatrix = glm::translate(m_OrthoProjection, translate);

		// Scale
		glm::vec3 scale{ m_Scate, m_Scate, 0.f };
		m_CameraMatrix *= glm::scale(glm::mat4{ 1.f }, scale);

		m_bNeedsUpdate = false;
	}

};
struct UVs
{
	float u, v, width, height;
	UVs()
		: u{ 0.f }, v{ 0.f }, width{ 0.f }, height{ 0.f }
	{

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
	UVs uvs{};
	int width = texture->GetWidth();
	int height = texture->GetHeight();
	SPARK_LOG("Loaded Texture: [width = {0}, height = {1}]", width, height);
	SPARK_WARN("Loaded Texture: [width = {0}, height = {1}]", width, height);
	auto generateUVs = [&](float startX, float startY, float spriteWidth, float spriteHeight)
	{
		uvs.width = spriteWidth / width;
		uvs.height = spriteHeight / height;

		uvs.u = startX / width;
		uvs.v = startY / height;
	};

	generateUVs(20, 28, 16, 16);
	// Create vertices for a Quad
	//float vertices[] =
	//{
	//	 0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	//	 0.5f,  -0.5f, 0.0f, 1.0f, 1.0f,
	//	 -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
	//	 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
	//};

	// Swapped tex coords
	float vertices[] =
	{
		 10.f,  26.f,  0.0f, uvs.u, (uvs.v + uvs.height),
		  10.f,  10.f,  0.0f, uvs.u, uvs.v,
		  26.f, 10.f,  0.0f, (uvs.u + uvs.width), uvs.v,
		 26.f, 26.f,  0.0f, (uvs.u + uvs.width), (uvs.v + uvs.height)
	};

	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// Create a temp camera
	Camera2D camera{};
	camera.SetScale(5.f);

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
		GL_ARRAY_BUFFER,						// 目标缓冲区类型
		sizeof(vertices) * 3 * sizeof(float),	// 缓冲区数据大小
		vertices,								// 缓冲区数据来源
		GL_STATIC_DRAW							// 缓冲区使用模式
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
		0,					// 属性位置
		3,					// 属性大小
		GL_FLOAT,			// 属性类型
		GL_FALSE,			// 是否标准化
		5 * sizeof(float),	// 步长
		(void*)0			// 偏移量
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,									// 属性位置
		2,									// 属性大小
		GL_FLOAT,							// 属性类型
		GL_FALSE,							// 是否标准化
		5 * sizeof(float),					// 步长
		reinterpret_cast<void*>(3 * sizeof(float))			// 偏移量
	);

	glEnableVertexAttribArray(1);

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