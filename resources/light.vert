#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 normal;
out vec3 frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//vec4 test = projection * view * model * vec4(1.0f
	gl_Position = projection * view * model * vec4(a_pos, 1.0f);
	normal = a_normal;
	frag_position = vec3(model * vec4(a_pos, 1));
}