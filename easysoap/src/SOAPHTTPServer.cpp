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


#include "SOAPHTTPServer.h"


extern "C" {
#include "abyss.h"
}

//
// Little static class to init the Abyss web server.
// Not sure if it's needed...
static class AbyssInit
{
public:
	AbyssInit()
	{
		DateInit();
		MIMETypeInit();
	}
} _abyssinit;


//
// Class for reading the socket/session the web server
// hands to us.
//
class SOAPHTTPServerTransport : public SOAPTransport
{
public:
	SOAPHTTPServerTransport(struct _TSession *session);
	virtual ~SOAPHTTPServerTransport();

	void SetError();
	const char *GetCharset() const;
	size_t Read(char *buffer, size_t buffsize);
	size_t Write(const SOAPMethod& method, const char *payload, size_t payloadsize);

private:
	struct _TSession	*m_session;
	size_t				m_bytestoread;
	SOAPString			m_charset;
	bool				m_error;
};

SOAPHTTPServerTransport::SOAPHTTPServerTransport(struct _TSession *session)
: m_session(session), m_bytestoread(0), m_error(false)
{
	const char *contlen = TableFind(&session->request_headers, "content-length");
	const char *contype = TableFind(&session->request_headers, "content-type");

	if (contlen)
		m_bytestoread = atoi(contlen);

	ParseContentType(m_charset, contype);
}

SOAPHTTPServerTransport::~SOAPHTTPServerTransport()
{
}

void
SOAPHTTPServerTransport::SetError()
{
	m_error = true;
}

const char *
SOAPHTTPServerTransport::GetCharset() const
{
	return m_charset;
}

size_t
SOAPHTTPServerTransport::Read(char *buffer, size_t buffsize)
{
	size_t bytesread = 0;
	if (m_bytestoread > 0)
	{
		if (buffsize > m_bytestoread)
			buffsize = m_bytestoread;

		bytesread = HTTPRead(m_session, buffer, buffsize);
		m_bytestoread -= bytesread;
	}
	return bytesread;
}

size_t
SOAPHTTPServerTransport::Write(const SOAPMethod& method, const char *payload, size_t payloadsize)
{
	char lenstr[32];
	snprintf(lenstr, sizeof(lenstr), "%u", payloadsize);

	ResponseStatus(m_session, m_error ? 500 : 200);
	ResponseContentType(m_session, "text/xml; charset=\"UTF-8\"");
	ResponseAddField(m_session, "Content-Length", lenstr);
	ResponseWrite(m_session);
	HTTPWrite(m_session, (char *)payload, payloadsize);
	HTTPWriteEnd(m_session);
	return payloadsize;
}

SOAPHTTPServer::SOAPHTTPServer(int port)
{
	m_server = new TServer;
	ServerCreate(m_server, EASYSOAP_STRING, port, 0, 0);
	ServerAddHandler(m_server, _RequestHandler);
	ServerInit(m_server);
	ServerSetUserData(m_server, (void *)this);
}

SOAPHTTPServer::~SOAPHTTPServer()
{
	ServerFree(m_server);
	delete m_server;
}

int
SOAPHTTPServer::Handle()
{
	ServerRun(m_server);
	return 0;
}

void
SOAPHTTPServer::RequestHandler(struct _TSession *session)
{
	SOAPHTTPServerTransport trans(session);
	m_dispatch.Handle(trans);
}

int
SOAPHTTPServer::_RequestHandler(struct _TSession *session)
{
	try
	{
		SOAPHTTPServer *server = (SOAPHTTPServer *)ServerGetUserData(session->server);
		server->RequestHandler(session);
		return 1;
	}
	catch (SOAPException&)
	{
		return 0;
	}
}

void SOAPHTTPServer::Stop()
{
	StopServer(m_server);
}

void SOAPHTTPServer::ResetFlag()
{
	ResetServerStopFlag(m_server);
}

int SOAPHTTPServer::IsStopped()
{
	return IsServerStopped(m_server);
}

