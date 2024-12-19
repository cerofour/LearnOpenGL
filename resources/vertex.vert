#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//vec4 test = projection * view * model * vec4(1.0f
	gl_Position = projection * view * model * vec4(aPos, 1);
	position = aPos;
}