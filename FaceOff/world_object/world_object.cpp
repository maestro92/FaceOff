#include "world_object.h"

Model* WorldObject::DEFAULT_MODEL;

WorldObject::WorldObject()
{
    m_position = glm::vec3(0.0, 0.0, 0.0);
    m_velocity = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(1.0, 1.0, 1.0);
    m_rotation = glm::mat4(1.0);

	m_model = DEFAULT_MODEL;

}


void WorldObject::renderSingle(Pipeline& m_pipeline, Renderer* renderer)
{
	renderSingle(m_pipeline, renderer, RENDER_PASS1);
}

void WorldObject::renderSingle(Pipeline& m_pipeline, Renderer* renderer, int pass)
{
	renderer->enableShader(pass);
	m_pipeline.pushMatrix();
		m_pipeline.translate(m_position);
		m_pipeline.addMatrix(m_rotation);
		m_pipeline.scale(m_scale);
		renderer->loadUniformLocations(m_pipeline, pass);
		m_model->render();
	m_pipeline.popMatrix();
	renderer->disableShader(pass);
}

void WorldObject::renderGroup(Pipeline& m_pipeline, Renderer* renderer)
{
	renderGroup(m_pipeline, renderer, RENDER_PASS1);
}

void WorldObject::renderGroup(Pipeline& m_pipeline, Renderer* renderer, int pass)
{
	m_pipeline.pushMatrix();
		m_pipeline.translate(m_position);
		m_pipeline.addMatrix(m_rotation);
		m_pipeline.scale(m_scale);
		renderer->loadUniformLocations(m_pipeline);
		m_model->render();
	m_pipeline.popMatrix();
}




void WorldObject::renderSingle(Pipeline& m_pipeline, Renderer* renderer, Model* model)
{
    renderSingle(m_pipeline, renderer, RENDER_PASS1, model);
}

void WorldObject::renderSingle(Pipeline& m_pipeline, Renderer* renderer, int pass, Model* model)
{
    renderer->enableShader(pass);
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.addMatrix(m_rotation);
		m_pipeline.scale(m_scale);
		renderer->loadUniformLocations(m_pipeline, pass);
        model->render();
    m_pipeline.popMatrix();
    renderer->disableShader(pass);
}

void WorldObject::renderGroup(Pipeline& m_pipeline, Renderer* renderer, Model* model)
{
    renderGroup(m_pipeline, renderer, RENDER_PASS1, model);
}

void WorldObject::renderGroup(Pipeline& m_pipeline, Renderer* renderer, int pass, Model* model)
{
    m_pipeline.pushMatrix();
        m_pipeline.translate(m_position);
        m_pipeline.addMatrix(m_rotation);
		m_pipeline.scale(m_scale);
		renderer->loadUniformLocations(m_pipeline);
        model->render();
    m_pipeline.popMatrix();
}




void WorldObject::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);
		m_wireFrameModel->render();
	p.popMatrix();
}

void WorldObject::updateAABB()
{

	/*
	The following Two are equivalent
	glm::mat4 M = m_rotation * glm::scale(m_scale);
	M = glm::transpose(M);
	*/
	glm::mat4 M = glm::scale(m_scale) * m_rotation;


	for (int i = 0; i < 3; i++)
	{
		m_minP[i] = m_maxP[i] = m_position[i];

		for (int j = 0; j < 3; j++)
		{
			float e = M[i][j] * m_model->m_minP[j];
			float f = M[i][j] * m_model->m_maxP[j];

			if (e < f)
			{
				m_minP[i] += e;
				m_maxP[i] += f;
			}
			else
			{
				m_minP[i] += f;
				m_maxP[i] += e;
			}
		}
	}

}

/*
CubeWireFrameModel WorldObject::createWireFrameModel()
{
	
	m_maxX = m_model->m_maxX * m_scale.x + m_position.x;
	m_minX = m_model->m_minX * m_scale.x + m_position.x;

	m_maxY = m_model->m_maxY * m_scale.y + m_position.y;
	m_minY = m_model->m_minY * m_scale.y + m_position.y;
	
	m_maxZ = m_model->m_maxZ * m_scale.z + m_position.z;
	m_minZ = m_model->m_minZ * m_scale.z + m_position.z;
	
	CubeWireFrameModel model(m_maxX, m_minX, m_maxY, m_minY, m_maxZ, m_minZ);
	return model;
}
*/