#ifndef NETWORK_CLIENT_H_
#define NETWORK_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include <dos.h>

#include <conio.h>

#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "game_messages.h"
#include "network_utility.h"




#define	CMD_BUFFER_SIZE			64	
#define	CMD_MASK			(CMD_BACKUP - 1)
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP

// must be power of two
#define	SNAPSHOT_BUFFER_SIZE	16	// copies of Snapshots to keep buffered
#define	UPDATE_MASK		(UPDATE_BACKUP-1)


#define	MAX_ENTITIES_IN_SNAPSHOT	256



#define	PACKET_BACKUP 32	// number of old messages that must be kept on client and
							// server for delta comrpession and ping estimation
#define MAX_ENTITIES 1024
#define MAX_PARSE_ENTITIES 2048


// playerState_t is a full superset of entityState_t as it is used by players,
// so if a playerState_t is transmitted, the entityState_t can be fully derived
// from it.


// playerState is mostly used for server sending down the client about information player information
struct PlayerState
{
	int cmdTime;		// cmd->serverTime of last executed command on the server

};



struct EntityState
{


};



struct Snapshot
{
	

	int	serverTime;

	// since snapshots are sent by server to the client,
	// the playerState is the info about that client
	PlayerState	playerState;

	int numEntities;



};




class Client
{
	public:
		Client();
		Client(RakNet::RakNetGUID guid);
		~Client();

		void init();
		void run();

		RakNet::RakPeerInterface* peer;
		bool isConnected;


		int framecount;
		int frametime;		// msec since last frame

		int realtime;
		int realFrameTime;	

		int nextSnapshotTime;

		int	serverTime;			// may be paused during play

		int	weapon;

		// keeping two entries for mouse smoothing
//		int mouseDx[2];
//		int mouseDy[2];
//		int mouseIndex;
		UserCmd cmds[CMD_BUFFER_SIZE];	// each message will send several old cmds
		int cmdCounter;				// incremented each frame, because multiple
									// frames may need to be packed into a single packet

		Snapshot prevSnapshot;		// latest received from server
		Snapshot snapshots[SNAPSHOT_BUFFER_SIZE];


		RakNet::RakNetGUID m_guid;
		RakNet::SystemAddress systemAddress;
		RakNet::SystemAddress serverSystemAddress;

		// EntityState entityBaseLines[MAX_ENTITIES];
		// EntityState parseEntities[MAX_PARSE_ENTITIES];

};

#endif