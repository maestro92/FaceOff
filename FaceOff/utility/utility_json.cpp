#include "utility.h"




const mValue& utl::findValue(const mObject& obj, const string& name)
{
	mObject::const_iterator it = obj.find(name);

	assert(it != obj.end());
	assert(it->first == name);

	return it->second;
}


glm::vec3 utl::findVec3(const mObject& obj, const string& name)
{
	const mObject vec3Obj = findValue(obj, name).get_obj();

	float x = findValue(vec3Obj, "x").get_real();
	float y = findValue(vec3Obj, "y").get_real();
	float z = findValue(vec3Obj, "z").get_real();

	glm::vec3 vec3Value(x, y, z);

	return vec3Value;
}

