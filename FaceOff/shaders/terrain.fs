#version 330
uniform sampler2D u_texture;

in vec2 vf_UV;
out vec4 FragColor;

void main()
{

	vec2 tc = vf_UV;	 
	FragColor = texture(u_texture, tc);

}


