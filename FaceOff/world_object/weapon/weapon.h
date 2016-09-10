#ifndef WEAPON_H_
#define WEAPON_H_

#include "world_object.h"
#include "imported_model.h"
#include "weapon_enum.h"
#include "weapon_data.h"
#include "particle_effect.h"
#include "smoke_effect.h"

using namespace std;


const float ROTATION_SPEED = 0.0005;

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




		int m_damage;
		Magazine m_magazine;
		int m_magazineCount;

		virtual void updateGameInfo();

		bool hasOwner;

		float m_modelScale;


		glm::vec3 m_firstPOVOffset;
		float m_firstPOVScale;






		void setData(WeaponData data);

		// void setType(WeaponTypeEnum type);
		int getType();

		void setName(WeaponNameEnum type);
		int getName();

		WorldObjectType getObjectType();


		void setGrenadeThrowerId(int id);
		int getGrenadeThrowerId();

		ParticleEffect* explode();


		void startExplodeDelayTimer();


		virtual WeaponSlotEnum getWeaponSlot();
		virtual WeaponNameEnum getWeaponName();


		bool shouldExplode();

		virtual bool ignorePhysics(WorldObject* obj);


	private:
		long long m_explodeDelayStartTime;
		long long m_explodeDelayTime;

		long long m_ignorePhysicsAfterThrownTime;

		bool m_explodeDelayMode;
		bool m_readyToExplode;

		float m_angle;

		int m_grenadeThrowerInstanceId;

		WeaponSlotEnum m_slotEnum;
		WeaponNameEnum m_nameEnum;
};



#endif