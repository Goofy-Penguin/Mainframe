#pragma once

#include <string>
#include <vector>

#include <sstream>
#include <vector>
#include <fcntl.h>
#include <ctype.h>
#include <cinttypes>

#include <mainframe/networking/enum/errors.h>

#ifdef _MSC_VER
#define _WINSOCKAPI_
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#endif

namespace mainframe {
	namespace networking {
		class Socket {
		public:
			enum class SockState {
				skDISCONNECTED = 0,
				skUNDEF1, //Not implemented
				skLISTENING,
				skUNDEF3, //Not implemented
				skUNDEF4, //Not implemented
				skUNDEF5, //Not implemented
				skUNDEF6, //Not implemented
				skCONNECTED,
				skERROR
			};

			bool blocking = true;
			bool ipv6 = false;

			sockaddr_in addr;
			sockaddr_in fromAddr;
			sockaddr_in6 addr6;
			sockaddr_in6 fromAddr6;
			unsigned long fromAddr_len = 0;

			SockState state = SockState::skDISCONNECTED;
			int sock = -1;

			int lastCode = 0;

			Socket(bool ipv6 = false);
			~Socket();

			bool create();
			bool create(int Protocol);
			bool create(int Protocol, int Type);
			bool bind(unsigned short port);
			bool listen();
			bool accept(Socket* socket);
			[[nodiscard]] SocketError connect(const std::string& host, unsigned short port);
			void close();

			uint64_t uAddr();
			bool isError();

			std::string getIpAddress();

			bool canRead();

			int receive(unsigned char* buffer, int size, int spos = 0);
			bool receiveAll(unsigned char* buffer, int size, int spos = 0);
			int send(const unsigned char* data, int dataSize);
			bool sendAll(const unsigned char* data, int dataSize);
			int sendUDP(const unsigned char* buffer, int size, sockaddr_in* to);
			int receiveUDP(unsigned char* buffer, int size, sockaddr_in* from);
			void setTimeout(int miliseconds);

		private:
#ifdef _MSC_VER
			WSADATA wsda;
#endif
			int MAXCON;

			fd_set scks;
			timeval times;

			unsigned int totaldata;
			bool check();
		};
	}
}