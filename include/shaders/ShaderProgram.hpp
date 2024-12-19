#pragma once


#include <string>
#include <vector>

#include <gl/GL.h>

namespace dlb {
	class ShaderProgramBuilder {
	public:

		struct Shader {

			/*
			* Path to the shader file, will be read and compiled
			*/
			std::string path;
			GLenum type;
			GLuint shaderId;
		};

		ShaderProgramBuilder()
			:shaders{}
		{}

		ShaderProgramBuilder& fragmentShader(const std::string& path) {
			shaders.push_back({ path, GL_FRAGMENT_SHADER });
			return *this;
		}

		ShaderProgramBuilder& vertexShader(const std::string& path) {
			shaders.push_back({ path, GL_VERTEX_SHADER });
			return *this;
		}

		int build();
		void checkCompileErrors(int current_shader);

	private:
		std::vector<Shader> shaders;
		int shaderProgram = -1;
	};
};