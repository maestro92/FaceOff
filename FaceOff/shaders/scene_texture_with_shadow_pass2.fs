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
uniform sampler2D u_shadowMap;
uniform vec2 u_shadowMapSize;
// uniform float u_matSpecularIntensity;
// uniform float u_specularPower;

const float matSpecularIntensity = 1.0;
const float specularPower = 16;


in vec3 vf_position;
in vec3 vf_normal;
in vec2 vf_UV;
in vec4 vf_lightSpacePos;
out vec4 FragColor;

#define blinn 1
#define PCF 1
#define EPSILON 0.00001
                          

float calcShadowFactor(vec4 shadowMapCoord)
{
 //   vec3 ProjCoords = lightSpacePos.xyz / lightSpacePos.w;
 //   vec2 UVCoords;
 //   UVCoords.x = 0.5 * ProjCoords.x + 0.5;
 //   UVCoords.y = 0.5 * ProjCoords.y + 0.5;
 //   float z = 0.5 * ProjCoords.z + 0.5;

    float xOffset = 1.0/u_shadowMapSize.x;
    float yOffset = 1.0/u_shadowMapSize.y;

    float shadowFactor = 0.0;

    for (float y = -1.5 ; y <= 1.5 ; y++) 
    {
        for (float x = -1.5 ; x <= 1.5 ; x++) 
        {
            vec2 offsets = vec2(x * xOffset, y * yOffset);
            vec3 uvc = vec3(shadowMapCoord.xy + offsets, shadowMapCoord.z + EPSILON);

        //    Factor += texture(u_shadowMap, uvc.xy).r;
        	if( texture( u_shadowMap, uvc.xy  ).r <= shadowMapCoord.z )
        	{
        		shadowFactor += 0.5;
        	}
        }
    }

    return (shadowFactor / 16.0);
}


// this assumes lightDir is normalized
vec4 calcLightInternal(BaseLight light, vec3 lightDir, vec3 worldPos, vec3 normal)
{
	// ambient light
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;

	// diffuse light
	float diffuseFactor = max(dot(normal, -lightDir), 0.0);
	vec4 diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

	// specular light
#if blinn==1
	// Phong specular model
	vec3 viewDir = normalize(u_eyePoint - worldPos);
	vec3 reflection = reflect(lightDir, normal);

	reflection = normalize(reflection);

	float specularFactor = max(dot(reflection, viewDir), 0.0);
	specularFactor = pow(specularFactor, specularPower);
	vec4 specularColor = vec4(light.color, 1.0) * matSpecularIntensity * specularFactor;
#else	
	// Blinn-Phong Specular Model
	vec3 viewDir = normalize(u_eyePoint - worldPos);
	vec3 halfwayDir = -normalize(lightDir - viewDir);	// if you are unsure about where all these negative signs come from, just draw a graph
	
	float specularFactor = max(dot(normal, halfwayDir), 0.0);
	specularFactor = pow(specularFactor, specularPower);
	vec4 specularColor = vec4(light.color, 1.0) * matSpecularIntensity * specularFactor;
#endif	

	// shadow
    vec4 shadowMapCoord = vf_lightSpacePos;
    shadowMapCoord /= shadowMapCoord.w;

    // used to lower morie pattern and self-shadowing
//    shadowCoord.z -= 0.00000005;


#if PCF == 1
	// the more you are in shadow, the less light (diffuse + specular) u get
    float shadowFactor = calcShadowFactor(shadowMapCoord);
	return ambientColor + (1 - shadowFactor) * (diffuseColor + specularColor);    
#else
	// the more you are in shadow, the less light (diffuse + specular) u get
    float shadowFactor = 0.0;

    // if the value on the map is less than current depth. then it's in shadow.
    if ( texture( u_shadowMap, shadowMapCoord.xy ).r <= shadowMapCoord.z)
    	shadowFactor = 0.5;

	return ambientColor + (1 - shadowFactor) * (diffuseColor + specularColor);    	
#endif




}

vec4 calcDirectionalLight(vec3 worldPos, vec3 normal)
{
	// u_dirLight.direction is already noramlized
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

	vec4 color = rawColor * finalLightColor;
//	FragColor = rawColor * finalLightColor;

	FragColor = rawColor * finalLightColor;
//	FragColor = (shadowValue == 0.0) ? vec4(0.1,0.1,0.1,1.0) * color : color;

}