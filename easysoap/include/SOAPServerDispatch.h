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


#if !defined(AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_)
#define AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_


#include <SOAP.h>

class SOAPDispatchHandlerInterface;
class SOAPHeaderHandlerInterface;

class EASYSOAP_EXPORT SOAPServerDispatch
{
public:
	SOAPServerDispatch();
	virtual ~SOAPServerDispatch();

	SOAPServerDispatch& DispatchTo(SOAPDispatchHandlerInterface *disp);
	SOAPServerDispatch& DispatchHeadersTo(SOAPHeaderHandlerInterface *disp);

	int Handle(SOAPTransport& transport);

protected:
	virtual bool	HandleRequest(SOAPEnvelope& request, SOAPResponse& response);
	virtual void	HandleHeaders(SOAPEnvelope& request);

private:

	SOAPServerDispatch(const SOAPServerDispatch&);
	SOAPServerDispatch& operator=(const SOAPServerDispatch&);
	void WriteFault(const char *code, const char *str);

	typedef SOAPArray<SOAPDispatchHandlerInterface*> Handlers;
	typedef SOAPArray<SOAPHeaderHandlerInterface*> HeaderHandlers;

	Handlers			m_handlers;
	HeaderHandlers		m_headerHandlers;
	SOAPTransport		*m_transport;
	SOAPResponse		m_response;
	SOAPEnvelope		m_request;
	SOAPPacketWriter	m_writer;
	SOAPParser			m_parser;
};


#endif // !defined(AFX_SOAPSERVERDISPATCH_H__F7015C4A_8459_4090_ABD1_FB3EAB144B39__INCLUDED_)


