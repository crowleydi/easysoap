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


#if !defined(AFX_SOAPENVELOPEHANDLER_H__DD7D4800_3A07_4FF2_943F_E7364E9278E7__INCLUDED_)
#define AFX_SOAPENVELOPEHANDLER_H__DD7D4800_3A07_4FF2_943F_E7364E9278E7__INCLUDED_


#include "SOAPBodyHandler.h"
#include "SOAPHeaderHandler.h"

BEGIN_EASYSOAP_NAMESPACE

class SOAPEnvelope;

class SOAPEnvelopeHandler : public SOAPParseEventHandler  
{

public:

	SOAPEnvelopeHandler();
	virtual ~SOAPEnvelopeHandler();

	void SetEnvelope(SOAPEnvelope& env);

	virtual SOAPParseEventHandler* start(SOAPParser& parser,
			const char *name,
			const char **attrs);
	virtual SOAPParseEventHandler* startElement(SOAPParser& parser,
			const char *name,
			const char **attrs);


private:
	bool				m_done;
	SOAPEnvelope		*m_envelope;
	SOAPBodyHandler		m_bodyHandler;
	SOAPHeaderHandler	m_headerHandler;

};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPENVELOPEHANDLER_H__DD7D4800_3A07_4FF2_943F_E7364E9278E7__INCLUDED_)

