#ifndef BILLBOARD_LIST_H_
#define BILLBOARD_LIST_H_

#include "world_object.h"
#include "utility.h"
#include "billboard_model.h"
class BillboardList : public WorldObject
{
	public:
		BillboardList();
		BillboardList(string textureFile);
		~BillboardList();

		void setTexture(string textureFile);

		void render(Pipeline& p);

		static Renderer r_billboard;

		BillboardModel m_model;
		GLuint m_textureID;
};


#endif