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
 *
 * $Id$
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#ifdef _WIN32
#if !defined (__MWERKS__)
#include <winsock2.h>
#endif
#else // not _WIN32
#include <sys/time.h>
#endif // _WIN32

#include <easysoap/SOAP.h>
#include <easysoap/SOAPDebugger.h>
#include <easysoap/SOAPSSLContext.h>

#include "SOAPSecureSocketImp.h"

USING_EASYSOAP_NAMESPACE

#ifndef HAVE_LIBSSL

SOAPSecureSocketImp::SOAPSecureSocketImp()
{
	throw SOAPSocketException("Secure sockets not supported.");
}

SOAPSecureSocketImp::SOAPSecureSocketImp(SOAPSSLContext& ctx) 
{
	throw SOAPSocketException("Secure sockets not supported.");
}

SOAPSecureSocketImp::~SOAPSecureSocketImp()
{
}

bool SOAPSecureSocketImp::Connect(const char *, unsigned int) {return false;}
void SOAPSecureSocketImp::Close() { }
size_t SOAPSecureSocketImp::Read(char *, size_t) {return 0;}
size_t SOAPSecureSocketImp::Write(const char *, size_t) {return 0;}
bool SOAPSecureSocketImp::WaitRead(int, int) {return false;}
void SOAPSecureSocketImp::InitSSL() {}
#else

extern "C" {
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SOAPSecureSocketImp::SOAPSecureSocketImp()
: m_ssl(0)
, m_delctx(true)
{
	m_context = new SOAPSSLContext;
}

SOAPSecureSocketImp::SOAPSecureSocketImp(SOAPSSLContext& ctx) 
: m_ssl(0)
, m_context(&ctx)
, m_delctx(false)
{
}

SOAPSecureSocketImp::~SOAPSecureSocketImp()
{
	if (m_delctx)
			delete m_context;
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
		char msg[2048];

#if OPENSSL_VERSION_NUMBER >= 0x00906000L
		ERR_error_string_n(err, msg, sizeof(msg) - 1);
#else
		// dangerous
		ERR_error_string(err, msg);
#endif // OPENSSL_VERSION_NUMBER

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
	m_ssl = SSL_new(m_context->GetContext());
	if (!m_ssl)
		throw SOAPMemoryException();
	
	int retcode;

	if ((retcode = SSL_set_fd(m_ssl, m_socket)) != 1)
		HandleError("Error attaching security layer to socket : %s\n", retcode);

	if ((retcode = SSL_connect(m_ssl)) != 1)
		HandleError("Error negotiating secure connection : %s\n", retcode);
}

void
SOAPSecureSocketImp::VerifyCert(const char* host)
{
	X509* server_cert = SSL_get_peer_certificate(m_ssl);
	if (!server_cert)
		throw SOAPException("Error getting server certificate.");

	if (SSL_get_verify_result(m_ssl) != X509_V_OK)
		throw SOAPException("Server certificate verification failed.");

	char buf[128];
	X509_NAME_get_text_by_NID(X509_get_subject_name(server_cert),
							  NID_commonName, buf, sizeof(buf));
	if (sp_strcasecmp(buf, host))
		throw SOAPException("Server certificate hostname does not match (%s != %s).", buf, host);

	X509_free(server_cert);
}

bool
SOAPSecureSocketImp::Connect(const char *host, unsigned int port)
{
	if (!super::Connect(host, port))
		return false;

	InitSSL();

	VerifyCert(host);

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
	Wait(sec, usec);
	return SSL_pending(m_ssl) > 0;
}

size_t
SOAPSecureSocketImp::Read(char *buff, size_t bufflen)
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
			SOAPDebugger::Print(2, "SRECV: %d bytes\n", bytes);
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
		SOAPDebugger::Write(1, buff, bytes);
	}
	return bytes;
}

size_t
SOAPSecureSocketImp::Write(const char *buff, size_t bufflen)
{
	if (!m_ssl)
		return super::Write(buff, bufflen);

	if (bufflen > 0)
	{
		bool retry = false;
		do
		{
			size_t bytes = SSL_write(m_ssl, buff, bufflen);
			SOAPDebugger::Print(2, "SSEND: %d bytes\n", bytes);
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
		SOAPDebugger::Write(1, buff, bufflen);
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

}

#endif // EASYSOAP_SSL

