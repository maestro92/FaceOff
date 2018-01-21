#include "quad_model.h"


QuadModel::QuadModel()
{}

QuadModel::~QuadModel()
{}

QuadModel::QuadModel(int w, int h)
{
    init(0.0, w,
         0.0, h,
         0, 0, 1);
}

QuadModel::QuadModel(int w, int h, float uv_x, float uv_y, float uv_w)
{
    init(0.0, w,
         0.0, h,
         uv_x, uv_y, uv_w);
}

QuadModel::QuadModel(float l, float r,
                     float b, float t)
{
    init(l, r,
         b, t,
         0, 0, 1);
}


QuadModel::QuadModel(float l, float r,
                     float b, float t,
                     float uv_x, float uv_y, float uv_w)
{
    init(l, r,
         b, t,
         uv_x, uv_y, uv_w);
}

void QuadModel::init(float l, float r,
                     float b, float t,
                     float uv_x, float uv_y, float uv_w)
{
	m_type = ModelType::quad;

	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	VertexData tmp;

    float uv_x0 = uv_x;  float uv_x1 = uv_x + 1;
    float uv_y0 = uv_y;  float uv_y1 = uv_y + 1;


    /// 0. bot left
    tmp.m_position = glm::vec3(l, b, 0.0);
    tmp.m_UV = glm::vec2(uv_x0 * uv_w, uv_y0 * uv_w);
    vertices.push_back(tmp);
    /// 1. bot right
    tmp.m_position = glm::vec3(r, b, 0.0);
    tmp.m_UV = glm::vec2(uv_x1 * uv_w, uv_y0 * uv_w);
    vertices.push_back(tmp);

    /// 2. top right
    tmp.m_position = glm::vec3(r, t, 0.0);
    tmp.m_UV = glm::vec2(uv_x1 * uv_w, uv_y1 * uv_w);
    vertices.push_back(tmp);
    /// 3. top left
    tmp.m_position = glm::vec3(l, t, 0.0);
    tmp.m_UV = glm::vec2(uv_x0 * uv_w, uv_y1 * uv_w);
    vertices.push_back(tmp);

    initIndices(indices);

    Mesh m(vertices, indices);
    m_meshes.push_back(m);
}



void QuadModel::initIndices(vector<unsigned int>& indices)
{
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
}


