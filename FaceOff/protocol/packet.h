#ifndef PROTOCOL_PACKET_H_
#define PROTOCOL_PACKET_H_

#include "network/address.h"
#include "protocol/object.h"



/*

layers:

network layer
protocol layer
application layer


so protocol uses network

*/

namespace protocol
{
	// forward declaration
	class PacketFactory;

	class Packet : public Object
	{
		network::Address address;
		int type;

		public:
			Packet(int _type) : type(_type) 
			{}

			int getType() const
			{
				return type;
			}

			void setAddress(const network::Address& _address)
			{
				address = _address;
			}

			const network::Address& getAddress() const
			{
				return address;
			}
	
		protected:

			virtual ~Packet()
			{}

			// The friend declaration appears in a class body and grants a function 
			// or another class access to private and protected members of the class where the friend declaration appears.
			friend class PacketFactory;

		private:
			Packet(const Packet& other);
			Packet& operator = (const Packet& other);
	};
}

#endif