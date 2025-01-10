#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 frag_position;
in vec2 tex_coord;

struct Material {
	sampler2D texture_diffuse0;
};

uniform Material u_material;

void main() {
	FragColor = texture(u_material.texture_diffuse0, tex_coord);
	//FragColor = vec4(normal, 1.0f);
}