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

#ifdef _WIN32
#pragma warning (disable: 4786)
#endif // _WIN32

#include "SOAPEnvelopeHandler.h"
#include "SOAPEnvelope.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *SOAPEnvelopeHandler::start_tag = FULL_SOAP_ENV PARSER_NS_SEP "Envelope";

SOAPEnvelopeHandler::SOAPEnvelopeHandler(SOAPEnvelope& envelope)
: m_envelope(&envelope)
, m_bodyHandler(envelope.GetBody())
, m_headerHandler(envelope.GetHeader())
, m_done(false)
{

}

SOAPEnvelopeHandler::~SOAPEnvelopeHandler()
{

}

SOAPParseEventHandler *
SOAPEnvelopeHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	m_done = false;
	return this;
}

SOAPParseEventHandler *
SOAPEnvelopeHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	m_done = false;
	if (sp_strcmp(name, SOAPBodyHandler::start_tag) == 0)
	{
		return m_bodyHandler.start(name, attrs);
	}
	else if (sp_strcmp(name, SOAPHeaderHandler::start_tag) == 0)
	{
		return m_headerHandler.start(name, attrs);
	}
	//
	// FIX ME: Get actual tag used, not one with the namespace
	// //
	throw SOAPException("Unknown tag in SOAP Envelope: %s", name);
}

void
SOAPEnvelopeHandler::characterData(const XML_Char *str, int len)
{
}

void
SOAPEnvelopeHandler::endElement(const XML_Char *name)
{
	if (sp_strcmp(name, start_tag) == 0)
		m_done = true;
}


