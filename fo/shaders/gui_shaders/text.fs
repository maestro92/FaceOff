#version 330

uniform sampler2D u_texture;
uniform vec3 u_color;

in vec2 vf_UV;
out vec4 FragColor;

void main()
{
	vec2 tc = vf_UV;
	FragColor = vec4(u_color, texture(u_texture, tc).r);
}
