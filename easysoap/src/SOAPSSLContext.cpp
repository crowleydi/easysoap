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

#include <easysoap/SOAP.h>
#include <easysoap/SOAPDebugger.h>
#include <easysoap/SOAPException.h>

#include <easysoap/SOAPSSLContext.h>

USING_EASYSOAP_NAMESPACE

#ifndef HAVE_LIBSSL

SOAPSSLContext::SOAPSSLContext() {
		throw SOAPSocketException("Secure sockets not supported.");
}

~SOAPSSLContext::SOAPSSLContext() {
}


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
		ERR_load_crypto_strings();
		SSL_load_error_strings();
		RAND_seed(rnd_seed, sizeof rnd_seed);
	}

	~OpenSSLinit()
	{
	}
};

// *******************************************************************


SOAPSSLContext::SOAPSSLContext() 
	:	m_ctx(0)
{
		static OpenSSLinit __opensslinit;
		m_ctx = SSL_CTX_new(SSLv23_client_method());
		if (!m_ctx)
				throw SOAPMemoryException();
}

SOAPSSLContext::SOAPSSLContext(const char* certfile, const char* keyfile, const char* password)
		: m_certfile(certfile)
		, m_keyfile(keyfile)
		, m_password(password)
		, m_ctx(0)
{
		static OpenSSLinit __opensslinit;
		m_ctx = SSL_CTX_new(SSLv23_client_method());
		if (!m_ctx)
				throw SOAPMemoryException();
		SetCertInfo(certfile, keyfile, password);
}
/*
SOAPSSLContext::SOAPSSLContext(const SOAPSSLContext& ctx)
{

}

SOAPSSLContext& SOAPSSLContext::operator=(const SOAPSSLContext& ctx)
{
	return *this;	
}
*/
void SOAPSSLContext::SetCertInfo(const char* certfile, const char* keyfile, const char* password)
{
	int retcode;
		
	// TODO: verify that the files exist
	
	

	// TODO: Implmement the rest of the RSA requirements.
		
	// set the certificate file.
	if ((retcode = SSL_CTX_use_certificate_chain_file(m_ctx, m_certfile.Str()))!= 1)
			HandleError("Error trying to use the certificate file: %s\n", retcode);

	// now set our password callback function...
	SSL_CTX_set_default_passwd_cb(m_ctx, &password_cb);
	// setup the callback userdata.
	SSL_CTX_set_default_passwd_cb_userdata(m_ctx, this);
	
	// call the right function based on the certificate type.
	if (type == DSA_cert) {
		if ((retcode = SSL_CTX_use_PrivateKey_file(m_ctx, m_keyfile.Str(), SSL_FILETYPE_PEM)) != 1) 
			HandleError("Error trying to use the private key from the certificate file : %s\n", retcode);
	} else {
		if ((retcode = SSL_CTX_use_RSAPrivateKey_file(m_ctx, m_keyfile.Str(), SSL_FILETYPE_PEM)) != 1)
			HandleError("Error trying to use the RSA private key from the certificate file: %s\n", retcode);
	}

	if ((retcode = SSL_CTX_check_private_key(m_ctx) != 1))
			HandleError("Error while checking the private key : %s\n", retcode);
	
	// TODO: Validate certificate if the root CA cert is given.

}

int SOAPSSLContext::password_cb(char* buf, int size, int rwflag, void *userdata) 
{
		SOAPString password = ((SOAPSSLContext*)userdata)->m_password;
		if (size < password.Length())
				throw SOAPMemoryException();
		
		sp_strcpy(buf, password.Str());
		
		return(password.Length());
}



void SOAPSSLContext::HandleError(const char* context, int retcode)
{
	char msg[2048];
	int err = ERR_get_error();
#if OPENSSL_VERSION_NUMBER >= 0x00906000L
	ERR_error_string_n(err, msg, sizeof(msg) - 1);
#else
	// dangerous
	ERR_error_string(err, msg);
#endif // OPENSSL_VERSION_NUMBER

	msg[sizeof(msg) - 1] = 0;
	throw SOAPSocketException(context, msg);
		
}

SSL_CTX* SOAPSSLContext::GetContext()
{
		return m_ctx;
}

SOAPSSLContext::~SOAPSSLContext() 
{
		if (m_ctx)
		{
				SSL_CTX_free(m_ctx);
				m_ctx = 0;
		}
}


#endif // EASYSOAP_SSLCONTEXT
