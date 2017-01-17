
#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

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
// #include "network_info.h"

#include "utility"
#include "player.h"
#include "weapon.h"
#include "model.h"
#include "model_manager.h"

using namespace std;





// http://classes.cs.kent.edu/gpg/trac/browser/EnginesS11/cmarshal/RakNet_POC
// http://classes.cs.kent.edu/gpg/trac/wiki/cmarshall

class NetworkManager
{
	public:
		
		void init(ModelManager* mm, vector<WorldObject*>* objects, vector<Player*>* players);
		Player* spawnClientPlayer(RakNet::BitStream& bs, bool defaultFlag);



	private:
		ModelManager* m_modelMgr;
		vector<WorldObject*>* m_objects;
		vector<Player*>* m_players;
};

#endif
