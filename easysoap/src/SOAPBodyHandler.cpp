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


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include "SOAPBodyHandler.h"
#include "SOAPBody.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *SOAPBodyHandler::start_tag = FULL_SOAP_ENV PARSER_NS_SEP "Body";

SOAPBodyHandler::SOAPBodyHandler(SOAPBody& body)
: m_body(&body)
, m_methodHandler(body.GetMethod())
, m_faultHandler(body.GetFault())
{

}

SOAPBodyHandler::~SOAPBodyHandler()
{

}

SOAPParseEventHandler *
SOAPBodyHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	return this;
}

SOAPParseEventHandler *
SOAPBodyHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	if (sp_strcmp(name, SOAPFaultHandler::start_tag) == 0)
	{
		m_body->SetIsFault(true);
		return m_faultHandler.start(name, attrs);
	}

	m_body->SetIsFault(false);
	return m_methodHandler.start(name, attrs);
}

void
SOAPBodyHandler::characterData(const XML_Char *str, int len)
{
}

void
SOAPBodyHandler::endElement(const XML_Char *name)
{
}
