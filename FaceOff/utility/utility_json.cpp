#include "utility.h"



mValue utl::readJsonFileToMap(char* filename)
{
	ifstream is(filename);

	mValue content;
	try
	{
		read_or_throw(is, content);
	}
	catch (Error_position errorPosition)
	{
		cout << "	error reading " << filename << endl;
		cout << "	line " << errorPosition.line_ << endl;
		cout << "	column " << errorPosition.column_ << endl;
		cout << "	" << errorPosition.reason_ << endl;
		while (true)
		{

		}
		exit(1);
	}
	return content;
}


Value utl::readJsonFileToVector(char* filename)
{
	ifstream is(filename);

	Value content;
	try
	{
		read_or_throw(is, content);
	}
	catch (Error_position errorPosition)
	{
		cout << "	error reading " << filename << endl;
		cout << "	line " << errorPosition.line_ << endl;
		cout << "	column " << errorPosition.column_ << endl;
		cout << "	" << errorPosition.reason_ << endl;
		while (true)
		{

		}
		exit(1);
	}
	return content;
}


const mValue& utl::findValue(const mObject& obj, const string& name)
{
	mObject::const_iterator it = obj.find(name);

	if (it == obj.end() || it->first != name)
	{
		cout << "error findingValue " << name << endl;
		//		assert(it != obj.end());
		//		assert(it->first == name);
		while (true)
		{

		}
		exit(1);
	}

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








