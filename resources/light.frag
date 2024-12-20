#version 330 core

out vec4 FragColor;

in vec3 color;
in vec3 normal;
in vec3 frag_position;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform vec3 u_eye_position;
uniform float u_ambient_strength;
uniform float u_specular_strength;
uniform float u_shininess;

void main() {

	vec3 ambient = u_light_color * u_ambient_strength;

	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(u_light_position - frag_position);

	float angle = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = u_light_color * angle;

	vec3 view_direction = normalize(u_eye_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), u_shininess);
	vec3 specular = u_specular_strength * spec * u_light_color;

	vec3 result = (ambient + diffuse + specular) * color;

	FragColor = vec4(result, 1.0f);
}