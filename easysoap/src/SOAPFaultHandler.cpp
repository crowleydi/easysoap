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

#include "SOAP.h"
#include "SOAPFaultHandler.h"
#include "SOAPFault.h"
#include "SOAPNamespaces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *SOAPFaultHandler::start_tag = FULL_SOAP_ENV "|Fault";

SOAPFaultHandler::SOAPFaultHandler(SOAPFault& fault)
{
	m_fault = &fault;
}

SOAPFaultHandler::~SOAPFaultHandler()
{

}

SOAPParseEventHandler *
SOAPFaultHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	return this;
}

SOAPParseEventHandler *
SOAPFaultHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	// Check for unscopted fault names
	if (sp_strcmp(name, "faultcode") == 0)
		m_paramHandler.SetParameter(m_fault->AddParameter(SOAPFault::faultcode_attr));
	else if (sp_strcmp(name, "faultactor") == 0)
		m_paramHandler.SetParameter(m_fault->AddParameter(SOAPFault::faultactor_attr));
	else if (sp_strcmp(name, "detail") == 0)
		m_paramHandler.SetParameter(m_fault->AddParameter(SOAPFault::faultdetail_attr));
	else if (sp_strcmp(name, "faultstring") == 0)
		m_paramHandler.SetParameter(m_fault->AddParameter(SOAPFault::faultstring_attr));
	else
		m_paramHandler.SetParameter(m_fault->AddParameter(name));

	return m_paramHandler.start(name, attrs);
}

void
SOAPFaultHandler::characterData(const XML_Char *str, int len)
{
}

void
SOAPFaultHandler::endElement(const XML_Char *name)
{
}

