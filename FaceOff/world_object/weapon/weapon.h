#ifndef WEAPON_H_
#define WEAPON_H_

#include "world_object.h"
#include "imported_model.h"
#include "weapon_enum.h"
#include "weapon_data.h"
#include "particle_effect.h"
#include "smoke_effect.h"
//

#include "global.h"
#include "model_manager.h"


using namespace std;


const float ROTATION_SPEED = 0.0005;

// const int NO_OWNER = -1;

struct Magazine
{
	int cur;
	int max;
};


class Weapon : public WorldObject
{


	public:
		Weapon();
		Weapon(WeaponData data);
		~Weapon();


		void init(WeaponData data);



		virtual void updateGameInfo();



		float m_modelScale;

		glm::vec3 m_firstPOVOffset;
		float m_firstPOVScale;

		glm::vec3 m_thirdPOVOffset;
		float m_thirdPOVScale;



		// void setType(WeaponTypeEnum type);
		int getType();

		void setName(WeaponNameEnum type);
		int getName();

		bool shouldRender();


		ParticleEffect* explode();

		int getCurAmmo();
		


		void startExplodeDelayTimer();


		virtual WeaponSlotEnum getWeaponSlot();
		virtual WeaponNameEnum getWeaponName();



//		void serialize(RakNet::BitStream& bs);
//		void deserialize(RakNet::BitStream& bs, ModelManager* mm);


		virtual void serialize_New(RakNet::BitStream& bs);
		virtual void deserialize_New(RakNet::BitStream& bs);

//		virtual void serialize_Delta(RakNet::BitStream& bs);
//		virtual void deserialize_Delta(RakNet::BitStream& bs);



//		void setGrenadeThrowerId(ObjectId id);
		int getGrenadeThrowerId();

		int getTotalAmmoCount();

	//	virtual bool shouldSend(int clientId);

		void reload();
		void fire();

		bool shouldExplode();
		
		virtual bool ignorePhysics();
		virtual bool ignorePhysicsWith(WorldObject* obj);

		
		virtual void renderGroup(Pipeline& p, Renderer* r);
		virtual void renderWireFrameGroup(Pipeline& p, Renderer* r);

		int getDamage();
		long long getRoundsCooldown();

		std::function<void(Weapon*)> onDelete;

		void setBeingUsed(bool flag);
		bool isBeingUsed();


	private:
		void setData(WeaponData data);

		int m_damage;
		Magazine m_magazine;

		int m_curNumMagazine;
		int m_maxNumMagazine;
		int m_rpm;
		long long m_roundsCooldown;	// cooldowns between rounds


		long long m_explodeDelayStartTime;
		long long m_explodeDelayTime;

		long long m_ignorePhysicsAfterThrownTime;

		bool m_explodeDelayMode;
		bool m_readyToExplode;

		float m_angle;

		bool isBeingUsedFlag;

	//	ObjectId m_grenadeThrowerId;

		WeaponNameEnum m_nameEnum;
		WeaponSlotEnum m_slotEnum;

};



#endif