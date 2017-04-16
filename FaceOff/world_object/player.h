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
#include "network/network_utility.h"

#include "world_object/weapon/weapon.h"

// #include "model_manager.h"

#include "custom_gui/bar.h"

using namespace std;

// class KDTree;

#define DEBUG 1;


const float PLAYER_ROTATION_SPEED = 3.0;
const float PLAYER_FORWARD_SPEED = 1.5;

class Player : public WorldObject
{
    public:
        Player();
		Player(ObjectId id);
//		Player(int id);
		~Player();

		// RakNet::RakNetGUID m_guid;
		glm::vec3 headPositionOffset;
        Camera* m_camera;

		static glm::vec3 firstPOVWeaponOffset;
		static glm::vec3 thirdPOVWeaponOffset;
		
	//	void setId(int id);

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);

		void setDefaultPlayerFlag(bool b);
		

		vector<glm::vec3> m_weaponPositionOffsets; 
		vector<glm::vec3> m_bulletStartPositionOffsets;
		vector<glm::vec3> m_bulletStartPositionOffsetScale;


		// for weapons
		// glm::vec3 getArmPosition();
		glm::vec3 getFirePosition();

		void update(Pipeline& p);
		void update(Pipeline& p, Terrain* terrain);
		void update(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
		void update(glm::vec3 wPos, float pitch, float yaw);

//		void control();

		void updateGameStats();
		// void updateCamera(Pipeline& p);
		// void updateCamera(Pipeline& p);
		void updateCameraViewMatrix(Pipeline& p);

		void updateModel();

		void updateWeaponTransform();
		void updateBulletTransform();
		void adjustWeaponAndBulletPosition();

		bool canJump();
	

		virtual void renderGroup(Pipeline& p, Renderer* r);
		virtual void renderWireFrameGroup(Pipeline& p, Renderer* r);


		// void weaponOnDelete(Weapon* weapon);

		void switchWeapon(WeaponSlotEnum slot);
		void pickUp(Weapon* weapon);
		Weapon* drop();
		void reloadWeapon();
		Weapon* getCurWeapon();

		vector<Weapon*>& getWeapons();

		Weapon* getWeapon(int index);

		void svDebug();
		void clDebug();

//		int getId();

//		int getPlayerId();

		int getClientId();

		bool hasWeaponAtSlot(WeaponSlotEnum slot);
//		int getInstanceId();

//		void setRotation(float pitch, float yaw);

		void fireWeapon(long long nowTime_ms);
		bool notInAttackCooldown(long long nowTime_ms);
		Weapon* throwGrenade();

//		float getCameraPitch();
//		float getCameraYaw();

		/*
		Bar* m_healthBarGUI;
		Bar* m_armorBarGUI;
		Bar* m_ammoBarGUI;
		*/


		bool inGrenadeGatherMode();
		bool isUsingLongRangedWeapon();

		// used for spawn
		/*
		void spawnInfoToBitStream(RakNet::BitStream& bs);
		void toBitStream(RakNet::MessageID msgId, RakNet::BitStream& bs);

		void spawnInfoFromBitStream(RakNet::BitStream& bs, ModelManager* mm);
		void setFromBitStream(RakNet::BitStream& bs);
		*/

		virtual void serialize_New(RakNet::BitStream& bs);
		virtual void deserialize_New(RakNet::BitStream& bs, ModelManager* mm);

//		virtual void serialize_Delta(int flags, RakNet::BitStream& bs);
//		virtual void deserialize_Delta(int flags, RakNet::BitStream& bs);


		// void processInput(Move move);
		void processUserCmd(const UserCmd& cmd);


		int weaponCount;

		bool hasMoved();

		PlayerState getPlayerState();

		bool isDefaultPlayer();

		virtual bool ignorePhysicsWith(WorldObject* obj);

		void updateMidAirVelocity();
		void updateContactNormalInfo(glm::vec3 normal);

		virtual void updateGameInfo();



		void updateVelXZ(float dir);
		void updateVelY(float dir);

		int getCurHP();
		int getCurArmor();
		int getCurAmmo();

		glm::vec3 getXAxis();
		glm::vec3 getYAxis();
		glm::vec3 getZAxis();


	private:
//		int m_id;


		long long m_lastFiredTime;

		glm::vec3 m_midAirHorVel;
		vector<Weapon*> m_weapons;

		Weapon* m_curWeapon;


		bool m_isDefaultPlayer;

		bool m_grenadeGatherMode = false;

		int jumpCoolDown;
		int curJumpCoolDown;
		bool jumped;

		float cdAngle;
};

#endif // PLAYER_H_
