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

#include "SOAPSecureSocketImp.h"

USING_EASYSOAP_NAMESPACE

#ifndef HAVE_LIBSSL

SOAPSecureSocketImp::SOAPSecureSocketImp()
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
void SOAPSecureSocketImp::SetCertificateInfo(const char* , const char*) {}
#else

extern "C" {
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
};

//
// Initialize OpenSSL
//
class OpenSSLinit
{
public:
	OpenSSLinit()
	{
		static const char rnd_seed[] = 
			"string to make the random number generator"
			"think it has some entropy.";

		SSL_library_init();
		SSL_load_error_strings();
		RAND_seed(rnd_seed, sizeof rnd_seed);
	}

	~OpenSSLinit()
	{
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SOAPSecureSocketImp::SOAPSecureSocketImp()
: m_ssl(0)
, m_ctx(0)
{
	static OpenSSLinit __opensslinit;
}

void SOAPSecureSocketImp::SetCertificateInfo(const char* keyfile, const char* password)
{
	m_keyfile = keyfile;
	m_password = password;
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

// password callback function for retrieving the password.
// Since we can't have user interaction, it will have to be
// stored somehow.. Gulp
int SOAPSecureSocketImp::password_cb(char* buf, int size, int rwflag, void *userdata) 
{
	const SOAPString& password = ((SOAPSecureSocketImp*)userdata)->m_password;
	if (size < sp_strlen(password.Str())+1)
		return 0;
	sp_strcpy(buf, password.Str());
	return(sp_strlen(password.Str()));
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
	// set up our keys and password, if required
	if (!m_keyfile.IsEmpty() ) {
			if ((retcode = SSL_CTX_use_certificate_chain_file(m_ctx, m_keyfile.Str()))!= 1)
					HandleError("Error trying to use the certificate file: %s\n", retcode);

			// now set our password callback function...
			SSL_CTX_set_default_passwd_cb(m_ctx, &password_cb);
			// setup the callback userdata.
			SSL_CTX_set_default_passwd_cb_userdata(m_ctx, this);
			
			retcode = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyfile.Str(), SSL_FILETYPE_PEM);
			if (retcode != 1) 
					HandleError("Error trying to use the private key from the certificate file : %s\n", retcode);

			if ((retcode = SSL_CTX_check_private_key(m_ctx) != 1))
					HandleError("Error while checking the private key : %s\n", retcode);

	}

	if ((retcode = SSL_set_fd(m_ssl, m_socket)) != 1)
		HandleError("Error attaching security layer to socket : %s\n", retcode);

	if ((retcode = SSL_connect(m_ssl)) != 1)
		HandleError("Error negotiating secure connection : %s\n", retcode);

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

	if (m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = 0;
	}
}

#endif // EASYSOAP_SSL

