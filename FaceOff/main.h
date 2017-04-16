#ifndef MAIN_H_
#define MAIN_H_

class WorldObject;
struct KDTreeNode;

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <unordered_set>
#include "SDL.h"
#include "SDL_image.h"

#include "define.h"
#include "utility.h"

#include <queue>

#include <GL/glew.h>

#include "cube_wireframe_model.h"

#include "utility.h"
#include "shader.h"

#include "label.h"
#include "gui_manager.h"
#include <chrono>

#include "imported_model.h"
#include "pipeline.h"

#include <ft2build.h>

#include "renderer_manager.h"
#include "renderer.h"
#include "renderer_constants.h"
#include "player.h"

#include "light\light_manager.h"
#include "first_person_camera.h"

#include "quad_model.h"
#include "xyz_axis_model.h"
#include "world_object.h"
#include "particle.h"
#include "skybox.h"
#include "billboard_list.h"

#include "world_object\particle_effect\particle_effect.h"
#include "world_object\particle_effect\fire_work_effect.h"
#include "world_object\particle_effect\smoke_effect.h"

#include "model_manager.h"

#define FRAME_VALUES 10
#include <list>

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID
// #include "network/network_manager.h"

#include "network\network_manager.h"
#include "network\server.h"
#include "network\client.h"

#include "game_messages.h"

#include "collision_detection/contact_data.h"
#include "collision_detection/collision_detection.h"

#include "collision_detection/kd_tree.h"

#include "terrain/terrain.h"
#include "terrain/multitexture_terrain.h"

#include "network\network_utility.h"


#include <al.h>
#include <alc.h>

using namespace std;
/*

For style
use http://google-styleguide.googlecode.com/svn/trunk/cppguide.html#Function_Names

C++ style
http://geosoft.no/development/cppstyle.html

http://stackoverflow.com/questions/1228161/why-use-prefixes-on-member-variables-in-c-classes

I use:

m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events
*/



/*
Constructing a BSP tree from scratch or recomputing parts of a tree is sufficiently
expensive that they are rarely built or modified at runtime. For this reason, BSP trees
are primarily used to hold static background geometry. Collision detection among
moving objects is usually handled through some other method.


Quad Trees
As the tree must be preallocated to a specific depth and cannot easily grow further,
the array representation is more suitable for static scenes and static octrees. The
pointer-based representation is more useful for dynamic scenes in which the octree
is constantly updated.


The biggest problem with using kd-trees, if I recall correctly, 
is that they are more difficult to insert/remove items from while maintaining balance. 
Therefore, I would recommend using one kd-tree for static objects such as houses and 
trees which is highly balanced, and another which contains players and vehicles, 
which needs balancing regularly. Find the nearest static object and 
the nearest mobile object, and compare those two.
http://gamedev.stackexchange.com/questions/14373/in-2d-how-do-i-efficiently-find-the-nearest-object-to-a-point

/*
Heh, well I think I am back to square one. Because the main actors in the game are:
- Players
- Fired Bullets
- Weapons (before they are picked up, i.e. placed in world => static )
- World level geometry


http://www.codercorner.com/SAP.pdf
- hased quad tree

- http://www.gamedev.net/page/resources/_/technical/game-programming/general-collision-detection-for-games-using-ell-r1026


http://forum.unity3d.com/threads/why-no-bsp-support.185410/

https://www.youtube.com/watch?v=yNYwZMmgTJk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=14


- Discussion Forum (it even has Chris Ericson's answer)
http://www.gamedev.net/topic/328022-what-collision-method-is-better/
http://www.gamedev.net/topic/565522-which-broadphase-collision-tech-should-i-use-/
https://www.reddit.com/r/gamedev/comments/1cssyn/sweep_and_prune_vs_quadtree_or_similar/


- kd tree

good for static object
keep dynamic objects in a separate tree
good for raycasting



- Sweep and prune

This broadphase has the best performance for typical dynamics worlds,
where most objects have little or no motion.

http://www.codercorner.com/SAP.pdf
http://www.toptal.com/game/video-game-physics-part-ii-collision-detection-for-solid-objects


- dynamic bounding tree 

It handles dynamic worlds where many objects are in motion,
and object addition and removal is faster than SAP.

http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Broadphase
http://www.randygaul.net/2013/08/06/dynamic-aabb-tree/
http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=4&t=9800
http://gamedev.stackexchange.com/questions/87625/sweep-and-prune-vs-quad-tree-when-all-objects-are-dynamic-moving
https://code.google.com/p/box2d/source/browse/trunk/Box2D/Box2D/Collision/b2DynamicTree.h
http://www.toptal.com/game/video-game-physics-part-ii-collision-detection-for-solid-objects



- fix the imported_model and texture index

*/



/*

-	UI
health bar
bullets
armor

overhead healthbar?


-	core
redo map		-		ice world
grenade 
gun pick up animation
throw gun animation



-	Audio



-	find models for guns



*/





/*
https://github.com/id-Software/DOOM-3

https://github.com/id-Software

http://fabiensanglard.net/

http://fabiensanglard.net/doom3/index.php

http://buildnewgames.com/real-time-multiplayer/

https://learn.goocreate.com/tutorials/create/multiplayer-fps/

https://www.reddit.com/r/Overwatch/comments/3u5kfg/everything_you_need_to_know_about_tick_rate/

*/

// FaceOff Vector


/*
pros

fixed size array (linear memory)
with internal free list (O(1) alloc/free, stable indicies)
with weak references keys (reuse of slot invalidates key)
zero overhead dereferences (when known-valid)
*/

/*
template<class T>
struct FOArray
{
	struct Item{
		T item;
		int id;
	}
	
	// http://stackoverflow.com/questions/14466620/c-template-specialization-calling-methods-on-types-that-could-be-pointers-or/14466705
	template<typename T>
	T * ptr(T & obj) { return &obj; } //turn reference into pointer!

	template<typename T>
	T * ptr(T * obj) { return obj; } //obj is already pointer, return it!
	

	vector<Item> objects;
	vector<int> freeList;

	int maxSize;
	int maxUsed;
	int count;

	void init(int maxSize)
	{
		this->maxSize = maxSize;
		objects.resize(maxSize);	

		for (int i = maxSize - 1; i >= 0; i--)
		{
			freeList.push_back(i);
		}

		maxUsed = 0;
		count = 0;
	}

	void clearFreeList()
	{
		freeList.clear();
	}

	int createObject()
	{
		if (!freeList.empty())
		{
			int free = freeList.back();
			freeList.pop_back();

			objects[free].id = free;
			objects[free].item = new T();

			count++;
			maxUsed = max(maxUsed, free);

			return free;
		}
		else
		{
			return -1;
		}
	}
	
	T getObject(int id)
	{
		return objects[id]->id == -1 ? NULL : objects[id].item;
	}

	// TODO: need to fix this, de-allocation is expensive
	void destroyObject(int id)
	{
		count--;
		freeList.push_back(id);

		
		delete getObject(id);
		objects[id] = NULL;
	}

	// only ever used on the client side
	// do not use this on the server side
	void set(T obj, int id)
	{
		if (getObject(id) != NULL)
		{
			destroyObject(id);
		}
		count++;
		maxUsed = max(maxUsed, id);
		objects[id] = obj;
	}

	int getIterationEnd()
	{
		return maxUsed + 1;
	}
};
*/

// slot map 32 bit
// upper 16 bit version
// lower 16 bit index

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};


// this class assumes T is pointer
template <class T>
struct FOArray
{
	struct Entry
	{
		ObjectId objId;
		T object;

		Entry()
		{
			object = NULL;
		}
	};


	vector<Entry> objects;
	vector<int> freeList;

	vector<T> toBeDestroyed;


	// i'm using this to start iterating for situations where I want to skip the static objects
	// i am making the assumption that all my static objects are put in my array first
	int preferredIterationStart;
	int maxSize;
	int maxUsed;
	int count;

	// getting only the number of dynamic objects;
	int getPreferredCount()
	{
		return maxSize - preferredIterationStart;
	}

	void init(int maxSize)
	{
		this->maxSize = maxSize;
		objects.resize(maxSize);

		for (int i = maxSize - 1; i >= 0; i--)
		{
			freeList.push_back(i);
		}

		for (int i = 0; i < maxSize; i++)
		{
			objects[i].objId.setTag(0);
			objects[i].objId.setIndex(i);
		}

		maxUsed = 0;
		count = 0;
	}

	T getByIndex(int index)
	{
		return objects[index].object;
	}

	T get(ObjectId objId)
	{
		//		return objects[id];
		//		return objects[id]->indexId == -1 ? NULL : objects[id];
		return objects[objId.getIndex()].object;
	}

	void clearFreeList()
	{
		freeList.clear();
	}

	ObjectId create()
	{
		if (!freeList.empty())
		{
			int free = freeList.back();
			freeList.pop_back();

			ObjectId objId = objects[free].objId;
			// the new version + index is managed by destroyedObject, so you don't have to
			// change the id here.
			objects[free].object = new WorldObject();
			objects[free].object->objectId = objId;

			count++;
			maxUsed = max(maxUsed, free);

			return objId;
		}
		else
		{
			ObjectId objId;
			objId.setId(INVALID_OBJECT_ID);
			return objId;
		}
	}


	ObjectId add(T obj)
	{
		if (!freeList.empty())
		{
			int free = freeList.back();
			freeList.pop_back();

			ObjectId objId = objects[free].objId;



			// the new version + index is managed by destroyedObject, so you don't have to
			// change the id here.
			objects[free].object = obj;
			objects[free].object->objectId = objId;

		//	cout << obj->m_name << "	tag is " << obj->objectId.getTag() << endl;
		//	cout << obj->m_name << "	index is " << obj->objectId.getIndex() << endl;

			count++;
			maxUsed = max(maxUsed, free);

			return objId;
		}
		else
		{
			ObjectId objId(ObjectId::INVALID_OBJECT_ID);
			return objId;
		}
	}

	void addToDestroyList(ObjectId objId)
	{
		toBeDestroyed.push_back(get(objId));
	}

	void destroyObjects()
	{
		for (int i = 0; i < toBeDestroyed.size(); i++)
		{
			WorldObject* obj = toBeDestroyed[i];
			destroy(obj->objectId);
		}

		toBeDestroyed.clear();
	}

	// TODO: need to fix this, de-allocation is expensive
	void destroy(ObjectId objId)
	{
		delete get(objId);

		int tag = objId.getTag();
		int index = objId.getIndex();

		objects[index].objId.setTag(tag + 1);
		objects[index].object = NULL;

		count--;

		freeList.push_back(index);
	}

	//	ObjectId nextAvaiableIndexId()
	ObjectId nextAvailableId()
	{
		if (!freeList.empty())
		{
			int free = freeList.back();
			return objects[free].objId;
		}
		else
		{
			ObjectId objId(ObjectId::INVALID_OBJECT_ID);
			return objId;
		}
	}

	// only ever used on the client side
	// do not use this on the server side
	// becuz the client objects list is only ever changed by snapshot updates from the server
	// therefore we do not have to worry about freeList
	void set(T obj, ObjectId objId)
	{
		int index = objId.getIndex();

		//		WorldObject* obj1 = objects[index].object;
		// cout << "		## index is " << index << endl;
		// cout << "		## new object is " << obj->m_name << endl;

		if (objects[index].object != NULL)
		{
			//	cout << "		## i want to delete " << (objects[index].object->m_name) << endl;
			delete objects[index].object;
		}
		else
		{
			// should remove index from freeList
			// but this is only used by the client
			// so we don't have to care about that
			count++;
		}

		objects[index].objId = objId;
		objects[index].object = obj;

		maxUsed = max(maxUsed, index);
	}

	int getIterationEnd()
	{
		return maxUsed + 1;
	}

	void debugIds()
	{
		for (int i = 0; i < maxUsed; i++)
		{
			cout << i << endl;
			T* obj = objects[i].object;
			if (obj != NULL)
			{
				cout << obj->m_name << "	tag is " << obj->objectId.getTag() << endl;
				cout << obj->m_name << "	index is " << obj->objectId.getIndex() << endl;
			}
			cout << endl;
		}

	}

};


struct DelayedPacket
{
	unsigned int deliveryTime;
	RakNet::SystemAddress address;
	RakNet::BitStream bs;
//	unsigned char* data;

	// http://www.geeksforgeeks.org/copy-constructor-vs-assignment-operator-in-c/
	// need to define copy constructer and assignment operator becuz RakNet does not allow BitStream copy by value
	
	DelayedPacket()
	{
		bs.Reset();
	}
	
	DelayedPacket(const DelayedPacket& dp)
	{
		this->deliveryTime = dp.deliveryTime;
		this->address = dp.address;
		this->bs.Reset();
		this->bs.Write(((RakNet::BitStream*)(&dp.bs)));
	}

	DelayedPacket& operator = (const DelayedPacket& dp)
	{
		this->deliveryTime = dp.deliveryTime;
		this->address = dp.address;
		this->bs.Reset();
		this->bs.Write(((RakNet::BitStream*)(&dp.bs)));
		return *this;
	}
	

	DelayedPacket(unsigned int time, RakNet::SystemAddress Address, RakNet::BitStream& Bs)
	{
		this->deliveryTime = time;
		this->address = Address;
		this->bs.Reset();
		this->bs.Write(Bs);		
	}
};

const int TIME_PROFILER_BUFFER = 10;
const int FPS_PROFILER_BUFFER = 20;

typedef std::queue<DelayedPacket> PacketQueue;

class FaceOff
{
	// private:
	public:
		static RendererManager		m_rendererMgr;
		static ModelManager			m_modelMgr;
		NetworkManager				m_nm;
		Renderer*					p_renderer;

		/// GUI
		long long m_runningTime;
		Uint32 m_nextGameTick = 0;
		MouseState m_mouseState;

		Pipeline m_pipeline;

		DelayedPacket myDP;

		float m_fps;
		float m_iterRefreshRate;
		float m_curIter;
		unsigned int m_frameCount;
		unsigned int m_frameTicks[FRAME_VALUES];
		unsigned int m_frameTicksIndex;
		unsigned int m_prevFrameTick;

		bool isRunning;

		// float FIXED_UPDATE_TIME;

		FirstPersonCamera m_spectatorCamera;

		// lights
		LightManager m_lightManager;

		// models
		Model*          p_model;
		Weapon* mainWeaponPtr;
		// objects
		Terrain o_terrain;
		MultiTextureTerrain o_multiTextureTerrain;

		BillboardList o_grassPatch;
		BillboardList o_flowerPatch;

		WorldObject		o_tree;
		WorldObject		o_lowPolyTree;

		bool containedFlag;

		WorldObject		o_cubeWireFrame;
		WorldObject     o_worldAxis;
		WorldObject     o_ground;
		SkyBox          o_skybox;
		WorldObject		o_sampleBullet;

		int debugCurClientId;

		list<Particle> m_bullets;
		queue<int> m_objectIndexPool;

//		int m_defaultPlayerID;
		ObjectId m_defaultPlayerObjectId;
		int getDefaultPlayerId();

		void initMap(FOArray<WorldObject*>& objects, FOArray<Player*>& players, KDTree& tree);



		GUIManager m_gui;

//		bool SINGLE_PLAYER_MODE;

		bool m_zoomedIn;
		float m_zoomFactor;
	public:

		KDTreeNode* hitNode;
		long long m_currentTimeMillis;
		bool singlePlayerMode;


		RakNet::Packet* sv_packet;
		RakNet::Packet* cl_packet;

		// RakNet::BitStream bsOut;

		// RakNet::RakString rs;
		// RakNet::SystemAddress server_address;


		vector<WorldObject*> objectsAlreadyTestedForFire;

		void destroyWorldObjectByIndex(vector<WorldObject*>& objects, int i);
		// void destroyWorldObjectByIndex(int i);
		// vector<WorldObject*> m_objects;

		// keeping two copies
		// vector<WorldObject*> sv_objects;	
		// vector<WorldObject*> cl_objects;	// used for client parsing server snapshot and rendering

		int timeProfilerIndex;
		long long timeProfiler[TIME_PROFILER_BUFFER];
//		uint64 timeProfiler[TIME_PROFILER_BUFFER];

		int fpsProfilerIndex;
		int fpsProfiler[FPS_PROFILER_BUFFER];

		bool interpolateFlag;
		bool predictionOn;

		bool serverRunPhysicsOnReadPacketsFlag;

		
		FOArray<WorldObject*> sv_objects;
		FOArray<WorldObject*> cl_objects;	// this never creates, only sets objects

//		FOPlayerArray sv_players;
//		FOPlayerArray cl_players;

		FOArray<Player*> sv_players;
		FOArray<Player*> cl_players;

		KDTree sv_objectKDtree;
		KDTree cl_objectKDtree;

//		queue<int> sv_emptyBucketPool;

		vector<FireWorkEffect*> m_fireWorkEffects;
		vector<SmokeEffect*> m_smokeEffects;

		// used for both server or client
		// the queue that server handles input from the client
		
		// server uses this to receive client inputs in the network thread

		// client uses this to collect inputs in each frame

		// int lastObjectIndex;
		// void addObject(vector<WorldObject*> objects, WorldObject* object);

		// vector<WorldObject*> m_hitPointMarks;
		// vector<FireWorkEffect*> m_fireWorkEffects;

	//	CollisionDetectionTestPairs collisionDetectionTestPairs;
	//	CollisionDetectionTestPairs clientInputCollisionDetectionTestPairs;


		// networking portion
		Client m_client;	// for the main player
		Server m_server;

		FaceOff();
		FaceOff(int nice);
		~FaceOff();

		/// init functions
		void init();
		void initObjects();
		// void lateInitObjects();
		void initRenderers();
		void initGUI();
		void initAudio();
		
		int endWithError(char* msg, int error = 0);

		void handleDeviceEvents();
		void serverHandleDeviceEvents();
		void clientHandleDeviceEvents();

		void initNetwork();
		void initNetworkLobby();
		void startNetworkThread();

		void RakNetFunction();

		void deserializePlayerAndWeaponAndAddToWorld(Player* p, RakNet::BitStream& bs, bool curPlayerFlag);
		void deserializeEntityAndAddToWorld(WorldObject* obj, RakNet::BitStream& bs);

		void replyBackToNewClient(ObjectId playerId, RakNet::BitStream& bs);
		void broadCastNewClientJoining(ObjectId playerId, RakNet::BitStream& bs);

		void start();
		void update();

		void initMap();

		int getAverageFPS();

		void interpolateEntities();

		void serverFrame(long long dt);
		void clientFrame(long long dt);

		void serverReadPackets();
		void clientReadPackets();

		void serverCalculatePing();
		void serverCheckTimeouts();

		void serverSimulationTick(int msec);
		void clientSimulationTick();

//		void serverRunClientMoveCmd(int clientId, UserCmd cmd);
		void serverRunClientMoveCmd(ObjectId playerId, UserCmd cmd);

		void serverSendClientMessages();

		void serverSendClientSnapshot(int clientId);
		void serverBuildClientSnapshot(Snapshot* from, Snapshot* to, RakNet::BitStream& bs);
		
		void serverWritePlayers(int clientId, Snapshot* from, Snapshot* to, RakNet::BitStream& bs);
		void serverWriteDefaultPlayer(Player* p, RakNet::BitStream& bs);
		void serverWriteOtherPlayer(Player* p, RakNet::BitStream& bs);

		void serverWriteEntities(int clientId, Snapshot* from, Snapshot* to, RakNet::BitStream& bs);
		void serverWriteNewWorldObject(WorldObjectState* obj1, RakNet::BitStream& bs);
		void serverWriteDeltaWorldObject(WorldObjectState* obj0, WorldObjectState* obj1, RakNet::BitStream& bs, bool force);
		void serverWriteRemoveWorldObject(WorldObjectState* obj0, RakNet::BitStream& bs);

		void serverConstructSnapshotToClient();


		void clientParseSnapshot(RakNet::BitStream& bs);
		void clientParsePlayers(ClientSnapshot* prev, ClientSnapshot* cur, RakNet::BitStream& bs);
		void clientParsePlayer(ClientSnapshot* cur, int flags, RakNet::BitStream& bs);
		void clientParseDefaultPlayer(ClientSnapshot* cur, int flags, ObjectId playerId, RakNet::BitStream& bs);
		void clientParseOtherPlayer(ClientSnapshot* cur, int flags, ObjectId playerId, RakNet::BitStream& bs);

		void clientParseEntities(ClientSnapshot* prev, ClientSnapshot* cur, RakNet::BitStream& bs);
		void clientParseDeltaEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs);
		void clientParseEntityData(WorldObject* obj, int flags, RakNet::BitStream& bs);
		void clientParseAddEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs);
		void clientParseRemoveEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs);

		/*
		1	CLIENT_INPUT						bsOut.Write((RakNet::MessageID)CLIENT_INPUT);
		4	sequence number						bsOut.Write(m_client.netchan.outgoingSequence);
		4	m_defaultPlayerID					bsOut.Write(m_defaultPlayerID);
		4	m_client.lastServerMsgSequence		bsOut.Write(m_client.lastServerMsgSequence);
		1	m_client.cmdNum						bsOut.Write(m_client.cmdNum);
		?	cmd									cmd.serialize(bsOut);
		*/
		void clientSendCmd();
		void clientSendPacket(RakNet::BitStream& bs);

		UserCmd clientCreateNewCmd();
		void clientCheckForResend();

		void clientPrediction();

		uint8_t svFireWeaponCollisionFlags[ENTITY_COLLISION_FLAG_SIZE];
		void processUserFireWeapon(Player* p);
		void render();


//		void simulatePlayerPhysics(KDTree& tree, Player* p, int i, bool setCollsionFlagsBothWays);
//		void simulateObjectPhysics(KDTree& tree, FOArray<WorldObject*>& objects, WorldObject* object, int i, bool setCollsionFlagsBothWays);
	
		void simulatePlayerPhysics(KDTree& tree, Player* p, bool setCollsionFlagsBothWays);
		void simulateObjectPhysics(KDTree& tree, FOArray<WorldObject*>& objects, WorldObject* object, bool setCollsionFlagsBothWays);
		void checkNeighbors(KDTree& tree, WorldObject* obj, bool setCollsionFlagsBothWays);

		void clearCollisionDetectionFlags();
		bool testCollisionDetection(WorldObject* a, WorldObject* b, ContactData& contactData);
		
		void GetTimeProfilerAverages();



		void renderGUI();

		void startCB();
		void resetGameBoardCB();
		
	//	unsigned int serverAbsoluteTime;
	//	unsigned int clientAbsoluteTime;

//		Uint32 serverAbsoluteTime;	// absoluteTime
//		Uint32 clientAbsoluteTime;	// absoluteTime

//		Uint32 serverRealTime;	// absoluteTime
//		Uint32 clientRealTime;	// absoluteTime


		vector<int> latencyOptions;		// round trip
		int latency;					// rount trip latency in milliseconds
		int curLatencyOption;

		vector<float> packetLossOptions;
		float packetLoss;	// percentage of packets lost
		int curPacketLossOption;

		

		// this is used to simulate packet loss and packet delay on a single player

		PacketQueue clientToServer;
		PacketQueue serverToClient;
		
		void clientToServerSendPacket(RakNet::SystemAddress& address, RakNet::BitStream& bs);
		void serverToClientSendPacket(RakNet::SystemAddress& address, RakNet::BitStream& bs);
		
		void processPacketQueue(unsigned int curAbsoluteTime, PacketQueue& packetQueue, bool isServerToClient);

		long long getCurrentTimeMillis();

		int noneCounter;


		// used for collision detection
		vector<WorldObject*> neighbors;
};

/*
thread m_networkThread;

// void clientNetworkThread();
// void serverNetworkThread();
// MoveQueue m_inputQueue;

// queue<Move> m_clientInputQueue;
// mutex m_clientInputMutex;

*/

#endif