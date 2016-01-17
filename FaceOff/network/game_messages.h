#ifndef GAME_MESSAGES_H_
#define GAME_MESSAGES_H_

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

enum GameMessages
{
	SPAWN_POSITION = ID_USER_PACKET_ENUM + 1,
	PLAYER_UPDATE = ID_USER_PACKET_ENUM + 2,
	YOUR_TURN = ID_USER_PACKET_ENUM + 3,
	NEW_CLIENT = ID_USER_PACKET_ENUM + 4,
	LOBBY_WAIT_END = ID_USER_PACKET_ENUM + 5
};

#endif