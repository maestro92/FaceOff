#ifndef BASE_LIGHT_H_
#define BASE_LIGHT_H_

#include "utility.h"

///////////////////////////
struct BaseLight
{
	BaseLight()
	{
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		ambientIntensity = 0.0f;
		diffuseIntensity = 0.0f;
	}

	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};


///////////////////////////
struct DirectionalLight : public BaseLight
{
	glm::vec3 direction;
	DirectionalLight()
	{
		direction = glm::vec3(1.0f, -1.0f, 0.0f);
	}
};



///////////////////////////
struct Attenuation
{
	float constant;
	float linear;
	float exp;
};


struct PointLight : public BaseLight
{
	glm::vec3 position;
	Attenuation attenuation;
	PointLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		attenuation.constant = 0.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}

	PointLight(glm::vec3 colorIn,
		float ambientIntensityIn,
		float diffuseIntensityIn,
		glm::vec3 positionIn,
		float constantIn, float linearIn, float expIn)
	{
		color = colorIn;
		ambientIntensity = ambientIntensityIn;
		diffuseIntensity = diffuseIntensityIn;
		position = positionIn;
		attenuation.constant = constantIn;
		attenuation.linear = linearIn;
		attenuation.exp = expIn;
	}
};



///////////////////////////
struct SpotLight : public BaseLight
{
	glm::vec3 direction;
	float cutoff;
	glm::vec3 position;

	Attenuation attenuation;

	SpotLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		cutoff = 0.0f;
		attenuation.constant = 0.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}
};


#endif