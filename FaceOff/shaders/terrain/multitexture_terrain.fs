#version 330
uniform sampler2D u_blendMapTexture;
// uniform sampler2D u_textures0;
// uniform sampler2D u_textures1;

uniform sampler2D u_textures[4];


in vec2 vf_UV;
out vec4 FragColor;


void main()
{

	//vec2 texCoord = vf_UV;

	vec4 blendMapColor = texture(u_blendMapTexture, vf_UV);

	float texAmount0 = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.z);

	vec2 texCoord = vf_UV *160.0;

	vec4 texColor0 = texture(u_textures[0], texCoord) * texAmount0;

	vec4 texColor1 = texture(u_textures[1], texCoord) * blendMapColor.r;

	vec4 texColor2 = texture(u_textures[2], texCoord) * blendMapColor.g;

	vec4 texColor3 = texture(u_textures[3], texCoord) * blendMapColor.b;


//	vec2 tc = vf_UV;	 
	FragColor = texColor0 + texColor1 + texColor2 + texColor3;

//	FragColor = texColor0;

//	FragColor = texColor1;
//	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


