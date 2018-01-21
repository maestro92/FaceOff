#version 330

uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelViewMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat4 u_modelMat;
uniform mat3 u_normalMat;


layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;

out vec3 vf_color;

void main()
{
	vf_color = color;
	gl_Position = u_modelViewProjMat * vec4(position, 1.0);
}



