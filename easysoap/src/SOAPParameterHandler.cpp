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

#include "SOAPParameterHandler.h"
#include "SOAPArrayHandler.h"
#include "SOAPStructHandler.h"
#include "SOAPNamespaces.h"
#include "SOAPParameter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPParameterHandler::SOAPParameterHandler()
: m_param(0)
, m_structHandler(0)
, m_arrayHandler(0)
, m_setvalue(false)
{
}

SOAPParameterHandler::~SOAPParameterHandler()
{
	delete m_arrayHandler;
	delete m_structHandler;
}

SOAPParseEventHandler *
SOAPParameterHandler::start(const XML_Char *name, const XML_Char **attrs)
{
	m_param->SetName(name);
	m_param->Reset();
	m_type = SOAPTypes::xsd_none;
	m_setvalue = true;
	m_str = "";

	bool haveArrayType = false;
	bool haveType = false;
	while (*attrs)
	{
		const XML_Char *tag = *attrs++;
		const XML_Char *val = *attrs++;

		if (sp_strcmp(tag, FULL_SOAP_XSI PARSER_NS_SEP "type") == 0)
		{
			m_type = SOAPTypes::GetXsdType(val);
			haveType = true;
		}
		else if (sp_strcmp(tag, FULL_SOAP_ENC PARSER_NS_SEP "arrayType") == 0)
		{
			haveArrayType = true;
		}
		else if (sp_strcmp(tag, FULL_SOAP_XSI PARSER_NS_SEP "null") == 0)
		{
			if (sp_strcmp(val, "1") == 0 || sp_strcmp(val, "true") == 0)
			{
				m_param->SetNull();
				m_setvalue = false;
			}
		}
	}

	if (haveArrayType && m_type == SOAPTypes::xsd_none)
		m_type = SOAPTypes::soap_array;

	if (m_type == SOAPTypes::soap_array)
	{
		if (!m_arrayHandler)
			m_arrayHandler = new SOAPArrayHandler();
		m_arrayHandler->SetParameter(m_param);
		return m_arrayHandler->start(name, attrs);
	}
	else if (m_type == SOAPTypes::soap_struct)
	{
		if (!m_structHandler)
			m_structHandler = new SOAPStructHandler();
		m_structHandler->SetParameter(m_param);
		return m_structHandler->start(name, attrs);
	}
	else if (m_type == SOAPTypes::xsd_none)
	{
		return this;
	}

	return this;
}

SOAPParseEventHandler *
SOAPParameterHandler::startElement(const XML_Char *name, const XML_Char **attrs)
{
	//
	// If a parameter has an element, then it must
	// be a struct!
	if (!m_structHandler)
		m_structHandler = new SOAPStructHandler();
	m_type  = SOAPTypes::soap_struct;
	m_structHandler->SetParameter(m_param);
	m_param->SetType(SOAPTypes::soap_struct);
	return m_structHandler->startElement(name, attrs);
}

void
SOAPParameterHandler::characterData(const XML_Char *str, int len)
{
	m_str.Append(str, len);
}

void
SOAPParameterHandler::endElement(const XML_Char *name)
{
	if (m_setvalue)
	{
		switch (m_type)
		{
		case SOAPTypes::xsd_int:
		case SOAPTypes::xsd_integer:
			m_param->SetInteger(m_str);
			break;
		case SOAPTypes::xsd_float:
			m_param->SetFloat(m_str);
			break;
		case SOAPTypes::xsd_double:
			m_param->SetDouble(m_str);
			break;
		case SOAPTypes::xsd_string:
		case SOAPTypes::xsd_none:// What to do here.. I guess assume we got a string back?
			m_param->SetValue(m_str);
			break;
		default:
			break;
		}
	}
}
