#include <mainframe/networking/socket.h>

#include <iostream>


#ifdef _MSC_VER
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"wsock32.lib")
#else

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#endif

#if !defined(SOCKET_ERROR)
#define SOCKET_ERROR (-1)
#endif

#if !defined(SOCKET_NONE)
#define SOCKET_NONE (0)
#endif

#if !defined(INVALID_SOCKET)
#define INVALID_SOCKET (-1)
#endif

namespace mainframe {
	namespace networking {
		Socket::Socket() {
			MAXCON = 64;
			memset(&addr, 0, sizeof(addr));

#ifdef _MSC_VER
			WSAStartup(MAKEWORD(1, 1), &wsda);
#endif

			this->times.tv_sec = 0;
			this->times.tv_usec = 0;
		}

		Socket::~Socket() {
			if (this->check())
				close();
		}

		bool Socket::check() {
			return sock > SOCKET_NONE;
		}

		bool Socket::create() {
			return this->create(IPPROTO_TCP, SOCK_STREAM);
		}

		bool Socket::create(int Protocol) {
			switch (Protocol) {
				case IPPROTO_TCP: return this->create(IPPROTO_TCP, SOCK_STREAM);
				case IPPROTO_UDP: return this->create(IPPROTO_UDP, SOCK_DGRAM);
				default:          return this->create(Protocol, SOCK_RAW);
			}
		}

		bool Socket::create(int Protocol, int Type) {
			if (this->check())
				return false;

			state = SockState::skDISCONNECTED;
			sock = static_cast<int>(::socket(AF_INET, Type, Protocol));
			lastCode = sock;

			return sock > SOCKET_NONE;
		}

		bool Socket::bind(unsigned short port) {
			if (!check()) return false;

			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(port);
			lastCode = ::bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

			return this->lastCode > 0 ? false : true;
		}

		bool Socket::listen() {
			lastCode = ::listen(sock, MAXCON);
			if (lastCode == SOCKET_ERROR) return false;

			state = SockState::skLISTENING;
			return true;
		}

		bool Socket::accept(Socket* socket) {
			if (!blocking && !canRead()) return false;

#ifdef _MSC_VER
			int length = sizeof(socket->addr);
#else
			auto length = static_cast<socklen_t>(sizeof(socket->addr));
#endif
			socket->sock = static_cast<int>(::accept(sock, reinterpret_cast<struct sockaddr*>(&socket->addr), &length));

			lastCode = socket->sock;
			if (socket->sock == SOCKET_ERROR)
				return false;

			socket->state = SockState::skCONNECTED;
			return true;
		}

		void Socket::close() {
			state = SockState::skDISCONNECTED;

#ifdef _MSC_VER
			::closesocket(sock);
#else
			::shutdown(sock, SHUT_RDWR);
			::close(sock);
#endif

			sock = static_cast<int>(INVALID_SOCKET);
		}

		uint64_t Socket::uAddr() {
			return addr.sin_addr.s_addr;
		}

		SocketError Socket::connect(const std::string& host, unsigned short port) {
			if (!check())
				return SocketError::invalidSocket;

#pragma warning( push )
#pragma warning( disable: 4996)
			hostent* phe = gethostbyname(host.c_str());
#pragma warning( pop )

			if (phe == nullptr)
				return SocketError::invalidHostname;

			memcpy(&addr.sin_addr, phe->h_addr, sizeof(struct in_addr));

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			if (::connect(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
				return SocketError::failed;

			state = SockState::skCONNECTED;
			return SocketError::success;
		}

		bool Socket::canRead() {
			FD_ZERO(&scks);
			FD_SET(static_cast<unsigned>(sock), &scks);

			return select(sock + 1, &scks, nullptr, nullptr, &times) > 0;
		}

		bool Socket::isError() {
			if (state == SockState::skERROR || sock == -1)
				return true;

			FD_ZERO(&scks);
			FD_SET(static_cast<unsigned>(sock), &scks);

			if (select(sock + 1, nullptr, nullptr, &scks, &times) >= 0)
				return false;

			state = SockState::skERROR;
			return true;
		}

		int Socket::receiveUDP(unsigned char* buffer, int size, sockaddr_in* from) {
#ifdef _MSC_VER
			int client_length = static_cast<int>(sizeof(struct sockaddr_in));
			return recvfrom(this->sock, reinterpret_cast<char*>(buffer), size, 0, reinterpret_cast<struct sockaddr*>(from), &client_length);
#else
			unsigned int client_length = static_cast<unsigned int>(sizeof(struct sockaddr_in));
			return recvfrom(this->sock, reinterpret_cast<char*>(buffer), size, 0, reinterpret_cast<struct sockaddr*>(from), &client_length);
#endif
		}

		void Socket::setTimeout(int miliseconds) {
#ifdef _MSC_VER
			DWORD timeout = miliseconds;
			setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#else
			struct timeval timeout;
			timeout.tv_sec = miliseconds / 1000;
			timeout.tv_usec = (miliseconds - timeout.tv_sec * 1000) * 1000;
			setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif
		}

		int Socket::receive(unsigned char* buffer, int size, int spos) {
			return ::recv(this->sock, reinterpret_cast<char*>(buffer) + spos, size, 0);
		}

		bool Socket::receiveAll(unsigned char* buffer, int size, int spos) {
			int recv = 0;

			while (recv != size) {
				int ret = ::recv(this->sock, reinterpret_cast<char*>(buffer) + spos + recv, size - recv, 0);
				if (ret <= 0)
					return false;

				recv += ret;
			}

			return true;
		}

		int Socket::sendUDP(const unsigned char* buffer, int size, sockaddr_in* to) {
			return ::sendto(sock, reinterpret_cast<const char*>(buffer), size, 0, reinterpret_cast<struct sockaddr*>(&to), sizeof(struct sockaddr_in));
		}

		int Socket::send(const unsigned char* data, int dataSize) {
			return ::send(sock, reinterpret_cast<const char*>(data), dataSize, 0);
		}

		bool Socket::sendAll(const unsigned char* data, int dataSize) {
			int sent = 0;

			while (sent != dataSize) {
				int ret = ::send(sock, reinterpret_cast<const char*>(data) + sent, dataSize - sent, 0);
				if (ret <= 0) return false;

				sent += ret;
			}

			return true;
		}
	}
}