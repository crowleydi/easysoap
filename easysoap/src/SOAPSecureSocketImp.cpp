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
#include <winsock2.h>
#else // not _WIN32
#include <sys/time.h>
#endif // _WIN32

#ifndef EASYSOAP_SSL

#include "SOAPSecureSocketImp.h"

SOAPSecureSocketImp::SOAPSecureSocketImp()
{
	throw SOAPSocketException("Secure sockets not supported.");
}

SOAPSecureSocketImp::~SOAPSecureSocketImp()
{
}

bool SOAPSecureSocketImp::Connect(const char *, unsigned int) {return false;}
void SOAPSecureSocketImp::Close() { }
int SOAPSecureSocketImp::Read(char *, int) {return 0;}
int SOAPSecureSocketImp::Write(const char *, int) {return 0;}
bool SOAPSecureSocketImp::WaitRead(int sec, int usec) {return false;}
void SOAPSecureSocketImp::InitSSL() {}

#else

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "SOAPSecureSocketImp.h"

//
// Initialize OpenSSH
//
static class OpenSSLinit
{
public:
	OpenSSLinit()
	{
		SSL_library_init();
		SSL_load_error_strings();
	}
	~OpenSSLinit()
	{
	}
} __opensslinit;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SOAPSecureSocketImp::SOAPSecureSocketImp()
: m_ssl(0)
, m_ctx(0)
{
}

SOAPSecureSocketImp::~SOAPSecureSocketImp()
{
	Close();
}

bool
SOAPSecureSocketImp::HandleError(const char *context, int retcode)
{
	// we may have an error.
	// we need to call SSL_get_error()
	bool retry = false;
	unsigned long err = SSL_get_error(m_ssl, retcode);
	switch (err)
	{
	case SSL_ERROR_NONE:
		break;

	case SSL_ERROR_ZERO_RETURN:
		Close();
		break;

	case SSL_ERROR_WANT_WRITE:
		super::WaitWrite();
		retry = true;
		break;
	case SSL_ERROR_WANT_READ:
		super::WaitRead();
		retry = true;
		break;
	case SSL_ERROR_WANT_X509_LOOKUP:
		retry = true;
		break;

	case SSL_ERROR_SYSCALL:
	case SSL_ERROR_SSL:
	default:
		{
		char msg[256];
		ERR_error_string_n(err, msg, sizeof(msg) - 1);
		msg[sizeof(msg) - 1] = 0;
		throw SOAPSocketException(context, msg);
		}
	}
	return retry;
}

void
SOAPSecureSocketImp::InitSSL()
{
	//
	// set up SSL
	//
	m_ctx = SSL_CTX_new(SSLv23_client_method());
	if (!m_ctx)
		throw SOAPMemoryException();

	m_ssl = SSL_new(m_ctx);
	if (!m_ssl)
		throw SOAPMemoryException();

	int retcode;

	if ((retcode = SSL_set_fd(m_ssl, m_socket)) != 1)
		HandleError("Error attaching security layer to socket", retcode);

	if ((retcode = SSL_connect(m_ssl)) != 1)
		HandleError("Error negotiating secure connection", retcode);

}

bool
SOAPSecureSocketImp::Connect(const char *host, unsigned int port)
{
	if (!super::Connect(host, port))
		return false;

	InitSSL();
	return true;
}

bool
SOAPSecureSocketImp::WaitRead(int sec, int usec)
{
	if (!m_ssl)
		return super::WaitRead(sec, usec);

	if (SSL_pending(m_ssl) > 0)
		return true;

	// we have to wait...
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	select(0, 0, 0, 0, sec == -1 ? 0 : &tv);

	return SSL_pending(m_ssl) > 0;
}

int
SOAPSecureSocketImp::Read(char *buff, int bufflen)
{
	if (!m_ssl)
		return super::Read(buff, bufflen);

	int bytes = 0;
	if (bufflen > 0)
	{
		bool retry = false;
		do
		{
			bytes = SSL_read(m_ssl, buff, bufflen);
			if (bytes > 0)
			{
				// good, we read some bytes.
				retry = false;
			}
			else 
			{
				// check for an error
				retry = HandleError("Error reading from secure socket", bytes);
				bytes = 0;
			}
		} while (retry);
	}
	return bytes;
}

int
SOAPSecureSocketImp::Write(const char *buff, int bufflen)
{
	if (!m_ssl)
		return super::Write(buff, bufflen);

	if (bufflen > 0)
	{
		bool retry = false;
		do
		{
			int bytes = SSL_write(m_ssl, buff, bufflen);
			if (bytes > 0)
			{
				if (bytes != bufflen)
					throw SOAPSocketException("Error writing to secure socket, "
						   "expected to write %d bytes, wrote %d bytes",
						   bufflen, bytes);
				retry = false;
			}
			else 
			{
				// check for an error
				retry = HandleError("Error writing to secure socket", bytes);
				bytes = 0;
			}
		} while (retry);
	}
	return 0;
}

void
SOAPSecureSocketImp::Close()
{
	if (m_ssl)
		SSL_shutdown(m_ssl);

	super::Close();

	if (m_ssl)
	{
		SSL_free(m_ssl);
		m_ssl = 0;
	}

	if (m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = 0;
	}
}


#endif // EASYSOAP_SSL

