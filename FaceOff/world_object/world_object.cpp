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
	m_geometryType = GM_AABB;



}


void WorldObject::renderSingle(Pipeline& p, Renderer* r)
{
	r->enableShader();
		renderGroup(p, r);
	r->disableShader();
}

void WorldObject::renderGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		p.translate(m_position);
		p.addMatrix(m_rotation);
		p.scale(m_scale);
		r->setUniLocs(p);
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


GMEnum WorldObject::getGeometryType()
{
	return m_geometryType;
}


void WorldObject::renderStaticWireFrameGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->setUniLocs(p);
		m_staticWireFrameModel->render();
	p.popMatrix();
}


void WorldObject::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		p.translate(m_position);
		p.addMatrix(m_rotation);
		p.scale(m_scale);
		r->setUniLocs(p);
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


void WorldObject::addParentNode(KDTreeNode* node)
{
	if (!m_emptyIndexPool.empty())
	{
		int index = m_emptyIndexPool.front();
		m_emptyIndexPool.pop();
		m_parentNodes[index] = node;
	}
	else
	{
		m_parentNodes.push_back(node);
	}
}

