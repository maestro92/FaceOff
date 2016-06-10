#include "world_object.h"

Model* WorldObject::DEFAULT_MODEL;

WorldObject::WorldObject()
{
	m_instanceId = utl::createUniqueObjectID();

    m_position = glm::vec3(0.0, 0.0, 0.0);
    m_velocity = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(1.0, 1.0, 1.0);
    m_rotation = glm::mat4(1.0);

	m_mass = 1.0;
	m_invMass = 1.0 / m_mass;
	
	m_model = DEFAULT_MODEL;
	isTested = isCollided = isHit = alreadyFireTested = false;
	isHitCounter = 0;

	m_dynamicType = STATIC;
	m_BVType = BV_AABB;
}


void WorldObject::renderSingle(Pipeline& p, Renderer* r)
{
	renderSingle(p, r, RENDER_PASS1);
}

void WorldObject::renderSingle(Pipeline& p, Renderer* r, int pass)
{
	r->enableShader(pass);
		renderGroup(p, r, pass);
	r->disableShader(pass);
}

void WorldObject::renderGroup(Pipeline& p, Renderer* r)
{
	renderGroup(p, r, RENDER_PASS1);
}

void WorldObject::renderGroup(Pipeline& p, Renderer* r, int pass)
{
	p.pushMatrix();
		p.translate(m_position);
		p.addMatrix(m_rotation);
		p.scale(m_scale);
		r->loadUniformLocations(p, pass);
		m_model->render();
	p.popMatrix();
}

WorldObjectType WorldObject::getObjectType()
{
	return SCENE_OBJECT;
}

DynamicType WorldObject::getDynamicType()
{
	return m_dynamicType;
}


BVEnum WorldObject::getBoundingVolumeType()
{
	return m_BVType;
}

/*

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

*/

void WorldObject::renderStaticWireFrameGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);
		m_staticWireFrameModel->render();
	p.popMatrix();
}


void WorldObject::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		p.translate(m_position);
		p.addMatrix(m_rotation);
		p.scale(m_scale);
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
//		m_minP[i] = m_maxP[i] = m_position[i];

		m_aabb.min[i] = m_aabb.max[i] = m_position[i];

		for (int j = 0; j < 3; j++)
		{
			float e = M[i][j] * m_model->m_aabb.min[j];
			float f = M[i][j] * m_model->m_aabb.max[j];


			if (e < f)
			{
				//m_minP[i] += e;
				//m_maxP[i] += f;
				m_aabb.min[i] += e;
				m_aabb.max[i] += f;
			}
			else
			{
				m_aabb.min[i] += f;
				m_aabb.max[i] += e;
				//m_minP[i] += f;
				//m_maxP[i] += e;
			}
		}
	}

}



void WorldObject::updateAABB(glm::vec3& maxP, glm::vec3& minP, glm::vec3 pos, glm::mat4 rotation, glm::vec3 scale)
{
	/*
	The following Two are equivalent
	glm::mat4 M = m_rotation * glm::scale(m_scale);
	M = glm::transpose(M);
	*/
	glm::mat4 M = glm::scale(scale) * rotation;


	for (int i = 0; i < 3; i++)
	{
		minP[i] = maxP[i] = pos[i];

		for (int j = 0; j < 3; j++)
		{
			float e = M[i][j] * m_model->m_aabb.min[j];
			float f = M[i][j] * m_model->m_aabb.max[j];


			if (e < f)
			{
				minP[i] += e;
				maxP[i] += f;
			}
			else
			{
				minP[i] += f;
				maxP[i] += e;
			}
		}
	}
}



void WorldObject::setAABBByPosition(float x, float y, float z)
{
	setPosition(x, y, z);
	updateAABB();
}


void WorldObject::setAABBByPosition(glm::vec3 pos)
{
	setPosition(pos);
	updateAABB();
}


void WorldObject::updateGameInfo()
{
	if (isHit)
	{
		isHitCounter++;
	}

	if (isHitCounter >= 50)
	{
		isHit = false;
		isHitCounter = 0;
	}

}

void WorldObject::update()
{

}

/*
void WorldObject::removeSelfFromNodes()
{
	for (int i = 0; i < m_parentNodes.size(); i++)
	{
		KDTreeNode* kNode = m_parentNodes[i];
		(kNode->m_objects2).erase(m_instanceId);
	}
}
*/

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