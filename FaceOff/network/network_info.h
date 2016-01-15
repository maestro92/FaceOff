#ifndef NETWORK_INFO_H_
#define NETWORK_INFO_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include "RakNetTypes.h"	// Message ID


#define MAX_CLIENTS 10
#define SERVER_PORT 60000

// How often the server sends position updates to the client
static const int DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES = 250;

struct Client
{
	int id, x, y, z;
	RakNet::RakNetGUID guid;

	Client(int passed_id) : id(passed_id)
	{}
};

#endif