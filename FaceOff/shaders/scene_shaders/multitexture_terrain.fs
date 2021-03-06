#version 330


struct BaseLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};


struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation atten;
    float scale;
};



// pipeline related
uniform vec3 u_eyePoint;


// object related
uniform sampler2D u_blendMapTexture;
uniform sampler2D u_textures[4];

uniform float u_matSpecularIntensity;
uniform float u_specularPower;


// all the light uniforms
const int MAX_POINT_LIGHTS = 103;  
uniform DirectionalLight u_dirLight;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

in vec3 vf_position;
in vec3 vf_normal;
in vec3 vf_color;
in vec2 vf_UV;

out vec4 FragColor;



vec4 computeBaseLight(BaseLight light, 
					  vec3 lightDirection, 
					  vec3 worldPos, 
					  vec3 normal)
{
	// ambient contribution
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;

	// diffuse contribution
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(light.color, 1.0) * diffuseFactor * light.diffuseIntensity;

		vec3 eyeToVertex = normalize(u_eyePoint - worldPos);

		vec3 reflection = normalize(reflect(eyeToVertex, normal));

		float specularFactor = dot(reflection, lightDirection);

		specularFactor = pow(specularFactor, u_specularPower);

		if(specularFactor > 0.0)
			specularColor = vec4(light.color, 1.0) * specularFactor * u_matSpecularIntensity;
	}

	return (ambientColor + diffuseColor + specularColor);
}



vec4 computeDirLight(vec3 worldPos, vec3 normal)
{
	return computeBaseLight(u_dirLight.base,
							u_dirLight.direction,
							worldPos, 
							normal);
}


void main()
{
	// compute object natural color
	vec4 blendMapColor = texture(u_blendMapTexture, vf_UV);
	float texAmount0 = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.z);
	vec2 texCoord = vf_UV * 160.0;

	vec4 texColor0 = texture(u_textures[0], texCoord) * texAmount0;
	vec4 texColor1 = texture(u_textures[1], texCoord) * blendMapColor.r;
	vec4 texColor2 = texture(u_textures[2], texCoord) * blendMapColor.g;
	vec4 texColor3 = texture(u_textures[3], texCoord) * blendMapColor.b;


	// compute light color
	vec3 normal = normalize(vf_normal);
	vec4 objectColor = texColor0 + texColor1 + texColor2 + texColor3;


	vec4 dirLightColor = objectColor * computeDirLight(vf_position, normal);

	FragColor = dirLightColor;
//	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	FragColor = vec4(normal, 1.0);
}


