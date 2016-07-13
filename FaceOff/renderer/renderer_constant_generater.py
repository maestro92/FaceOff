# pip install simplejson

import json
from collections import OrderedDict
from pprint import pprint

def process_renderer_name(renderer):
	s = ""

	for i in range(0, len(renderer)):

		if(i < 3):
			s+=renderer[i]
	
		else:
			if(renderer[i].isupper()):
				s+= "_"

			s+=renderer[i]

	s = s.upper()
	return s


def write_renderer_enum(file, renderer, enum_dict):
	s = "\n\nnamespace ";
	
	s = s + process_renderer_name(renderer)

	s = s + "\n{\n"
	s = s + "	enum E\n	{\n"
	file.write(s)


	write_enums(file, enum_dict)

	s = "\n	};"

	s = s + "\n}"

	file.write(s)


def write_enums(file, enum_dict):
	s = "	";
	indent = "		"
	i = 0
	for enum in enum_dict:

		if(i == 0):
	
			if( len(enum_dict) == 1):
				s = indent + enum + " = 0"
			else:
				s = indent + enum + " = 0,\n"

		elif (i==len(enum_dict) - 1):
			s = indent + enum

		else:
			s = indent + enum + ",\n"


		i+=1
		file.write(s)


# if you see 
#	ValueError: No JSON object could be decoded
# check for ending ","

input_filename = "renderer_data.json"
gui_input_filename = "../gui/gui_renderer_data.json"

output_filename = "renderer_constants.h"
gui_output_filename = "../gui/gui_renderer_constants.h"

output_file = open(output_filename, 'w')
gui_output_file = open(gui_output_filename, 'w')


# header
header = """#ifndef RENDERER_CONSTANT_H_
#define RENDERER_CONSTANT_H_

// http://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e
"""
output_file.write(header)


header = """#ifndef GUI_RENDERER_CONSTANT_H_
#define GUI_RENDERER_CONSTANT_H_

// http://stackoverflow.com/questions/23288934/c-how-to-have-same-enum-members-name-in-different-enum-names-without-getting-e
"""
gui_output_file.write(header)




# data
with open(input_filename) as data_file:    
    data = json.load(data_file, object_pairs_hook = OrderedDict)

with open(gui_input_filename) as gui_data_file:
	gui_data = json.load(gui_data_file, object_pairs_hook = OrderedDict)

for entry in data:
	if("data" in entry):
		renderer = entry["r"]

		enum_dict = entry["data"]
	
		print (str(renderer))
		write_renderer_enum(output_file, renderer, enum_dict)



for entry in gui_data:
	if("data" in entry):
		renderer = entry["r"]

		enum_dict = entry["data"]
	
		print (str(renderer))
		write_renderer_enum(gui_output_file, renderer, enum_dict)





# footer
footer = "\n\n#endif"


output_file.write(footer)
gui_output_file.write(footer)