/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include "SOAPClientSocketImp.h"



#ifdef _WIN32

#include <winsock2.h>
//
// Initialize Windows socket library
//
static class WinSockInit
{
public:
	WinSockInit()
	{
		WSADATA wsaData;
		// Is version 0x0202 appropriate?
		// I have no idea...
		int err = WSAStartup( 0x0202, &wsaData );
	}

	~WinSockInit()
	{
		int err = WSACleanup();
	}
} __winsockinit;

#define SOL_TCP IPPROTO_TCP
#else // not _WIN32

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <netdb.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close

#define SOCKADDR struct sockaddr
#define SOCKADDR_IN struct sockaddr_in
#define LPHOSTENT struct hostent*
#define LPIN_ADDR struct in_addr

#endif // _WIN32

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPClientSocketImp::SOAPClientSocketImp()
: m_socket(INVALID_SOCKET)
{
}

SOAPClientSocketImp::~SOAPClientSocketImp()
{
	Close();
}

void
SOAPClientSocketImp::Close()
{
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);

	m_socket = INVALID_SOCKET;
}


bool
SOAPClientSocketImp::WaitRead(int sec, int usec)
{
	struct timeval tv;
	fd_set fset;

	FD_ZERO(&fset);
	FD_SET(m_socket, &fset);

	tv.tv_sec = sec;
	tv.tv_usec = usec;

	int ret = select(m_socket+1, &fset, 0, 0, sec == -1 ? 0 : &tv);
	if (ret == SOCKET_ERROR)
		throw SOAPException("WaitRead select error");

	return ret == 1;
}

bool
SOAPClientSocketImp::WaitWrite(int sec, int usec)
{
	struct timeval tv;
	fd_set fset;

	FD_ZERO(&fset);
	FD_SET(m_socket, &fset);

	tv.tv_sec = sec;
	tv.tv_usec = usec;

	int ret = select(m_socket+1, 0, &fset, 0, sec == -1 ? 0 : &tv);
	if (ret == SOCKET_ERROR)
		throw SOAPException("WaitWrite select error");

	return ret == 1;
}

bool
SOAPClientSocketImp::IsOpen()
{
	return m_socket != INVALID_SOCKET;
}


bool
SOAPClientSocketImp::Connect(const char *server, unsigned int port)
{
	Close();

	//
	// TODO: Abstract this away into other class/methods
	//
	m_socket = 0;
	m_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		throw SOAPSocketException("Error creating socket");

	struct sockaddr_in sockAddr;
	sp_memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_socket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		throw SOAPSocketException("Error binding socket");

	sp_memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(server);
	sockAddr.sin_port = htons((u_short)port);
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		struct hostent *lphost = gethostbyname(server);
		if (lphost != NULL)
		{
			sockAddr.sin_addr.s_addr = ((struct in_addr *)lphost->h_addr)->s_addr;
		}
		else
			throw SOAPSocketException("Could not resolve host name: %s", server);
	}

	if (connect(m_socket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		throw SOAPSocketException("Could not connect to host: %s", server);
	}

	int nodelay = 1;
	if (setsockopt(m_socket, SOL_TCP, TCP_NODELAY, (const char *)&nodelay, sizeof(nodelay)) == -1)
	{
		throw SOAPSocketException("Could not set TCP_NODELAY");
	}
	return true;
}

int
SOAPClientSocketImp::Read(char *buff, int bufflen)
{
	int bytes = 0;
	if (bufflen > 0)
	{
		bytes = recv(m_socket, buff, bufflen, 0);
		if (bytes == 0)
		{
			Close(); // other side dropped the connection
		}
		else if (bytes == SOCKET_ERROR)
		{
			throw SOAPSocketException("Error reading socket");
		}
	}
	return bytes;
}

int
SOAPClientSocketImp::Write(const char *buff, int bufflen)
{
	int bytes = 0;
	if (bufflen > 0)
	{
		bytes = send(m_socket, buff, bufflen, 0);
		if (bytes == SOCKET_ERROR)
		{
			throw SOAPSocketException("Error writing to socket");
		}
		else if (bytes != bufflen)
		{
			throw SOAPSocketException("Error writing to socket, "
					"tried to write %d bytes, wrote %d",
					bufflen, bytes);
		}
	}
	return bytes;
}

