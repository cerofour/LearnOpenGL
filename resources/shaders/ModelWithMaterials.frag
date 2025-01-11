#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 frag_position;
in vec2 tex_coord;

struct Material {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};

struct DirectionalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

struct LightPoint {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

/*
struct SpotLight {
	vec3 direction;
	vec3 position;
	float cutoff;
};
*/

#define LIGHT_BULBS 4

uniform DirectionalLight u_light;
//uniform LightPoint u_light_points[LIGHT_BULBS];
uniform Material u_material;
uniform vec3 u_eye_position;

vec3 calculateDirectionalLight(DirectionalLight l, vec3 norm, vec3 view_dir) {

	vec3 light_direction = normalize(-l.direction);
	float angle = max(dot(norm, light_direction), 0.0f);

	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_dir, reflect_direction), 0.0f), u_material.shininess);

	vec3 ambient = l.ambient * u_material.ambient;
	vec3 diffuse = l.diffuse * (angle * u_material.diffuse);
	vec3 specular = l.specular * (spec * u_material.specular);

	vec3 result = ambient + diffuse + specular;
	
	return result;
}

/*
vec3 calculatePointLight(LightPoint l, vec3 norm, vec3 frag_pos, vec3 view_dir) {
	vec3 light_direction = normalize(l.position - frag_pos);

	float angle = max(dot(norm, light_direction), 0.0f);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_dir, reflect_direction), 0.0f), u_material.shininess);

	float dist = length(l.position - frag_pos);
	float attenuation = 1.0f / (l.constant + l.linear * dist + l.quadratic * dist * dist);

	vec3 ambient  = l.ambient * texture(u_material.texture_diffuse0, tex_coord).rgb;
	vec3 diffuse  = l.diffuse * (angle * texture(u_material.texture_diffuse0, tex_coord).rgb);
	vec3 specular = l.specular * (spec * texture(u_material.texture_specular0, tex_coord).rgb);

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	return result;
}
*/

void main() {
    // properties
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(u_eye_position - frag_position);

    // phase 1: Directional lighting
    vec3 result = calculateDirectionalLight(u_light, norm, view_dir);
    // phase 2: Point lights
    //for(int i = 0; i < LIGHT_BULBS; i++)
        //result += calculatePointLight(u_light_points[i], norm, frag_position, view_dir);

	FragColor = vec4(result, 1.0f);
}