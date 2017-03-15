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
#include "utility.h"

#include "shared.h"

// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP

// must be power of two

const unsigned int CMD_BUFFER_SIZE = 64;
const unsigned int CMD_BUFFER_MASK = (CMD_BUFFER_SIZE - 1);

const unsigned int 	CL_SNAPSHOT_BUFFER_SIZE = 32;	// copies of Snapshots to keep buffered
const unsigned int 	CL_SNAPSHOT_BUFFER_MASK = (CL_SNAPSHOT_BUFFER_SIZE - 1);




#define	PACKET_BACKUP 32	// number of old messages that must be kept on client and
							// server for delta comrpession and ping estimation


const int MAX_ENTITIES_IN_SNAPSHOT = 128;
const int MAX_PARSE_ENTITIES = 2048;
// #define MAX_ENTITIES_IN_SNAPSHOT 1024

// playerState_t is a full superset of entityState_t as it is used by players,
// so if a playerState_t is transmitted, the entityState_t can be fully derived
// from it.



/*
struct Snapshot
{


};
*/


// used to store the snapshot sent down from the server
// i couldn't get the circular parsed entity to work becuz
// during the interpolations stage, you will have to access that object in the past snapshot
// and I cna't think of a constant time method to acess that in a parsed entity
//



struct ClientWorldObjectState
{

	int flags;
	WorldObjectState state;

	ClientWorldObjectState()
	{

	}

	ClientWorldObjectState(int flags)
	{
		this->flags = flags;
	}

	ClientWorldObjectState(int flags, const WorldObjectState& state)
	{
		this->flags = flags;
		this->state = state;
	}
};

struct ClientSnapshot
{
	int ping;
	long long serverTime;

	int messageSequenceNum;	// copied from netchan->incoming_sequence

	// since snapshots are sent by server to the client,
	// the playerState is the info about this client
	PlayerState	playerState;

	int numEntities;
	ClientWorldObjectState entities[MAX_ENTITIES_IN_SNAPSHOT];
	int start;
	int end;

	ClientSnapshot()
	{
		serverTime = -1;
		start = 9999999;
		end = -1;
	}

	bool valid()
	{
		return serverTime != -1;
	}

	void set(int index, const ClientWorldObjectState& entityState)
	{		
		entities[index] = entityState;

		start = std::min(start, index);
		end = std::max(end, index);;
	}

	int getIterationEnd()
	{
		return end + 1;
	}

};



class ClientConnection
{


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


		long long realTime;

//		int realtime;
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

		ClientSnapshot* curSnapshot;		// latest received from server
		ClientSnapshot snapshots[CL_SNAPSHOT_BUFFER_SIZE];

		// index (not anded off) into parseEntities[]
//		int	parseEntitiesIndex;

	
//		I have trouble accessing past frames without doing linear time
	

//		ClientWorldObjectState parseEntities[MAX_PARSE_ENTITIES];
		//		WorldObjectState entityBaseLines[MAX_ENTITIES];
		//		WorldObjectState parseEntities[MAX_PARSE_ENTITIES];

		// the circular objects states used when parsing the snapshot from the server
		// will also be used when the client tries to interpolate across snapshots
		// vector<WorldObjectState> snapshotObjectStates;	

//		int lastUserCmdFrame;	// the frame that the last client usercmd send over
		NetChannel netchan;

		RakNet::RakNetGUID m_guid;
		RakNet::SystemAddress systemAddress;
		RakNet::SystemAddress serverSystemAddress;


		int serverMessageSequence;

		int id;
		
};

#endif