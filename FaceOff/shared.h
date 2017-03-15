#ifndef SHARED_H_
#define SHARED_H_

#include "define.h"


#define NUM_MAX_OBJECTS 256

const int INVALID_OBJECT_ID = -1;

const int END_OF_SV_SNAPSHOT_PLAYERS = -9999;
const int END_OF_SV_SNAPSHOT_ENTITIES = -9999;


// PlayerState communication
#define	PS_M_TYPE			(1<<0)
#define	PS_M_ORIGIN			(1<<1)
#define	PS_M_VELOCITY		(1<<2)
#define	PS_M_TIME			(1<<3)
#define	PS_M_FLAGS			(1<<4)
#define	PS_M_GRAVITY		(1<<5)
#define	PS_M_DELTA_ANGLES	(1<<6)

#define	PS_VIEWOFFSET		(1<<7)
#define	PS_VIEWANGLES		(1<<8)
#define	PS_KICKANGLES		(1<<9)
#define	PS_BLEND			(1<<10)
#define	PS_FOV				(1<<11)
#define	PS_WEAPONINDEX		(1<<12)
#define	PS_WEAPONFRAME		(1<<13)
#define	PS_RDFLAGS			(1<<14)



// WorldObjectState communication, 

// U stands for Update

// try to pack the common update flags into the first byte
#define	U_DELTA		(1<<0)
#define	U_ADD		(1<<1)
#define	U_REMOVE	(1<<2)

#define	U_POSITION0	(1<<3)
#define	U_POSITION1	(1<<4)
#define	U_POSITION2	(1<<5)
#define	U_ANGLE0	(1<<6)
#define	U_ANGLE1	(1<<7)
#define	U_ANGLE2	(1<<8)
#define	U_FRAME8	(1<<9)		// frame is a byte

// second byte
#define	U_EVENT		(1<<10)
#define	U_NUMBER16	(1<<11)		// NUMBER8 is implicit if not set
#define	U_MOREBITS1	(1<<12)		// read one additional byte
#define	U_MODEL		(1<<13)
#define U_RENDERFX8	(1<<14)		// fullbright, etc
#define	U_EFFECTS8	(1<<15)		// autorotate, trails, etc
#define	U_MOREBITS2	(1<<16)		// read one additional byte

// third byte
#define	U_SKIN8		(1<<17)
#define	U_FRAME16	(1<<18)		// frame is a short
#define	U_RENDERFX16 (1<<19)	// 8 + 16 = 32
#define	U_EFFECTS16	(1<<20)		// 8 + 16 = 32
#define	U_MODEL2	(1<<21)		// weapons, flags, etc
#define	U_MODEL3	(1<<22)
#define	U_MODEL4	(1<<23)
#define	U_MOREBITS3	(1<<24)		// read one additional byte

// fourth byte
#define	U_OLDORIGIN	(1<<25)		// FIXME: get rid of this
#define	U_SKIN16	(1<<26)
#define	U_SOUND		(1<<27)
#define	U_SOLID		(1<<28)

#define U_SCENE_OBJECT	(1<<29)
#define U_PLAYER	(1<<30)
#define U_WEAPON	(1<<31)

union ObjectId
{
	int id;
	struct 
	{
		uint16_t tag;
		uint16_t index;
	} s;

	ObjectId()
	{

	}

	ObjectId(int Id)
	{
		id = Id;
	}
};

// mainly used in building the snapshot
// we need to keep a deep copy of WorldObjectStates in the curSVSnapshotObjects
struct WorldObjectState
{
	ObjectId objectId;
	glm::vec3 position;
	glm::vec3 angles;

	WorldObjectState()
	{

	}

	WorldObjectState(ObjectId objId, glm::vec3 pos, glm::vec3 angles)
	{
		this->objectId = objId;
		this->position = pos;
		this->angles = angles;
	}
};


// see quake3 net_chan.c 
// Netchan_setup
struct NetChannel
{

	NetChannel()
	{
		incomingSequence = 0;	// in quake3, this is mainly used to check packets are out of order?								
		outgoingSequence = 1;
	}

	int incomingSequence;
	int outgoingSequence;
};


// playerState is mostly used for server sending down the client about information player information
struct PlayerState
{
	int cmdTime;		// cmd->serverTime of last executed command on the server

};



struct EntityState
{


};



#endif