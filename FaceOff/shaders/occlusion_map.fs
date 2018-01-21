#version 330

in vec3 vf_color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(vf_color, 1.0);
}