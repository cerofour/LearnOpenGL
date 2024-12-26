#include <glad/glad.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <iostream>

#include "Texture.hpp"

namespace dlb {
	Texture2DGroup Texture2DGroupBuilder::build() {
		Texture2DGroup tex_group{};

		std::vector<GLuint> texs( textures.size(), 0 );
		glGenTextures(static_cast<GLsizei>(texs.size()), (GLuint*)(texs.data()));

		for (int i = 0; i < textures.size(); i++) {
			// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			glBindTexture(GL_TEXTURE_2D, texs[i]);
			// set the texture wrapping parameters
			// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textures[i].wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textures[i].wrap);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textures[i].min_filtering);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textures[i].mag_filtering);

			int width, height, nrChannels;
			unsigned char* data = stbi_load(textures[i].path.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
					textures[i].path.ends_with("png") ? GL_RGBA : GL_RGB , GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
		}

		tex_group.setTextures(std::move(texs));

		return tex_group;
	}
}