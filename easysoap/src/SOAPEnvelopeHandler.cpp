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

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include "SOAPEnvelopeHandler.h"
#include <SOAPEnvelope.h>
#include <SOAPNamespaces.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPEnvelopeHandler::SOAPEnvelopeHandler()
: m_done(false)
, m_envelope(0)
{

}

SOAPEnvelopeHandler::~SOAPEnvelopeHandler()
{

}


void
SOAPEnvelopeHandler::SetEnvelope(SOAPEnvelope& env)
{
	m_envelope = &env;
	m_done = false;
	m_bodyHandler.SetBody(m_envelope->GetBody());
	m_headerHandler.SetHeader(m_envelope->GetHeader());
}


SOAPParseEventHandler *
SOAPEnvelopeHandler::start(SOAPParser&, const XML_Char *, const XML_Char **)
{
	m_done = false;
	return this;
}

SOAPParseEventHandler *
SOAPEnvelopeHandler::startElement(SOAPParser& parser, const XML_Char *name, const XML_Char **attrs)
{
	m_done = false;
	if (sp_strcmp(name, SOAP_ENV PARSER_NS_SEP "Body") == 0)
	{
		return m_bodyHandler.start(parser, name, attrs);
	}
	else if (sp_strcmp(name, SOAP_ENV PARSER_NS_SEP "Header") == 0)
	{
		return m_headerHandler.start(parser, name, attrs);
	}
	else if (sp_strcmp(name, SOAP_ENV PARSER_NS_SEP "Envelope") == 0)
	{
		return this;
	}
	//
	// FIX ME: Get actual tag used, not one with the namespace
	// //
	throw SOAPException("Unknown tag in SOAP Envelope: %s", name);
}

void
SOAPEnvelopeHandler::endElement(const XML_Char *name)
{
	if (sp_strcmp(name, SOAP_ENV PARSER_NS_SEP "Envelope") == 0)
		m_done = true;
}


