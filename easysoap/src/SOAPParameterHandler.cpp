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

#include "SOAPParameterHandler.h"
#include "SOAPNamespaces.h"
#include "SOAPParameter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPParameterHandler::SOAPParameterHandler()
: m_param(0)
{
}

SOAPParameterHandler::~SOAPParameterHandler()
{

}

SOAPParseEventHandler *
SOAPParameterHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	m_param->SetName(name);
	m_param->Reset();
	m_type = SOAPTypes::xsd_none;

	while (*attrs)
	{
		const XML_Char *tag = *attrs++;
		const XML_Char *val = *attrs++;

		if (sp_strcmp(tag, FULL_SOAP_XSI "|type") == 0)
		{
			m_type = SOAPTypes::GetXsdType(val);
			if (m_type == SOAPTypes::xsd_none)
			{
				// throw, unknown type
				return 0;
			}
			break;
		}
		else if (sp_strcmp(tag, FULL_SOAP_XSI "|null") == 0)
		{
			if (*val == '1')
				m_type = SOAPTypes::xsd_null;
		}
	}

	m_str = "";
	return this;
}

SOAPParseEventHandler *
SOAPParameterHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	// we shouldn't get in here...
	return 0;
}

void
SOAPParameterHandler::characterData(const XML_Char *str, int len)
{
	m_str.Append(str, len);
}

void
SOAPParameterHandler::endElement(const XML_Char *name)
{
	switch (m_type)
	{
	case SOAPTypes::xsd_int:
		m_param->SetInteger(m_str);
		break;
	case SOAPTypes::xsd_float:
		m_param->SetFloat(m_str);
		break;
	case SOAPTypes::xsd_double:
		m_param->SetDouble(m_str);
		break;
	case SOAPTypes::xsd_string:
		m_param->SetValue(m_str);
		break;
	case SOAPTypes::xsd_null:
		m_param->SetNull();
		break;
	case SOAPTypes::xsd_none:
		// What to do here.. I guess assume we got a string back?
		m_param->SetValue(m_str);
		break;
	default:
		break;
	}
}
