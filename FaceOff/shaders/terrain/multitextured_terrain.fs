#version 330
unniform sampler2D u_textures[4];
uniform sampler2D u_blendMapTexture;


in vec2 vf_UV;
out vec4 FragColor;



void main()
{
	vec4 blendMapColor = texture(u_blendMapTexture, vf_UV);
	float texAmount0 = 1 - (blendMapColor.r + blendMapColor.g, blendMapColor.z);
	vec4 texColor0 = texture(u_textures[0], vf_UV) * texture0Amount;

	vec4 texColor1 = texture(u_textures[1], vf_UV) * blendMapColor.r;

	vec4 texColor2 = texture(u_textures[2], vf_UV) * blendMapColor.g;

	vec4 texColor3 = texture(u_textures[3], vf_UV) * blendMapColor.b;

//	vec2 tc = vf_UV;	 
	FragColor = texColor0 + texColor1 + texColor2 + texColor3;

}


