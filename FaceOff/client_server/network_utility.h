#ifndef NETWORK_UTILITY_H_
#define NETWORK_UTILITY_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include "RakNetTypes.h"	// Message ID
#include <string>

#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "shared.h"

using namespace std;

#define MAX_CLIENTS 10

// How do you choose a port ? You can choose whatever you want as long as no one else is using it and its within the range of 2 ^ 16 (0 to 65535).
#define SERVER_PORT 60000
#define CLIENT_PORT 60006

#define INFINITE_PING 99999

// http://gafferongames.com/game-physics/networked-physics/
// weapon firing needs to be done server side


/*

server basically sits in a loop waiting for input form each of the clients.
Each character object has its physics advanced ahead in time individually as input rpcs are received from the client
that owns it

*/





const float TURN_SPEED = 0.5;
const float FORWARD_SPEED = 0.4;

const string serverDebugPrefix = "";


// this is sent to the server each client frame
struct UserCmd
{
//	int playerId;
	long long serverTime;		// the time that this cmd was acknowledged by the server
	float angles[3];	// int angles[3]?
	uint8_t buttons;
	uint8_t weapon;
	/*
	bool forwardmove; 
	bool rightmove;
	bool 
	bool 
	*/

	UserCmd()
	{
//		playerId = 0;
		serverTime = 0;
		angles[PITCH] = 0.0f;
		angles[YAW] = 0.0f;
		angles[ROLL] = 0.0f;
		buttons = 0;
		weapon = 0;
	}


	void serialize(RakNet::BitStream& bs)
	{
		bs.Write(serverTime);
//		bs.Write(playerId);
		
		bs.Write(angles[0]);
		bs.Write(angles[1]);
		bs.Write(angles[2]);

		bs.Write(buttons);
		bs.Write(weapon);
	}

	void deserialize(RakNet::BitStream& bs)
	{
		bs.Read(serverTime);
//		bs.Read(playerId);

		bs.Read(angles[0]);
		bs.Read(angles[1]);
		bs.Read(angles[2]);

		bs.Read(buttons);
		bs.Read(weapon);
	}

};





#endif



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

/*
struct Client
{
int id, x, y, z;
RakNet::RakNetGUID guid;

Client(int passed_id) : id(passed_id)
{}
};
*/