#ifndef BILLBOARD_LIST_H_
#define BILLBOARD_LIST_H_

#include "world_object.h"
#include "utility.h"
#include "billboard_model.h"
#include "renderer_manager.h"

enum BILLBOARD_MODE
{
	UNIFORM,
	RANDOM
};

class BillboardList : public WorldObject
{
	public:
		BillboardList();
		BillboardList(string textureFile);
		~BillboardList();

		void setUniormationFormation(int w, int h, int gap);
		void setUniormationFormation(int w, int h, int gap, Terrain* terrain);

		void setRandomFormation(int maxW, int maxH, int count);
		void setRandomFormation(int maxW, int maxH, int count, Terrain* terrain);

		void setTexture(string textureFile);

		void render(Pipeline& p);

	//	static Renderer r_billboardOneQuad;
	//	static Renderer r_billboardTwoQuad;
		Renderer* p_renderer;

		BillboardModel m_model;
		GLuint m_textureID;

		int m_mode;
		
};


#endif