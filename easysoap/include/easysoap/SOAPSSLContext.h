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
#if !defined(AFX_SOAPSSLCONTEXT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)
#define AFX_SOAPSSLCONTEXT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_
#include <openssl/ssl.h>
#include <easysoap/SOAP.h>


BEGIN_EASYSOAP_NAMESPACE

class EASYSOAP_EXPORT SOAPSSLContext
{
		public:
				SOAPSSLContext();
				SOAPSSLContext(const char* certfile, const char* keyfile, const char* password);
				~SOAPSSLContext();
				//SOAPSSLContext(const SOAPSSLContext& ctx);
				//SOAPSSLContext& operator=(const SOAPSSLContext& ctx);
				
				SSL_CTX*	GetContext();
				void SetCertInfo(const char* certfile, const char* keyfile, const char* password);
				
				// callbacks required by openSSL
				static int 	password_cb(char *buf, int num, int rwflag, void *userdata);

				typedef enum {
						RSA_cert,
						DSA_cert
				} CertType;

		private:
				SSL_CTX*			m_ctx;
				SOAPString 			m_certfile;
				SOAPString 			m_keyfile;
				SOAPString 			m_password;
				CertType 			type;
				// handles error, returns true if they are recoverable, false if not.
				void HandleError(const char* context, int retcode);
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPSSLCONTEXT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)



