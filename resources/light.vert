#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//vec4 test = projection * view * model * vec4(1.0f
	gl_Position = projection * view * model * vec4(a_pos, 1);
	color = a_color;
}