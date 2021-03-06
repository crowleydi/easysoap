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


#if !defined(AFX_SOAPSECURESOCKETIMP_H__7481DF95_30AD_4892_B5E4_44463F2F6D42__INCLUDED_)
#define AFX_SOAPSECURESOCKETIMP_H__7481DF95_30AD_4892_B5E4_44463F2F6D42__INCLUDED_

#include <easysoap/SOAPSocket.h>
#include "SOAPClientSocketImp.h"

struct ssl_st;

BEGIN_EASYSOAP_NAMESPACE

class SOAPSSLContext;

class SOAPSecureSocketImp : public SOAPSocketInterface
{
private:
	SOAPClientSocketImp m_socket;
	void NotSupported();

protected:
	ssl_st				*m_ssl;
	SOAPSSLContext		        *m_context;
	bool				m_delctx;
        void                            *m_cbdata;

	bool HandleError(const char *context, int retcode, bool shouldWait = true);
	void InitSSL();
	void VerifyCert(const char* host);
	const char* CheckForCertError(int rc);
public:
	SOAPSecureSocketImp();
	SOAPSecureSocketImp(SOAPSSLContext& ctx, void* cbdata=0);
	virtual ~SOAPSecureSocketImp();

	virtual bool WaitRead(int sec = -1, int usec = 0);
	virtual bool WaitWrite(int sec = -1, int usec = 0);
	virtual bool IsOpen();
	virtual void Close();
	virtual bool Connect(const char *host, unsigned int port) {
		return Connect(host, port, true);
	}
	virtual bool Connect(const char *host, unsigned int port, bool client );
	virtual size_t Read(char *buffer, size_t len);
	virtual size_t Write(const char *buffer, size_t len);

};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPSECURESOCKETIMP_H__7481DF95_30AD_4892_B5E4_44463F2F6D42__INCLUDED_)

