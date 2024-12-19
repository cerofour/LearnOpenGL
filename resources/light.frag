#version 330 core

out vec4 FragColor;

in vec3 color;

uniform vec3 u_light_color;

void main() {
	FragColor = vec4(u_light_color * color, 1.0f);
}