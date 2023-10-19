#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normals;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normals1;

void main()
{    
	FragColor = texture(texture_diffuse1, TexCoords);
}