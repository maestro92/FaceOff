#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 3) in vec2 UV;

uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelMat;

out vec3 vf_position;
out vec3 vf_normal;
out vec2 vf_UV;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position, 1.0);

	vf_position = vec3(u_modelMat * vec4(position, 1.0));
	vf_normal = normalize(vec3(u_modelMat * vec4(normal, 0.0)));
//	vf_normal = vec3(u_modelMat * vec4(normal, 0.0));
	vf_UV = UV;
}



