# pip install simplejson

import json
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


def write_renderer_enum(file, renderer, enum_list):
	s = "\n\nenum ";
	
	s = s + process_renderer_name(renderer)

	s = s + "\n{\n"
	file.write(s)


	write_enums(file, enum_list)

	s = "\n};"

	file.write(s)


def write_enums(file, enum_list):
	s = "	";
	for i in range(0, len(enum_list)):
		if(i == 0):
	
			if( len(enum_list) == 1):
				s = "	" + enum_list[i] + " = 0"
			else:
				s = "	" + enum_list[i] + " = 0,\n"

		elif (i==len(enum_list) - 1):
			s = "	" + enum_list[i]

		else:
			s = "	" + enum_list[i] + ",\n"

		file.write(s)

	# file.write(s)

# if you see 
#	ValueError: No JSON object could be decoded
# check for ending ","

input_filename = "renderer_data.json"
output_filename = "renderer_constants.h"



output_file = open(output_filename, 'w')

# header
header = """#ifndef RENDERER_CONSTANT_H_
#define RENDERER_CONSTANT_H_
"""

output_file.write(header)



# data
with open(input_filename) as data_file:    
    data = json.load(data_file)

for entry in data:
	#print (entry)
	#print (type(entry))

	print (entry)


	if("data" in entry):
		renderer = entry["r"]

		enum_list = entry["data"]
	
		print (renderer)
		write_renderer_enum(output_file, renderer, enum_list)




#print (type(data))
#pprint(data)



# footer
footer = "\n\n#endif"


output_file.write(footer)
