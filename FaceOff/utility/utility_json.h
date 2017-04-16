#ifndef UTILITY_JSON_H_
#define UTILITY_JSON_H_

#include "define.h"

#include <fstream>
#include "json_spirit.h"
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"

#ifndef JSON_SPIRIT_MVALUE_ENABLED
#error Please define JSON_SPIRIT_MVALUE_ENABLED for the mValue type to be enabled 
#endif

using namespace std;
using namespace json_spirit;

namespace utl
{
	/// utl_json.cpp
	mValue readJsonFileToMap(char* file);

	Value readJsonFileToVector(char* file);

	const mValue& findValue(const mObject& obj, const std::string& name);

	glm::vec3 findVec3(const mObject& obj, const std::string& name);
}



#endif
