#include <glad/glad.h>
#include <gl/gl.h>

#include <iostream>

#include "ShaderProgram.hpp"

#include "Renderable.hpp"

namespace dlb {
	Renderable::Renderable(bool _use_ebo) {
		vertices_no = 0;
		use_ebo = _use_ebo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);

		if (use_ebo)
			glGenBuffers(1, &ebo);

		shader_program = nullptr;
	}

	Renderable::~Renderable() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void Renderable::configureVertexAttributes(GLenum buffer_type, GLenum usage, const std::function<void(void)>& configure) {
		glBindVertexArray(vao);
		glBindBuffer(buffer_type, vbo);

		if (use_ebo) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			assert(indices.size() >= 0);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), usage);
		}

		glBufferData(
			buffer_type,
			sizeof(vertices[0]) * vertices.size(),
			vertices.data(),
			usage);

		configure();
	}

	void Renderable::render(const std::function<void(const dlb::ShaderProgram *, dlb::Texture2DGroup*)> & pre_render) {
		if (do_render) {
			assert(shader_program != nullptr && "Shader Program is not defined");

			shader_program->use();

			glBindVertexArray(vao);

			pre_render(shader_program, textures);

			if (!use_ebo)
				glDrawArrays(GL_TRIANGLES, 0, vertices_no);
			else {
				std::cout << "Rendering container" << indices.size() << std::endl;
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}