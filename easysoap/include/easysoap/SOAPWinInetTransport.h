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

#if !defined(AFX_SOAPWININETTRANSPORT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)
#define AFX_SOAPWININETTRANSPORT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_

#include <windows.h>
#include <wininet.h>

#include <easysoap/SOAPUrl.h>
#include <easysoap/SOAPTransport.h>

BEGIN_EASYSOAP_NAMESPACE

class SOAPWinInetTransport : public SOAPTransport
{
private:
	SOAPWinInetTransport(const SOAPWinInetTransport&);
	SOAPWinInetTransport& operator=(const SOAPWinInetTransport&);

	SOAPString		m_charset;
	SOAPString		m_errorString;
	SOAPString		m_userAgent;
	size_t			m_canRead;
	bool			m_keepAlive;
	SOAPUrl			m_endpoint;
	HINTERNET		m_hInternet;
	HINTERNET		m_hConnect;
	HINTERNET		m_hRequest;

	const char * GetErrorInfo();
	void Close();

public:
	SOAPWinInetTransport();
	SOAPWinInetTransport(const SOAPUrl& endpoint);
	SOAPWinInetTransport(const SOAPUrl& endpoint, const SOAPUrl& proxy);
	~SOAPWinInetTransport();

	void ConnectTo(const SOAPUrl& endpoint);
	void ConnectTo(const SOAPUrl& endpoint, const SOAPUrl& proxy);

	void SetUserAgent(const char *userAgent);
	void SetKeepAlive(bool keepAlive = true);

	const char *GetCharset() const;
	size_t Read(char *buffer, size_t bufflen);
	size_t Write(const SOAPMethod& method, const char *packet, size_t len);
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPWININETTRANSPORT_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)

