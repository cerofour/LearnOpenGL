#version 330 core

layout (location = 0) in vec3 p;

void main() {
	vec3 new_pos = vec3(p.x * -1, p.y * -1, p.z * -1);
	gl_Position = vec4(new_pos, 1);
}