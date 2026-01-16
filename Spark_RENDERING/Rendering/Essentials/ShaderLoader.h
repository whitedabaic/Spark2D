#pragma once
#include "Shader.h"
#include <memory>

namespace SPARK_RENDERING
{
	class ShaderLoader 
	{
	private:
		static GLuint CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
		static GLuint CompileShader(GLuint shaderType, const std::string& filepath);
		static bool CompileSuccess(GLuint shader);
		static bool IsProgramValid(GLuint program);
		static bool LinkShaderProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader);

	public:
		ShaderLoader() = delete;
		static std::shared_ptr<Shader> Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	};
}