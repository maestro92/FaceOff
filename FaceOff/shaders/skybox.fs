#version 330

#extension GL_NV_shadow_samplers_cube : enable

uniform samplerCube u_cubeMapTextureID;

in vec3 vf_position;

out vec4 FragColor;
void main()
{
	FragColor = textureCube(u_cubeMapTextureID, vf_position);
	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);

}






