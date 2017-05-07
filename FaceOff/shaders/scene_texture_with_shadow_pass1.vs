#version 330

layout (location = 0) in vec3 position;

uniform mat4 u_modelViewProjMat;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position, 1.0);
}



