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

#include "game_messages.h"

#include "collision_detection/contact_data.h"
#include "collision_detection/collision_detection.h"

#include "collision_detection/kd_tree.h"

#include "terrain/terrain.h"
#include "terrain/multitexture_terrain.h"

#include "network/network_manager.h"

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
*/





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

		

		float m_fps;
		float m_iterRefreshRate;
		float m_curIter;
		unsigned int m_frameCount;
		unsigned int m_frameTicks[FRAME_VALUES];
		unsigned int m_frameTicksIndex;
		unsigned int m_prevFrameTick;

		bool isRunning;

		FirstPersonCamera m_serverCamera;

		// lights
		LightManager m_lightManager;

		// models
		Model*          p_model;

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



		list<Particle> m_bullets;
		queue<int> m_objectIndexPool;
	//	list<WorldObject*> m_dynamicObjects;
	
		int m_defaultPlayerID;

		vector<Player*> m_players;

//		GLuint tempTexture;

		GUIManager m_gui;

		KDTree m_objectKDtree;

		bool m_zoomedIn;
		float m_zoomFactor;
	public:

		KDTreeNode* hitNode;
		long long m_currentTimeMillis;

		bool m_isServer;
		RakNet::RakPeerInterface* peer;
		bool connected;


		RakNet::Packet* packet;
		RakNet::RakString rs;
		RakNet::SystemAddress server_address;


		RakNet::BitStream bsOut;


		void destroyWorldObjectByIndex(int i);
		vector<WorldObject*> m_objects;
		queue<int> m_emptyBucketPool;

		vector<FireWorkEffect*> m_fireWorkEffects;
		vector<SmokeEffect*> m_smokeEffects;

		// used for both server or client
		// the queue that server handles input from the client
		
		// server uses this to receive client inputs in the network thread

		// client uses this to collect inputs in each frame

		MoveQueue m_inputQueue;

		// queue<Move> m_clientInputQueue;
		// mutex m_clientInputMutex;


		// vector<WorldObject*> m_hitPointMarks;
		// vector<FireWorkEffect*> m_fireWorkEffects;

		CollisionDetectionTestPairs collisionDetectionTestPairs;
		CollisionDetectionTestPairs clientInputCollisionDetectionTestPairs;

		FaceOff();
		~FaceOff();

		/// init functions
		void init();
		void initObjects();
		void initRenderers();
		void initGUI();
		void initAudio();
		
		int endWithError(char* msg, int error = 0);

		void handleDeviceEvents();
		void serverHandleDeviceEvents();
		void clientHandleDeviceEvents();

		void initNetworkServerClient();
		void initNetworkLobby();
		void startNetworkThread();

		void clientNetworkThread();
		void serverNetworkThread();



		void start();
		void update();

		void serverFrame();
		void clientFrame();


		void serverSimulation();
		void clientSimulation();

		void simulatePlayerPhysics(Player* p, int i);
		void simulatePlayerPhysics(Player* p, int i, Move move);
		void simulateObjectPhysics(WorldObject* obj, int i);
		void simulatePhysics();
		bool testCollisionDetection(WorldObject* a, WorldObject* b, ContactData& contactData);
		void checkNeighbors(WorldObject* obj);
		void checkNeighborsAfterClientInput(WorldObject* obj);

		void render();


		void renderGUI();

		void startCB();
		void resetGameBoardCB();

		thread m_networkThread;

		long long getCurrentTimeMillis();

};

#endif