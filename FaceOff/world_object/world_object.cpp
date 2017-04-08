#include "world_object.h"
#include "model_manager.h"
#include "collision_detection/kd_tree_node.h"

WorldObject::WorldObject()
{
	active = true;

	// m_instanceId = utl::createUniqueObjectID();

    m_position = glm::vec3(0.0, 0.0, 0.0);
    m_velocity = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(1.0, 1.0, 1.0);
    m_rotation = glm::mat4(1.0);

	m_mass = 1.0;
	m_invMass = 1.0 / m_mass;
	
	m_model = NULL;

	m_geometryType = CD_NULL;
	m_aabb = NULL;
	m_sphere = NULL;

	isTested = isCollided = isHit = false;
	isHitCounter = 0;

	setMaterialEnergyRestitution(0.0f);
	setMaterialSurfaceFriction(1);

	m_entityType = SCENE_OBJECT;
	m_dynamicType = STATIC;

	resetCollisionFlags();

	m_modelEnum = -1;
}


WorldObject* WorldObject::getOne()
{
	WorldObject* obj = new WorldObject();
	return obj;
}



WorldObject::~WorldObject()
{
	clearParentNodes();

	if (m_aabb != NULL)
	{
		delete m_aabb;
	}

	else if (m_sphere != NULL)
	{
		delete m_sphere;
	}
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


EntityType WorldObject::getEntityType()
{
	return m_entityType;
}

DynamicType WorldObject::getDynamicType()
{
	return m_dynamicType;
}

void WorldObject::setCollisionDetectionGeometry(CDEnum type)
{
	if (m_model == NULL)
	{
		utl::debug("Error setCollisionDetectionGemoetry has no Model");
		while (1)
		{}
	}

	m_geometryType = type;

	if (type == CD_AABB)
	{
		m_aabb = new AABB();
	}

	else if (type == CD_SPHERE)
	{
		m_sphere = new Sphere();
	}

	updateCollisionDetectionGeometry();
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

void WorldObject::updateCollisionDetectionGeometry()
{
	if (m_geometryType == CD_AABB)
	{
		// The following Two are equivalent
		// glm::mat4 M = m_rotation * glm::scale(m_scale);
		// M = glm::transpose(M);

		glm::mat4 M = glm::scale(m_scale) * m_rotation;

		for (int i = 0; i < 3; i++)
		{
			m_aabb->min[i] = m_aabb->max[i] = m_position[i];

			for (int j = 0; j < 3; j++)
			{
				float e = M[i][j] * m_model->m_aabb.min[j];
				float f = M[i][j] * m_model->m_aabb.max[j];

				if (e < f)
				{
					m_aabb->min[i] += e;
					m_aabb->max[i] += f;
				}
				else
				{
					m_aabb->min[i] += f;
					m_aabb->max[i] += e;
				}
			}
		}
	}
	else if (m_geometryType == CD_SPHERE)
	{
		m_sphere->center = m_position;
		m_sphere->radius = m_scale.x;
	}
}


bool WorldObject::ignorePhysics()
{
	return m_dynamicType == STATIC;
}


bool WorldObject::ignorePhysicsWith(WorldObject* obj)
{
	if (getCollisionFlagIndex() == obj->getCollisionFlagIndex())
	{
		return true;
	}

	return false;
}

bool WorldObject::alreadyTestedPhysicsWith(WorldObject* obj)
{
	int objIndex = obj->getCollisionFlagIndex();

	int arrayIndex = objIndex / ENTITY_COLLISION_ENTRY_SIZE;
	int intIndex = objIndex - arrayIndex * ENTITY_COLLISION_ENTRY_SIZE;
	return (collisionFlags[arrayIndex] >> intIndex) & 1;
}


void WorldObject::resetCollisionFlags()
{
	memset(collisionFlags, 0, sizeof(collisionFlags));
}

void WorldObject::print_uint8_t(uint8_t n)
{
	int i;
	for (i = 8; i >= 0; i--)
		printf("%d", (n & (1 << i)) >> i);
	putchar('\n');
}

void WorldObject::registerCollsionFlag(int objIndex)
{
	int arrayIndex = objIndex / ENTITY_COLLISION_ENTRY_SIZE;
	int intIndex = objIndex - arrayIndex * ENTITY_COLLISION_ENTRY_SIZE;

	
	collisionFlags[arrayIndex] |= 1 << intIndex;

	/*
	if (m_name == "player 0")
	{
		cout << "	size " << ENTITY_COLLISION_ENTRY_SIZE << endl;
		cout << "	arrayIndex " << arrayIndex << endl;
		cout << "	intIndex " << intIndex << endl;
		cout << "	value is " << unsigned(collisionFlags[arrayIndex]) << endl;
		
		print_uint8_t(collisionFlags[arrayIndex]);
	}
	*/

}


WeaponSlotEnum WorldObject::getWeaponSlot()
{
	return WEAPON_SLOT_NULL;
}

WeaponNameEnum WorldObject::getWeaponName()
{
	return WEAPON_NAME_NULL;
}

void WorldObject::updateContactNormalInfo(glm::vec3 normal)
{

}

void WorldObject::clearParentNodes()
{
	//remove object from its stored kdTree nodes
	for (int i = 0; i < m_parentNodes.size(); i++)
	{
		KDTreeNode* kNode = m_parentNodes[i];
		if (kNode == NULL)
			continue;

		kNode->remove(this);
	}
}



#if 1
void WorldObject::serialize(RakNet::BitStream& bs)
{
	bs.Write(objectId.id);

	//unsigned char *val = new unsigned char[m_name.length() + 1];
	//strcpy((char *)val, m_name.c_str());

//	RakNet::RakString rakString("%s", m_name);
//	bs.Write(rakString);



//	RakNet::RakString raksName(m_name.c_str());
//	bs.Write(raksName);
	utl::write(bs, m_name);

	bs.WriteVector(m_position.x, m_position.y, m_position.z);
//	bs.Write(getCameraPitch());
//	bs.Write(getCameraYaw());



	bs.Write(m_modelEnum);

	// cout << "objectId.id " << objectId.id << endl;
	// cout << "	m_modelEnum " << m_modelEnum << endl << endl;

	bs.Write(getGeometryType());
	bs.Write(getMass());
	bs.Write(getMaterialEnergyRestitution());
	bs.Write(getMaterialSurfaceFrictionToBitStream());


	bs.Write(isHit);
}



bool WorldObject::shouldRender()
{
	return true;
}

/*
int WorldObject::getInstanceId()
{
	return objectId.id;
}
*/


int WorldObject::getCollisionFlagIndex()
{
	if (m_entityType == PLAYER)
	{
		return objectId.s.index;
	}
	else
	{
		return objectId.s.index + NUM_MAX_CLIENTS;
	}
}

void WorldObject::deserialize(RakNet::BitStream& bs, ModelManager* mm)
{
	// the message id is already ignored
	bs.Read(objectId.id);

	/*

	RakNet::RakString raksName;
	bs.Read(raksName);
	string str(raksName.C_String);
	m_name = str;
	*/
	utl::read(bs, m_name);

	bs.ReadVector(m_position.x, m_position.y, m_position.z);

	if (m_position.x != 0 || m_position.z != 0)
	{
		utl::clDebug("position is", m_position);
	}

	/*
	float pitch, yaw;
	bs.Read(pitch);					bs.Read(yaw);
	m_camera->m_pitch = pitch;		m_camera->m_yaw = yaw;
	setRotation(pitch, yaw);
	*/
	

	/*
	cout << "	objectId id " << objectId.id << endl;
	cout << "	tag " << objectId.s.tag << endl;
	cout << "	index " << objectId.s.index << endl;
	*/
	bs.Read(m_modelEnum);
	// cout << "	m_modelEnum " << m_modelEnum << endl;
	setModel(mm->get(m_modelEnum));

	bs.Read(m_geometryType);
	setCollisionDetectionGeometry(m_geometryType);

	float mass = 0;
	bs.Read(mass);	setMass(mass);

	float restitution = 0;
	bs.Read(restitution);		setMaterialEnergyRestitution(restitution);

	float friction = 0;
	bs.Read(friction);			setMaterialSurfaceFriction(friction);

	bs.Read(isHit);
//	prevState = GetState();
}





#endif
