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


		string name;
		RakNet::RakNetGUID m_guid;

        Camera* m_camera;

		static glm::vec3 firstPOVWeaponOffset;
		static glm::vec3 thirdPOVWeaponOffset;

		Weapon* m_curCollidedWeapon;

		Renderer* r_gun;
		
		void setId(int id);

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);


		
		
//		vector<WorldObject*> m_weapons;
//		int m_curWeaponIndex;
//		vector<Weapon*> m_weapons;
		vector<glm::vec3> m_weaponPositionOffsets;
		vector<glm::vec3> m_bulletStartPositionOffsets;
		vector<glm::vec3> m_bulletStartPositionOffsetScale;

		glm::vec3 getFirePosition();

		void update(Pipeline& p);
		void update(Pipeline& p, Terrain* terrain);
		void update(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
		void update(glm::vec3 wPos, float pitch, float yaw);

		void control();
		void control(KDTree* t);


		void updateGameStats();
		void updateCamera(Pipeline& p);

		void updateModel();

		void updateWeaponTransform();
		void updateBulletTransform();
		void adjustWeaponAndBulletPosition();

		bool isNotJumping();

		void renderGroup(Pipeline& p, Renderer* r);

		void switchWeapon(WeaponSlotEnum slot);
		void pickUpWeapon(Weapon* weapon);
		Weapon* dropWeapon();
		void reloadWeapon();
		Weapon* getCurWeapon();

		vector<Weapon*> getWeapons();

		int getId();

		bool hasWeaponAtSlot(WeaponSlotEnum slot);

		void fireWeapon();
		Weapon* throwGrenade();

		float getCameraPitch();
		float getCameraYaw();

		Bar* m_healthBarGUI;
		Bar* m_armorBarGUI;
		Bar* m_ammoBarGUI;

		WorldObjectType getObjectType();

		bool inGrenadeGatherMode();
		bool isUsingLongRangedWeapon();

		// used for spawn
	//	void spawnFromBitStream(RakNet::BitStream& bs);

		void toBitStream(RakNet::MessageID msgId, RakNet::BitStream& bs);
		void setFromBitStream(RakNet::BitStream& bs);

		void processInput(Move move);

		bool hasMoved();
		Move getMoveState();

	private:
		int m_id;

		int m_maxHP;
		int m_curHP;

		int m_maxArmor;
		int m_curArmor;

		vector<Weapon*> m_weapons;
		Weapon* m_curWeapon;


		bool m_grenadeGatherMode = false;
};

#endif // PLAYER_H_
