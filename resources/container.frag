#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 tex_coord;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
	FragColor = mix(
		texture(tex0, tex_coord),
		texture(tex1, tex_coord),
		0.2);
}