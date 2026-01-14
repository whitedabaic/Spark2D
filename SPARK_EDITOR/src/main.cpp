#define SDL_MAIN_HANDLED 1;
#include <Windowing/Window/Window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

int main() 
{
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
	SPARK_WINDOWING::Window window("SPARK Editor", 1280, 720, SDL_WINDOWPOS_CENTERED,
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

	// Create temp vertex data
	//float vertices[] = {
	//	 0.0f,  0.5f, 0.0f,
	//	 -0.5f, -0.5f, 0.0f,
	//	 0.5f, -0.5f, 0.0f,
	//};

	// Create vertices for a Quad
	float vertices[] =
	{
		 0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 -0.5f, 0.5f, 0.0f,
		 -0.5f, -0.5f, 0.0f,
	};

	GLuint indices[] =
	{
		0, 1, 3,
		2, 3, 0
	};
	// Create a temp vertex source
	const char* vertexSource = R"(
		#version 460 core
		layout(location = 0) in vec3 aPosition;
		void main()
		{
			gl_Position = vec4(aPosition, 1.0);
		}
	)";

	// Create the shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Add the vertex shader source
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	// Get the compilation status
	int status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader\n" << infoLog << std::endl;
		return -1;
	}

	// Create a temp fragment shader
	const char* fragmentSource = R"(
		#version 460 core
		out vec4 color;
		void main()
		{
			color = vec4(1.0f, 0.0f, 1.0, 1.0);
		}
	)";

	// Create the shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Add the fragment shader source
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	// Get the compilation status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader\n" << infoLog << std::endl;
		return -1;
	}

	// Create the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach the above shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Now we want to link the shader program
	glLinkProgram(shaderProgram);

	// Check the link status
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (!status)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program!\n" << infoLog << std::endl;
		return -1;
	}

	// Now we can enable the shader program
	glUseProgram(shaderProgram);

	// Once the program is created and linked, we can delete the shaders
	glDeleteBuffers(1, &vertexShader);
	glDeleteBuffers(1, &fragmentShader);

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
		3 * sizeof(float),	// 步长
		(void*)0			// 偏移量
	);

	glEnableVertexAttribArray(0);

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

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		SDL_GL_SwapWindow(window.GetSDLWindow().get());
	}

	std::cout << "Exiting Application..." << std::endl;
	return 0;
}