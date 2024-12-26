#include <vector>
#include <functional>

#include <gl/GL.h>

#include "ShaderProgram.hpp"
#include "Material.hpp"
#include "Texture.hpp"

namespace dlb {

	/*
	* Represents a renderable object.
	* This class contains the VAO and VBO necessary to send data
	* to the GPU.
	*/
	class Renderable {
	public:
		Renderable(bool _use_ebo = false);
		~Renderable();

	public:
		void feedData(std::vector<float>&& verts, int verts_no) {
			vertices = std::move(verts);
			vertices_no = verts_no;
		}

		void setShaderProgram(const dlb::ShaderProgram* sp) {
			shader_program = sp;
		}

		void setMaterial(const dlb::Material& mat) {
			material = mat;
		}

		void setUseMaterial(bool x) {
			use_material = x;
		}

		void useEBO(bool x) {
			use_ebo = x;
		}

		void setIndices(std::vector<GLuint>&& idx) {
			indices = std::move(idx);
		}

		void setTextures(Texture2DGroup* textures) {
			this->textures = textures;
		}

	public:

		/*
		* Configures the VBO.
		* @param configuration Expects a function that calls `glVertexAttribPointer`
		*	to configure the attributes of the VBO. A typical `configuration` function
		*	should be a lambda with:
		*	```cpp
		*		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), NULL);
		*		glEnableVertexAttribArray(0);
		*		// configure more attributes...
		*	```
		*/
		void configureVertexAttributes(GLenum target, GLenum usage, const std::function<void(void)>& configuration);

		void render(const std::function<void(const dlb::ShaderProgram*, dlb::Texture2DGroup*)>& pre_render);

	private:
		GLuint vao, vbo, ebo;

		bool do_render = true;
		bool use_material = true;
		bool use_ebo = false;

		int vertices_no;
		std::vector<float> vertices;
		const dlb::ShaderProgram* shader_program;
		std::vector<GLuint> indices;

		/*
		* Default material value
		*/
		dlb::Material material{
			glm::vec3(1.0F),
			glm::vec3(1.0F),
			glm::vec3(1.0F),
			1.0F,
		};

		dlb::Texture2DGroup* textures = nullptr;
	};
}