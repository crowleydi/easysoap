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


#if !defined(AFX_SOAPONHTTP_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)
#define AFX_SOAPONHTTP_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_

#include "SOAPUrl.h"
#include "SOAPSocket.h"
#include "SOAPTransport.h"


class EASYSOAP_EXPORT SOAPHTTPProtocol : public SOAPProtocolBase
{
private:
	typedef	SOAPProtocolBase super;
	typedef SOAPHashMapNoCase<SOAPString, SOAPString>	HeaderMap;

	HeaderMap	m_headers;
	SOAPUrl		m_endpoint;
	SOAPUrl		m_proxy;
	SOAPString	m_httpmsg;
	int			m_canread;
	bool		m_httpproxy;
	bool		m_doclose;

	int		GetReply();
	bool	Connect();
	void	WriteHostHeader(const SOAPUrl&);
	void	FlushInput();
	void	StartVerb(const char *verb, const char *path);

public:
	SOAPHTTPProtocol()
		: m_canread(-1)
		, m_doclose(false)
		, m_httpproxy(false)
	{}

	SOAPHTTPProtocol(const SOAPUrl& endpoint)
		: m_canread(-1)
		, m_doclose(false)
		, m_httpproxy(false)
	{
		ConnectTo(endpoint);
	}

	SOAPHTTPProtocol(const SOAPUrl& endpoint, const SOAPUrl& proxy)
		: m_canread(-1)
		, m_doclose(false)
		, m_httpproxy(false)
	{
		ConnectTo(endpoint, proxy);
	}

	~SOAPHTTPProtocol()
	{}

	void	ConnectTo(const SOAPUrl& endpoint);
	void	ConnectTo(const SOAPUrl& endpoint, const SOAPUrl& proxy);
	int		Get(const char *path);
	void	BeginPost(const char *path);
	int		PostData(const char *bytes, int len);
	void	WriteHeader(const char *header, const char *value);
	void	WriteHeader(const char *header, int value);

	const char *GetHeader(const char *header);
	int		GetContentLength();

	virtual int Read(char *buffer, int len);
	virtual void Close();
	virtual bool CanRead();
};


class EASYSOAP_EXPORT SOAPonHTTP : public SOAPTransport
{
private:
	SOAPHTTPProtocol	m_http;
	SOAPString			m_path;

public:
	SOAPonHTTP() {}

	SOAPonHTTP(const SOAPUrl& endpoint)
		: m_http(endpoint)
		, m_path(endpoint.Path())
	{}

	SOAPonHTTP(const SOAPUrl& endpoint, const SOAPUrl& proxy)
		: m_http(endpoint, proxy)
		, m_path(endpoint.Path())
	{}

	virtual ~SOAPonHTTP() {}

	// read the payload into the buffer.
	// can be called multiple times.
	// returns 0 if entire payload has been read.
	virtual int Read(char *buffer, int buffsize);

	// send the payload.  can only be called ONCE per
	// payload.
	virtual int Write(const SOAPMethod& method, const char *payload, int payloadsize);
};


#endif // !defined(AFX_SOAPONHTTP_H__7D357522_E8B1_45A2_8CE1_A472A7D58C13__INCLUDED_)


