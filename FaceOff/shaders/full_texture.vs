#version 330

uniform mat4 u_modelViewProjMat;

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 UV;

out vec2 vf_UV;

const float density = 0.007;
const float gradient = 1.5;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position, 1.0);


	vf_UV = UV;
}



