#version 330

out vec4 FragColor;
in vec3 position;

void main() {

	FragColor = vec4(position * 2.0f + 1.0f, 1.0f);
}