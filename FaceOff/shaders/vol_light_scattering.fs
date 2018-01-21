#version 330

// uniform vec3 u_color;

uniform vec2 u_lightPosition;	// position of sun on the screen
uniform sampler2D u_occlusionMapId;
uniform sampler2D u_sceneTextureId;

/*
const int NUM_SAMPLES = 200;
const float EXPOSURE = 0.015;
const float DECAY = 1.0;
const float DENSITY = 1.0;
const float WEIGHT = 1.00;
*/

const int NUM_SAMPLES = 128;
const float EXPOSURE = 0.0034;
const float DECAY = 1.0;
const float DENSITY = 0.84;
const float WEIGHT = 5.65;


in vec2 vf_UV;

out vec4 FragColor;

void main()
{


    float intensity = 0.25;
    float decay = 0.96875;

	vec2 texcoord = vf_UV;	 texcoord.y = 1.0 - vf_UV.y;

	vec2 currentTexCoord = texcoord;
	vec2 deltaTexCoord = u_lightPosition - texcoord;	
	deltaTexCoord /= NUM_SAMPLES;
	
	vec3 color = texture(u_occlusionMapId, currentTexCoord).xyz;

	for (int i = 0; i < NUM_SAMPLES; i++) 
	{
		color += texture(u_occlusionMapId, currentTexCoord).xyz * intensity;
		intensity *= decay;
		currentTexCoord += deltaTexCoord;
	}
	
	color += texture2D(u_sceneTextureId, texcoord).xyz;
	
	FragColor = vec4(color.xyz, 1.0);

/*

	vec2 texcoord = vf_UV;	 texcoord.y = 1.0 - vf_UV.y;

	vec2 deltaTexCoord= texcoord - u_lightPosition;
	vec2 currentTexCoord = texcoord;
	deltaTexCoord *= 1.0 / float(NUM_SAMPLES) * DENSITY;
	float illuminationDecay = 1.0;
	
	vec4 color = vec4(0);
	
	for (int i = 0; i < NUM_SAMPLES; i++) 
	{
		currentTexCoord -= deltaTexCoord;
		vec4 occlusionSample = texture2D(u_occlusionMapId, currentTexCoord);
		occlusionSample *= illuminationDecay * WEIGHT;
		color += occlusionSample;
		illuminationDecay *= DECAY;
	}
	
	color *= EXPOSURE;
	
	color += texture2D(u_sceneTextureId, texcoord);
	
	FragColor = clamp(color, vec4(0), vec4(1));
*/

//	FragColor = texture(u_occlusionMapId, texcoord);
//	FragColor = texture(u_sceneTextureId, texcoord);
}