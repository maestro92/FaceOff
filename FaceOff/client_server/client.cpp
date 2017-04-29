#include "client.h"


Client::Client()
{
	isConnected = false;
	peer = NULL;
	curSnapshot = NULL;

}

Client::Client(RakNet::RakNetGUID guid)
{
	m_guid = guid;
	isConnected = false;
	peer = NULL;
	curSnapshot = NULL;
}

Client::~Client()
{
	if (peer != NULL)
	{
		RakNet::RakPeerInterface::DestroyInstance(peer);
	}
}

void Client::init(int cmdSampleRate)
{
	peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	char str[512];

	utl::clDebug("Starting the client.");
	utl::clDebug("Enter server IP or hit enter for 127.0.0.1");
	gets(str);
	if (str[0] == 0)
	{
		strcpy(str, "127.0.0.1");
	}


	peer->Connect(str, SERVER_PORT, 0, 0);
	curSnapshot = NULL;

	cmdRate = 60;
	CL_FIXED_CMD_SAMPLE_TIME_ms = 1000 / cmdRate;
//	CL_FIXED_CMD_SAMPLE_TIME_ms = CL_FIXED_CMD_SAMPLE_TIME_s * 1000;

	cout << "cmdRate " << cmdRate << endl;
//	cout << "CL_FIXED_CMD_SAMPLE_TIME_s " << CL_FIXED_CMD_SAMPLE_TIME_s << endl;
	cout << "CL_FIXED_CMD_SAMPLE_TIME_ms " << CL_FIXED_CMD_SAMPLE_TIME_ms << endl;

	lastServerMsgSequence = -1;
	lastAckCmdNum = -1;
	/*
	peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	char str[512];

	printf("Enter server IP or hit enter for 127.0.0.1\n");
	gets(str);
	if (str[0] == 0)
		strcpy(str, "127.0.0.1");

	printf("Starting the client.\n");
	peer->Connect(str, SERVER_PORT, 0, 0);
	*/
}


bool Client::timeToSampleUserCmd(long long nowTime_ms)
{
	if (nowTime_ms - lastSampleTime > CL_FIXED_CMD_SAMPLE_TIME_ms)
	{
		return true;
	}
	return false;
}

/*
void NetworkClient::run()
{

	bool connected = false;
	RakNet::RakString rakString;
	int int_message;
	RakNet::BitStream bsOut;

	RakNet::Packet* packet;


	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{

			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(int_message);

			switch (packet->data[0])
			{
				case NEW_CLIENT:
					std::cout << "Server said I'm client number " << int_message << std::endl;
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					connected = true;
					break;
				case PLAYER_UPDATE:
					// report the server's new counter value
					std::cout << "Server said we are now at " << int_message << std::endl;
					break;
				case YOUR_TURN:
					printf("My Turn. Sending message.\n");
					bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);


}
*/
