#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 frag_position;
in vec2 tex_coord;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	//sampler2D emission;
	float shininess;
};

struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

uniform DirectionalLight u_light;
uniform Material u_material;
uniform vec3 u_eye_position;
uniform float u_time;

void main() {

	vec3 ambient = u_light.ambient * texture(u_material.diffuse, tex_coord).rgb;

	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(-u_light.direction);

	float angle = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = u_light.diffuse * (angle * texture(u_material.diffuse, tex_coord).rgb);

	vec3 view_direction = normalize(u_eye_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), u_material.shininess);
	vec3 specular_texture = texture(u_material.specular, tex_coord).rgb;
	vec3 specular = u_light.specular * (spec * specular_texture);

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}