#version 330



struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
    float scale;
};



uniform mat4 u_modelViewProjMat;
uniform mat4 u_modelViewMat;
uniform mat4 u_modelMat;
uniform vec3 u_eyePoint;
uniform mat4 u_lightModelViewProjMat;

const int MAX_POINT_LIGHTS = 100;

uniform DirectionalLight u_dirLight;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];


uniform sampler2D u_texture;
// uniform float u_matSpecularIntensity;
// uniform float u_specularPower;

const float matSpecularIntensity = 0.2;
const float specularPower = 1.0;


in vec3 vf_position;
in vec3 vf_normal;
in vec2 vf_UV;
out vec4 FragColor;


vec4 calcLightInternal(BaseLight light, vec3 lightDir, vec3 worldPos, vec3 normal)
{
	// ambient light
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;


	// diffuse light
	float diffuseFactor = dot(normal, -lightDir);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;


		// specular light
		vec3 eyeToWorldPos = normalize(u_eyePoint - worldPos);

		vec3 reflection = reflect(eyeToWorldPos, normal);

		reflection = normalize(reflection);

		float specularFactor = dot(reflection, lightDir);

		specularFactor = pow(specularFactor, specularPower);

		if(specularFactor > 0.0)
		{
			specularColor = vec4(light.color, 1.0) * matSpecularIntensity * specularFactor;
		}

	}

	return ambientColor + diffuseColor + specularColor;
}

vec4 calcDirectionalLight(vec3 worldPos, vec3 normal)
{
	return calcLightInternal(u_dirLight.base, u_dirLight.direction, worldPos, normal);
}


vec4 calcPointLight()
{
	return vec4(0.0, 0.0, 0.0, 1.0);
}


void main()
{
	vec2 tc = vf_UV;	 //	tc.y = 1.0 - vf_UV.y;
	vec4 rawColor = texture(u_texture, tc);

//	vec3 normal = normalize(vf_normal);

	vec4 dirLightColor = calcDirectionalLight(vf_position, vf_normal);

	vec4 ptLightColor = calcPointLight();

	vec4 finalLightColor = dirLightColor + ptLightColor;


	FragColor = rawColor * finalLightColor;
}