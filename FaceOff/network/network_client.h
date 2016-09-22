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


class NetworkClient
{
	public:
		NetworkClient();
		~NetworkClient();

		void init();
		void run();

		RakNet::RakPeerInterface* peer;
		bool connected;
};

#endif