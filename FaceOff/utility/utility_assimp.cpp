#include "utility_assimp.h"

glm::vec3 utl::toGlmVec(aiVector3D& v)
{
    return glm::vec3(v.x, v.y, v.z);
}


glm::vec3 utl::toGlmVec(aiColor3D& color)
{
    return glm::vec3((float)(color.r), (float)(color.g), (float)(color.b));
}


glm::vec3 utl::toGlmVec(aiColor4D& color)
{
    return glm::vec3((float)(color.r), (float)(color.g), (float)(color.b));
}



/*
aiMatrix4x4 stores in row-major
		a1 a2 a3 a4
		b1 b2 b3 b4
		c1 c2 c3 c4
		d1 d2 d3 d4
meaning in linear memory, it is
		
	a1 a2 a3 a4 b1 b2 b3 b4 c1 c2 c3 c4 d1 d2 d3 d4

glm is column-major, so we give the transpose, 

or

	a1 b1 c1 d1 a2 b2 c2 d2 a3 b3 c3 d3 a4 b4 c4 d4

in linear memory
*/

glm::mat4 utl::toGlmMat(aiMatrix3x3& m)
{
	
    float temp[16] = {m.a1, m.b1, m.c1, 0.0,
                      m.a2, m.b2, m.c2, 0.0,
                      m.a3, m.b3, m.c3, 0.0,
                      0.0,  0.0,  0.0,  1.0};
    return glm::make_mat4(temp);
	
	/*
	glm::mat4 m1;
	m1[0][0] = m2.a1; m1[1][0] = m2.a2; m1[2][0] = m2.a3; m1[3][0] = 0.0;
	m1[0][1] = m2.b1; m1[1][1] = m2.b2; m1[2][1] = m2.b3; m1[3][1] = 0.0;
	m1[0][2] = m2.c1; m1[1][2] = m2.c2; m1[2][2] = m2.c3; m1[3][2] = 0.0;
	m1[0][3] = 0.0;   m1[1][3] = 0.0;   m1[2][3] = 0.0;   m1[3][3] = 1.0;
	return m1;
	*/
}


glm::mat4 utl::toGlmMat(aiMatrix4x4& m)
{
	
	float temp[16] = { m.a1, m.b1, m.c1, m.d1,
					   m.a2, m.b2, m.c2, m.d2,
					   m.a3, m.b3, m.c3, m.d3,
					   m.a4, m.b4, m.c4, m.d4 };
	return glm::make_mat4(temp);
	
	/*
	glm::mat4 m1;
	m1[0][0] = m2.a1; m1[1][0] = m2.a2; m1[2][0] = m2.a3; m1[3][0] = m2.a4;
	m1[0][1] = m2.b1; m1[1][1] = m2.b2; m1[2][1] = m2.b3; m1[3][1] = m2.b4;
	m1[0][2] = m2.c1; m1[1][2] = m2.c2; m1[2][2] = m2.c3; m1[3][2] = m2.c4;
	m1[0][3] = m2.d1; m1[1][3] = m2.d2; m1[2][3] = m2.d3; m1[3][3] = m2.d4;
	return m1;
	*/
}


glm::mat4 utl::toGlmMat(const aiMatrix4x4& m)
{
	
    float temp[16] = {m.a1, m.b1, m.c1, m.d1,
                      m.a2, m.b2, m.c2, m.d2,
                      m.a3, m.b3, m.c3, m.d3,
                      m.a4, m.b4, m.c4, m.d4};
    return glm::make_mat4(temp);
	
	/*
	glm::mat4 m1;
	m1[0][0] = m2.a1; m1[1][0] = m2.a2; m1[2][0] = m2.a3; m1[3][0] = m2.a4;
	m1[0][1] = m2.b1; m1[1][1] = m2.b2; m1[2][1] = m2.b3; m1[3][1] = m2.b4;
	m1[0][2] = m2.c1; m1[1][2] = m2.c2; m1[2][2] = m2.c3; m1[3][2] = m2.c4;
	m1[0][3] = m2.d1; m1[1][3] = m2.d2; m1[2][3] = m2.d3; m1[3][3] = m2.d4;
	return m1;
	*/
}

glm::mat4 utl::toGlmMat2(aiMatrix4x4& m2)
{
	/*
	glm::mat4 m1;
	m1[0][0] = m2.a1; m1[0][1] = m2.a2; m1[0][2] = m2.a3; m1[0][3] = m2.a4;
	m1[1][0] = m2.b1; m1[1][1] = m2.b2; m1[1][2] = m2.b3; m1[1][3] = m2.b4;
	m1[2][0] = m2.c1; m1[2][1] = m2.c2; m1[2][2] = m2.c3; m1[2][3] = m2.c4;
	m1[3][0] = m2.d1; m1[3][1] = m2.d2; m1[3][2] = m2.d3; m1[3][3] = m2.d4;
	return m1;
	*/
	glm::mat4 m1;
	m1[0][0] = m2.a1; m1[1][0] = m2.a2; m1[2][0] = m2.a3; m1[3][0] = m2.a4;
	m1[0][1] = m2.b1; m1[1][1] = m2.b2; m1[2][1] = m2.b3; m1[3][1] = m2.b4;
	m1[0][2] = m2.c1; m1[1][2] = m2.c2; m1[2][2] = m2.c3; m1[3][2] = m2.c4;
	m1[0][3] = m2.d1; m1[1][3] = m2.d2; m1[2][3] = m2.d3; m1[3][3] = m2.d4;
	return m1;
}
