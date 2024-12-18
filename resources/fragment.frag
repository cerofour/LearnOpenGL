#version 330

out vec4 FragColor;

uniform vec3 variatingColor;

void main() {
	FragColor = vec4(variatingColor, 1);
}