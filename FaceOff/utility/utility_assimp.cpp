#include "utility.h"

glm::vec3 Utility::toGlmVec(aiVector3D& v)
{
    return glm::vec3(v.x, v.y, v.z);
}


glm::vec3 Utility::toGlmVec(aiColor3D& color)
{
    return glm::vec3((float)(color.r), (float)(color.g), (float)(color.b));
}


glm::vec3 Utility::toGlmVec(aiColor4D& color)
{
    return glm::vec3((float)(color.r), (float)(color.g), (float)(color.b));
}


glm::mat4 Utility::toGlmMat(aiMatrix3x3 m)
{
    float temp[16] = {m.a1, m.a2, m.a3, 0.0,
                      m.b1, m.b2, m.b3, 0.0,
                      m.c1, m.c2, m.c3, 0.0,
                      0.0,  0.0,  0.0,  1.0};
    return glm::make_mat4(temp);
}


glm::mat4 Utility::toGlmMat(aiMatrix4x4& m)
{
    float temp[16] = {m.a1, m.a2, m.a3, m.a4,
                      m.b1, m.b2, m.b3, m.b4,
                      m.c1, m.c2, m.c3, m.c4,
                      m.d1, m.d2, m.d3, m.d4};
    return glm::make_mat4(temp);
}


glm::mat4 Utility::toGlmMat(const aiMatrix4x4& m)
{
    float temp[16] = {m.a1, m.a2, m.a3, m.a4,
                      m.b1, m.b2, m.b3, m.b4,
                      m.c1, m.c2, m.c3, m.c4,
                      m.d1, m.d2, m.d3, m.d4};
    return glm::make_mat4(temp);
}
