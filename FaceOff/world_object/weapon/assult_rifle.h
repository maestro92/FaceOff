#ifndef ASSULT_RIFLE_H_
#define ASSULT_RIFLE_H_

#include "weapon/weapon.h"
#include "pipeline.h"
#include "renderer.h"

class AssultRifle : public Weapon
{
	public:
		AssultRifle();
		AssultRifle(string gunType);
		~AssultRifle();



		virtual int getType();
};

#endif