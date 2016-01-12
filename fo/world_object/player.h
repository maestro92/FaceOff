#ifndef PLAYER_H_
#define PLAYER_H_

#include "first_person_camera.h"
#include "world_object.h"

class Player : public WorldObject
{
    public:
        Player();
        ~Player();

        Camera* camera;
        void update();

        void render();
};

#endif // PLAYER_H_
