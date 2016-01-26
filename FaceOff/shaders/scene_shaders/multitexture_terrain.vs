#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 UV;


uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelMat;

out vec3 vf_position;
out vec3 vf_normal;
out vec3 vf_color;
out vec2 vf_UV;


void main()
{
	gl_Position = u_modelViewProjMat * vec4(position,1.0);

	vf_position = (u_modelMat * vec4(position, 1.0)).xyz;
	vf_normal = (u_modelMat * vec4(normal, 0.0)).xyz;
	vf_color = color;
	vf_UV = UV;
}

