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

#include <easysoap/SOAP.h>

struct ssl_st;
struct ssl_ctx_st;
struct rsa_st;
struct x509_st;
struct ssl_method_st;

BEGIN_EASYSOAP_NAMESPACE

#ifndef HAVE_LIBSSL
/**
*
*/
class EASYSOAP_EXPORT SOAPSSLContext
{
private:
	void NotSupported()
	{
		throw SOAPException("HTTPS NOT SUPPORTED WITHOUT OpenSSL");
	}

public:
        typedef enum {
                SSL_v2,
                SSL_v23,
                SSL_v3,
                TLS_v1
        } MethodType;

	SOAPSSLContext(MethodType /*methodType*/=SSL_v23)
	{
		NotSupported();
	}

	SOAPSSLContext(const char* /*cafile*/, MethodType /*methodType*/=SSL_v23)
	{
		NotSupported();
	}

	SOAPSSLContext(const char* /*certfile*/,
		const char* /*keyfile*/,
		const char* /*password*/,
		const char* /*cafile*/=0,
		MethodType /*methodType*/=SSL_v23)
	{
		NotSupported();
	}

	ssl_ctx_st*	GetContext() {return 0;}
	void SetCAInfo(const char* /*cafile*/);
	void SetCertInfo(const char* /*certfile*/,
		const char* /*keyfile*/,
		const char* /*password*/)
	{
	}

	bool VerifyServerCert() { return false; };
	//void SetVerifyServerCert (bool /*v*/) { } ;

        typedef bool (*VerifyPeerCallback) (struct x509_st* peercert, void* cbdata);
        VerifyPeerCallback GetVerifyPeerCallback() { return 0; }
        void SetVerifyPeerCallback(VerifyPeerCallback)  { }

	bool IgnoreCertError(int /*rc*/ ) { return false; } ;
	void AddCertErrorToIgnoreList(int /*rc*/) { };
	bool RemoveCertErrorFromIgnoreList(int /*rc*/) { return false; };
	~SOAPSSLContext() {}
};
#else


/**
*
*/
class EASYSOAP_EXPORT SOAPSSLContext
{
public:
        typedef enum {
                SSL_v2,
                SSL_v23,
                SSL_v3,
                TLS_v1
        } MethodType;

	SOAPSSLContext(MethodType methodType=SSL_v23);
	SOAPSSLContext(const char* cafile, MethodType methodType=SSL_v23);
	SOAPSSLContext(const char* certfile,
					const char* keyfile,
					const char* password,
					const char* cafile=0,
					MethodType methodType=SSL_v23);
	~SOAPSSLContext();

	ssl_ctx_st*	GetContext();
	void SetCAInfo(const char* cafile);
	void SetCertInfo(const char* certfile, const char* keyfile, const char* password);

	// callbacks required by openSSL
	static int 	password_cb(char *buf, int num, int rwflag, void *userdata);
	
	bool VerifyServerCert() { return m_verifyserver; } ;
	void SetVerifyServerCert (bool v)  { m_verifyserver = v; } ;

        typedef bool (*VerifyPeerCallback) (struct x509_st* peercert, void* cbdata);
        VerifyPeerCallback GetVerifyPeerCallback() { return m_verifycb; }
        void SetVerifyPeerCallback(VerifyPeerCallback cb)  { m_verifycb = cb; }

	bool IgnoreCertError(int rc );
	void AddCertErrorToIgnoreList(int rc) ;
	bool RemoveCertErrorFromIgnoreList(int rc) ;

private:
	typedef enum {
			RSA_cert,
			DSA_cert
	} CertType;

	static rsa_st*		m_tmpRSAKey;
	SOAPString 			m_cafile;
	SOAPString 			m_certfile;
	SOAPString 			m_keyfile;
	SOAPString 			m_password;
	ssl_ctx_st*			m_ctx;
	CertType 			type;

	bool				m_verifyserver;
	SOAPArray<int> 		m_certerrors;
        VerifyPeerCallback      m_verifycb;


	SOAPSSLContext(const SOAPSSLContext& ctx);
	SOAPSSLContext& operator=(const SOAPSSLContext& ctx);

	void sslinit();
        ssl_method_st* getMethod(MethodType methodType);
	// handles error, returns true if they are recoverable, false if not.
	void HandleError(const char* context, int retcode);
};
#endif
END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPSSLCONTEXT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)



