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

#include "model_manager.h"

#include "custom_gui/bar.h"

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


		void updateGameStatus();
		void updateCamera(Pipeline& p);

		void updateModel();

		void updateWeaponTransform();
		void updateBulletTransform();
		void adjustWeaponAndBulletPosition();


		void renderGroup(Pipeline& p, Renderer* r);
	//	void render(Pipeline& p, Renderer* r, ModelManager& mm);

		void renderWeapon(Pipeline& p);

//		void renderModel(Pipeline& p, Renderer* r);

		void addWeapon(Weapon* weapon);
		Weapon* getCurWeapon();

		void fireWeapon(list<Particle>& bullets);


		float getCameraPitch();
		float getCameraYaw();

		Bar* m_healthBarGUI;
		Bar* m_armorBarGUI;
		Bar* m_ammoBarGUI;

	private:
		int m_maxHP;
		int m_curHP;

		int m_maxArmor;
		int m_curArmor;
};

#endif // PLAYER_H_
