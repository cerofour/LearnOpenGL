#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec3 normal;
out vec3 color;
out vec3 frag_position;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//vec4 test = projection * view * model * vec4(1.0f
	gl_Position = projection * view * model * vec4(a_pos, 1.0f);
	normal = a_normal;
	color = vec3(0.25, 0.52, 0.96);
	frag_position = vec3(model * vec4(a_pos, 1));
	tex_coords = a_tex_coords;
}