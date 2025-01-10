#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 frag_position;
in vec2 tex_coord;

void main() {
	FragColor = vec4(0.89, .71, .2, 1.0);
}