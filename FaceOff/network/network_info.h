#ifndef NETWORK_INFO_H_
#define NETWORK_INFO_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include "RakNetTypes.h"	// Message ID


#define MAX_CLIENTS 10
#define SERVER_PORT 60000

// How often the server sends position updates to the client
// static const int DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES = 250;
static const int DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES = 1000;




// http://gafferongames.com/game-physics/networked-physics/
// weapon firing needs to be done server side


/*

server basically sits in a loop waiting for input form each of the clients.
Each character object has its physics advanced ahead in time individually as input rpcs are received from the client
that owns it

*/

/*
struct Move
{
	double time;
	Input input;
	State state;

};
*/

/*
struct NetworkPlayerInput
{
	bool left;
	bool right;
	bool forward;
	bool back;
	bool jump;
	bool weaponFired;
};
*/

/*
struct NetworkPlayerState
{
	glm::vec3 position;
	glm::vec3 velocity;

};
*/

struct Client
{
	int id, x, y, z;
	RakNet::RakNetGUID guid;

	Client(int passed_id) : id(passed_id)
	{}
};

#endif