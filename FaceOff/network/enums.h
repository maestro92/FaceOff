#ifndef NETWORK_ENUMS_H_
#define NETWORK_ENUMS_H_


namespace network
{
	enum AddressType
	{
		ADDRESS_UNDEFINED,
		ADDRESS_IPV4,
		ADDRESS_IPV6
	};

	// BSD socket is just a set of API on sockets from the 1983
	// now it's considered the standard for internet socket APIs
	// POSIX is based o nnit
	enum BSDSocketError
	{
		BSD_SOCKET_ERROR_NONE = 0,
		BSD_SOCKET_ERROR_CREATE_FAILED,

		// SOCKOPT means socket options
		// example: The setsockopt() function shall set the option specified by the option_name argument,
		// http://pubs.opengroup.org/onlinepubs/009695399/functions/setsockopt.html
		BSD_SOCKET_ERROR_SOCKOPT_IPV6_ONLY_FAILED,
		BSD_SOCKET_ERROR_BIND_IPV6_FAILED,
		BSD_SOCKET_ERROR_BIND_IPV4_FAILED,
		BSD_SOCKET_ERROR_SET_NON_BLOCKING_FAILED
	};

	enum BSDSocketCounter
	{
		BSD_SOCKET_COUNTER_PACKETS_SENT,
		BSD_SOCKET_COUNTER_PACKETS_RECEIVED,
		BSD_SOCKET_COUNTER_SEND_FAILURES,
		BSD_SOCKET_COUNTER_SERIALIZE_WRITE_OVERFLOW,
		BSD_SOCKET_COUNTER_SERIALIZE_READ_OVERFLOW,
		BSD_SOCKET_COUNTER_PACKET_TOO_LARGE_TO_SEND,
		BSD_SOCKET_COUNTER_CREATE_PACKET_FAILTURES,
		BSD_SOCKET_COUNTER_PROTOCOL_ID_MISMATCH,
		BSD_SOCKET_COUNTER_ABORTED_PACKET_READS,
		BSD_SOCKET_COUNTER_NUM_COUNTERS
	};
}

#endif