#pragma once

#include <vector>
#include <string>

#include <gl/GL.h>

#include <cassert>

namespace dlb {

	struct Texture2DConfiguration {
		GLuint wrap = GL_REPEAT;
		GLuint min_filtering = GL_LINEAR_MIPMAP_LINEAR;
		GLuint mag_filtering = GL_LINEAR;
		std::string path;
	};


	class Texture2DGroupBuilder;

	class Texture2DGroup {
	private:
		Texture2DGroup() {};

	protected:
		void setTextures(std::vector<GLuint>&& texs) {
			textures = std::move(texs);
		}

	public:

		const std::vector<GLuint>& getTextures() {
			return textures;
		}

		void use() {
			for (int i = 0; i < textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i]);
			}
		}

	private:
		std::vector<GLuint> textures;

		friend class Texture2DGroupBuilder;
	};

	class Texture2DGroupBuilder {
	public:
		Texture2DGroupBuilder() {
		}

		Texture2DGroupBuilder& configure_new() {
			assert(textures.size() < 16);
			current_tex = textures.size();
			textures.push_back({});
			return *this;
		}

		Texture2DGroupBuilder& wrap(GLuint value) {
			textures[current_tex].wrap = value;
			return *this;
		}

		Texture2DGroupBuilder& min_filtering(GLuint value) {
			textures[current_tex].min_filtering = value;
			return *this;
		}

		Texture2DGroupBuilder& path(std::string&& path) {
			textures[current_tex].path = std::move(path);
			return *this;
		}

		Texture2DGroup build();
	
	public:
		int current_tex = 0;
		std::vector<Texture2DConfiguration> textures;
	};
}