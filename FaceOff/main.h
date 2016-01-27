#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"

#include "define.h"
#include "utility.h"

#include <GL/glew.h>


#include "gol_model_manager.h"
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
#include "player.h"

#include "light\light_manager.h"
#include "first_person_camera.h"

#include "quad_model.h"
#include "xyz_axis_model.h"
#include "world_object.h"
#include "particle.h"
#include "skybox.h"
#include "billboard_list.h"
#define FRAME_VALUES 10
#include <list>

#include <iostream>
#include <chrono>
#include <thread>

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "game_messages.h"
#include "network_info.h"

#include "terrain/terrain.h"
#include "terrain/multitexture_terrain.h"
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





*/


class FaceOff
{
	// private:
	public:
		RendererManager              m_rm;
		Renderer*                    p_renderer;

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

		FirstPersonCamera m_firstPersonCamera;

		// lights
		LightManager m_lightManager;

		// models
		Model*          p_model;
		ImportedModel   m_groundModel;
		XYZAxisModel    m_xyzModel;
		ImportedModel   m_gunModel;
		ImportedModel	m_bulletModel;
		ImportedModel	m_tree;
		ImportedModel	m_lowPolyTree;
		ImportedModel	m_stairs;
		ImportedModel	m_woodenBox;


		// objects
		Terrain o_terrain;
		MultiTextureTerrain o_multiTextureTerrain;

		BillboardList o_grassPatch;
		BillboardList o_flowerPatch;

		WorldObject		o_tree;
		WorldObject		o_lowPolyTree;



		WorldObject		o_cubeWireFrame;
		WorldObject     o_worldAxis;
		WorldObject     o_ground;
		WorldObject     o_gun;
		SkyBox          o_skybox;
		WorldObject		o_sampleBullet;

		list<Particle> m_bullets;

		int m_defaultPlayerID;

		vector<Player*> m_players;

		GLuint tempTexture;

		GUIManager m_gui;
		GOLModelManager m_GOLModelManager;
	public:

		Weapon* weap;

		Player p;

		bool m_isServer;
		RakNet::RakPeerInterface* peer;
		bool connected;


		RakNet::Packet* packet;
		RakNet::RakString rs;
		RakNet::SystemAddress server_address;

		int int_message;
		RakNet::BitStream bsOut;


		vector<WorldObject*> m_objects;

		FaceOff();
		~FaceOff();

		/// init functions
		void initObjects();
		void initModels();
		void initRenderers();
		void initGUI();
		void initNetwork();
		void initLobby();

		void start();
		void update();

		void forwardRender();
		void renderScene();

		void renderGUI();

		void startCB();
		void resetGameBoardCB();
		void GOLModelListBoxCB();



		thread m_networkThread;

		void clientNetworkThread();
		void serverNetworkThread();

};

#endif