#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 frag_position;
in vec2 tex_coord;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_eye_position;
uniform float u_time;

void main() {

	vec3 ambient = u_light.ambient * texture(u_material.diffuse, tex_coord).rgb;

	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(u_light.position - frag_position);

	float angle = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = u_light.diffuse * (angle * texture(u_material.diffuse, tex_coord).rgb);

	vec3 view_direction = normalize(u_eye_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), u_material.shininess);
	vec3 specular_texture = texture(u_material.specular, tex_coord).rgb;
	vec3 specular = u_light.specular * (spec * specular_texture);


	vec3 emission_texture = texture(u_material.emission, tex_coord).rgb;
	vec3 emission = vec3(0.0F);

	if (specular_texture.r == 0 && specular_texture.g == 0 && specular_texture.b == 0) {
		emission = texture(u_material.emission, tex_coord + vec2(0.0f, u_time)).rgb;
		emission = emission * (sin(u_time) + 1.0f);
	}

	vec3 result = ambient + diffuse + specular + emission;

	FragColor = vec4(result, 1.0f);
}