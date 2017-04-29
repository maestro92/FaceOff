#ifndef NETWORK_ADDRESS_H_
#define NETWORK_ADDRESS_H_

#include <cstdint>
// http://stackoverflow.com/questions/13642827/cstdint-vs-stdint-h
#include "network/Enums.h"

// forward declarations?
struct addrinfo;
struct sockaddr_in6;
struct sockaddr_storage;

namespace network
{
	class Address
	{
		private:
			AddressType m_type;

			// recall that ipv4 is like 192.168.1.1
			// ipv6 is like   3ffe:1900:4545:3:200:f8ff:fe21:67cf
			// so we use a uint16_t to represent each segment in the ipv6
			// recall that two hex number is one byte
			// so using uint16_t to represent 4 hex number is just right
			union
			{
				std::uint32_t m_address4;
				std::uint16_t m_address6[8];
			};

			std::uint16_t m_port;

		public:

			Address();
			
			Address(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d, uint16_t port = 0);

			explicit Address(std::uint32_t address, std::int16_t port = 0);

			explicit Address(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d,
			   				 std::uint16_t e, std::uint16_t f, std::uint16_t g, std::uint16_t h, std::uint16_t port = 0);

			explicit Address(const std::uint16_t address[], std::uint16_t port = 0);

			explicit Address(const sockaddr_storage& addr);

			explicit Address(const sockaddr_in6& addr_ipv6);

			explicit Address(addrinfo* p);

			explicit Address(const char* address);
			
			explicit Address(const char* address, std::uint16_t port);

			void clear();

			std::uint32_t getAddress4() const;

			const std::uint16_t* getAddress6() const;

			void setPort(std::uint64_t port);

			std::uint16_t getPort() const;

			AddressType getType() const;

			const char* toString(char buffer[], int bufferSize) const;

			bool isValid() const;

			bool operator ==(const Address& other) const;

			bool operator !=(const Address& other) const;

		protected:
			
			void parse(const char* address);
	};

}


#endif // ! NETWORK_ADDRESS_H_
