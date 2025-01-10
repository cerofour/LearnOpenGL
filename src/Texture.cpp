#include "Application.hpp"

#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <iostream>
#include <cassert>


namespace dlb {
	Texture2DGroup Texture2DGroupBuilder::build() {
		Texture2DGroup tex_group{};

		std::vector<Texture2D> texs( textures.size() );

		assert(texs.size() == textures.size() && "This should not fail");

		//glGenTextures(texs.size()), (GLuint*)(texs.data()));

		auto& context = ApplicationSingleton::getInstance();

		for (int i = 0; i < textures.size(); i++) {

			std::cout << "Loading: " << textures[i].path;

			if (context.getTexture2DPool()->exists(textures[i].path)) {
				
				std::cout << "[REUSED]" << std::endl;

				texs.push_back(context.getTexture2DPool()->reuse(textures[i].path));
				continue;
			}

			glGenTextures(1, (GLuint*)&texs[i].id);

			context.getTexture2DPool()->insert(textures[i].path, texs[i]);

			// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			glBindTexture(GL_TEXTURE_2D, texs[i].id);
			// set the texture wrapping parameters
			// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textures[i].wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textures[i].wrap);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textures[i].min_filtering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textures[i].mag_filtering);

			int width, height, nr_channels;

			bool is_png = textures[i].path.ends_with("png");

			unsigned char* data = stbi_load(textures[i].path.c_str(), &width, &height, &nr_channels, is_png ? STBI_rgb_alpha : 0);

			GLenum format = GL_RED;
			if (nr_channels == 1)
				format = GL_RED;
			else if (nr_channels == 3)
				format = GL_RGB;
			else if (nr_channels == 4)
				format = GL_RGBA;

			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format , GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "Failed to load texture: " << textures[i].path << std::endl;
			}
			stbi_image_free(data);
		}

		tex_group.setTextures(std::move(texs));

		return tex_group;
	}
}