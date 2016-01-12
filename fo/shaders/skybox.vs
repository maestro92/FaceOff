#version 330

layout (location = 0) in vec3 position;

uniform mat4 u_modelViewProjMat;
out vec3 vf_position; // the vector that points the vertices itself

void main()
{
	gl_Position = u_modelViewProjMat * vec4(position,1.0);
	vf_position = position;	// object space
}


