#version 330

uniform sampler2D u_texture;

in vec2 vf_UV;
out vec4 FragColor;

void main()
{
	// if it's a depth texture
	vec2 tc = vf_UV;	tc.y = 1.0 - tc.y;

	float power = 4096;
	vec4 value = texture2D(u_texture, tc);
	value.x = pow(value.x,power);
	value.y = pow(value.y,power);
	value.z = pow(value.z,power);
	FragColor = value;
}


