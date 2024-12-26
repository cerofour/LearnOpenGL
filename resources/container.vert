#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_tex_coord;

out vec3 color;
out vec2 tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	//vec4 test = projection * view * model * vec4(1.0f
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);

	color = vec3(a_color);
	tex_coord = a_tex_coord;
}