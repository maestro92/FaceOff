#version 330

uniform mat4 u_modelViewProjMat;

layout (location = 0) in vec3 position;

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position, 1.0);
}



