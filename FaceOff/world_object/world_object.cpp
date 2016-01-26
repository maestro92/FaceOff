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
