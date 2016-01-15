#ifndef PLAYER_H_
#define PLAYER_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "first_person_camera.h"
#include "world_object.h"
#include "renderer.h"
#include "renderer_manager.h"
#include "model.h"
#include "imported_model.h"


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
		Renderer* m_renderer;

		void setId(int id);

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);


		void update(Pipeline& p);

		void render(Pipeline& p);
};

#endif // PLAYER_H_
