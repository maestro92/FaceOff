#ifndef PLAYER_H_
#define PLAYER_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "first_person_camera.h"
#include "third_person_camera.h"


#include "collision_detection/kd_tree.h"
#include "world_object.h"

#include "renderer.h"
#include "renderer_manager.h"
#include "model.h"
#include "imported_model.h"
#include "particle.h"
#include "terrain/terrain.h"

#include <list>


#include "world_object/weapon/weapon.h"
#include "world_object/weapon/assult_rifle.h"


using namespace std;

// class KDTree;

class Player : public WorldObject
{
    public:
        Player();
		Player(int id);
		~Player();

		int m_id;
		string name;
		RakNet::RakNetGUID m_guid;

        Camera* m_camera;
		Model* m_model;
//		Renderer* m_renderer;
		Renderer* r_gun;
		
		Sphere m_boundingSphere;

		void setId(int id);

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);


//		vector<WorldObject*> m_weapons;
		int m_curWeaponIndex;
		vector<Weapon*> m_weapons;
		vector<glm::vec3> m_weaponPositionOffsets;
		vector<glm::vec3> m_bulletStartPositionOffsets;
		vector<glm::vec3> m_bulletStartPositionOffsetScale;

		void update(Pipeline& p);
		void update(Pipeline& p, Terrain* terrain);
		void update(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
		void update(glm::vec3 wPos, float pitch, float yaw);

		void control();
		void control(KDTree* t);

		void updateCamera(Pipeline& p);
//		void updateCollision(KDTree* tree);

		void updateModel();

		void updateWeaponTransform();
		void updateBulletTransform();
		void adjustWeaponAndBulletPosition();


		void render(Pipeline& p, Renderer* r);
		void renderWeapon(Pipeline& p);

		void renderModel(Pipeline& p, Renderer* r);

		void addWeapon(Weapon* weapon);
		Weapon* getCurWeapon();

		void fireWeapon(list<Particle>& bullets);


		float getCameraPitch();
		float getCameraYaw();

	private:
		float m_maxHP;
		float m_curHP;

};

#endif // PLAYER_H_
