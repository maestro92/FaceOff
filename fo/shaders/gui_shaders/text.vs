#version 330

/*
in vec3 vertex;
in vec3 normal;
in vec3 tangent;
in vec3 color;
in vec2 UV;
*/
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 UV;


uniform mat4 u_modelViewProjMat;
out vec2 vf_UV;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position,1.0);
	vf_UV = UV;
}
