#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include <cassert>

namespace dlb {
	enum Texture2DType {
		Whatever = 0,
		Specular,
		Diffuse
	};

	struct Texture2D {
		GLuint id;
		Texture2DType type;
	};

	struct Texture2DConfiguration {
		GLuint wrap = GL_REPEAT;
		GLuint min_filtering = GL_LINEAR_MIPMAP_LINEAR;
		GLuint mag_filtering = GL_LINEAR;
		Texture2DType type;
		std::string path;
	};

	class Texture2DGroupBuilder;

	class Texture2DGroup {
	private:
		Texture2DGroup() {};


	public:
		Texture2DGroup(Texture2DGroup&& rhs) = default;

		~Texture2DGroup() {
			// drawback of having an array of texture2D instead of an array of ints
			// maybe refactor this later so all textures can be deleted in one call.
			/*
			for (int i = 0; i < textures.size(); i++) {
				glDeleteTextures(1, &textures[i].id);
			}
			*/
		}

	protected:
		void setTextures(std::vector<Texture2D>&& texs) {
			textures = std::move(texs);
		}

	public:
		const std::vector<Texture2D>& getTextures() {
			return textures;
		}

		void use() {
			for (int i = 0; i < textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
		}

	private:
		std::vector<Texture2D> textures;

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

		Texture2DGroupBuilder& configuration(Texture2DConfiguration&& conf) {
			textures[current_tex] = std::move(conf);
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

		Texture2DGroupBuilder& type(Texture2DType ty) {
			textures[current_tex].type = ty;
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

	class Texture2DPool {
	private:
		Texture2DPool() {}

	public:
		static Texture2DPool& getInstance() {
			static Texture2DPool pool{};
			return pool;
		};

	public:
		bool exists(const std::string& path) {
			return texture_pool_.find(path) != texture_pool_.end();
		}

		Texture2D& reuse(const std::string& path) {
			assert(exists(path));
			return texture_pool_.at(path);
		}
	
	protected:
		friend class Texture2DGroupBuilder;

		void insert(const std::string& path, Texture2D& tex) {
			assert(!exists(path));
			texture_pool_.insert({ path, tex });
		}

	private:
		std::unordered_map<std::string, Texture2D> texture_pool_;
	};
}