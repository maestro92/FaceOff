#version 330

layout (location = 0) in vec3 	position;
layout (location = 1) in vec3 	normal;
layout (location = 2) in vec3 	color;
layout (location = 3) in vec2 	UV;
layout (location = 4) in ivec4 	boneIDs;
layout (location = 5) in vec4 	weights;


uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelMat;
uniform mat4 u_lightViewProjMat;


const int MAX_BONES = 100;
uniform mat4 u_bones[MAX_BONES];
 
out vec3 vf_position;
out vec3 vf_normal;
out vec2 vf_UV;
out vec4 vf_lightSpacePos;




void main()
{

    mat4 boneTransform = u_bones[boneIDs[0]] * weights[0];
    boneTransform     += u_bones[boneIDs[1]] * weights[1];
    boneTransform     += u_bones[boneIDs[2]] * weights[2];
    boneTransform     += u_bones[boneIDs[3]] * weights[3];
  
	vec4 newPos = boneTransform * vec4(position, 1.0);
	gl_Position = u_modelViewProjMat * newPos;


	vf_position = vec3(u_modelMat * newPos);
	vf_normal = vec3(u_modelMat * (boneTransform * vec4(normal, 0.0)));
	vf_normal = normalize(vf_normal);
	vf_UV = UV;

	vf_lightSpacePos = u_lightViewProjMat * u_modelMat * vec4(position, 1.0);
}
