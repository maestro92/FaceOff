
#include "weapon.h"

unordered_map<string, ImportedModel> Weapon::s_weaponModels;


Weapon::Weapon()
{

}

Weapon::~Weapon()
{

}

void Weapon::initWeaponModels()
{
	// Melee

	// Pistols

	// SubMachine guns


	// Assult Rifles

	// Sniper Rifles
	s_weaponModels["Ak-47"] = ImportedModel("./Assets/models/weapons/Ak_47/Ak-47.obj");

	// Machine Guns
}

void Weapon::render(Pipeline& p, Renderer* r)
{
	r->enableShader();
	p.pushMatrix();
		p.translate(m_position);
		p.addMatrix(m_rotation);
		p.scale(m_scale);
		r->loadUniformLocations(p);
		Weapon::s_weaponModels[m_gunType].render();
	p.popMatrix();
	r->disableShader();

}

