#include "utility.h"


string utl::intToStr(int value)
{
	stringstream ss;
	ss << value; // add number to string
	return ss.str();
}

string utl::floatToStr(float value)
{
    ostringstream buff;
    buff << std::setprecision(4) << value;
    return buff.str();
}


int utl::randInt(int min, int max)
{
	float result = randFloat((float)min, (float)max);
	return (int)(floor(result));
}


float utl::randFloat(float min, float max)
{
    float num = (float)rand() / (float) RAND_MAX;
    return min + (max - min) * num;
}

float utl::barycentricInterpolation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


bool utl::chance(float percent)
{
	const float value = rand() / (float)RAND_MAX * 100;
	return value <= percent;
}

glm::vec3 utl::interpolateEntityPosition(glm::vec3 pos0, glm::vec3 pos1, float interpFactor)
{
	glm::vec3 pos;
	pos = pos0 + (pos1 - pos0) * interpFactor;
	return pos;
}