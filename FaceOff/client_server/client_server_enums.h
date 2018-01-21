#ifndef CLIENT_SERVER_ENUMS_H_
#define CLIENT_SERVER_ENUMS_H_


namespace clientServer
{

	enum ClientState
	{
		CLIENT_STATE_DISCONNECTED,
		CLIENT_STATE_RESOLVING_HOSTNAME,
		CLIENT_STATE_SENDING_CONNECTION_REQUEST,
		CLIENT_STATE_SENDING_CHALLENGE_RESPONSE,
		CLIENT_STATE_SENDING_CLIENT_DATA,
		CLIENT_STATE_READY_FOR_CONNECTION,
		CLIENT_STATE_CONNECTED,
		CLIENT_STATE_COUNT
	};

	inline const char* GetClientStateName(int clientState)
	{
		switch (clientState)
		{
			case CLIENT_STATE_DISCONNECTED:		return "DISCONNECTED";

			case CLIENT_STATE_CONNECTED:		return "CONNECTED";
			default:
				// cout << "error client state" << endl;
				break;
		}
	}





	enum ServerClientState
	{
		SERVER_CLIENT_STATE_DISCONNECTED,		// client is disconnected. default state.
		SERVER_CLIENT_STATE_CONNECTED			// client is fully connected. connection packets are now exchanged
	};
}

#endif