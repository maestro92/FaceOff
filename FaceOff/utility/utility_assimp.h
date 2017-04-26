#ifndef UTILITY_ASSIMP_H_
#define UTILITY_ASSIMP_H_

#include "define.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace utl
{
	/// utl_assimp.cpp
	glm::vec3 toGlmVec(aiVector3D& v);
	glm::vec3 toGlmVec(aiColor3D& c);
	glm::vec3 toGlmVec(aiColor4D& c);

	// for matrix, aiMatrix are row-major, glm::mat are column-major
	// essentially, we are changing row-major to column-major matrix
	glm::mat4 toGlmMat(aiMatrix3x3& m);
	glm::mat4 toGlmMat(aiMatrix4x4& m);
	glm::mat4 toGlmMat(const aiMatrix4x4& m);
	glm::mat4 toGlmMat2(aiMatrix4x4& m2);
}

#endif