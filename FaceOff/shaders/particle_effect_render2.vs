#version 330

layout (location = 0) in vec3 position;

uniform float u_time;
uniform float u_fadeRate;

out vec3 vg_position;
out vec3 vg_velocity;
out float vg_alpha;

void main()
{
	gl_Position = vec4(position, 1.0);
}