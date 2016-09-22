#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_


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

using namespace std;



class NetworkServer
{

	public:
		NetworkServer();
		~NetworkServer();

		void init();

		void run();

		RakNet::RakPeerInterface* peer;
//		vector<Client> clients;
};



#endif



